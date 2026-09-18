#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "square.h"

static volatile sig_atomic_t child_alarm;
static volatile sig_atomic_t parent_alarm;

static void child_alarm_handler(int signal_number)
{
    (void)signal_number;
    child_alarm = 1;
}

static void parent_alarm_handler(int signal_number)
{
    (void)signal_number;
    parent_alarm = 1;
}

static int install_handler(int signal_number, void (*handler)(int))
{
    struct sigaction action;

    action.sa_handler = handler;
    action.sa_flags = 0;
    if (sigemptyset(&action.sa_mask) == -1)
        return -1;
    return sigaction(signal_number, &action, NULL);
}

static void child_process(int id, long size)
{
    unsigned long invocations = 0;
    long completed = 0;
    long n;

    if (install_handler(SIGALRM, child_alarm_handler) == -1)
        _exit(EXIT_FAILURE);

    for (n = 1; n <= size && !child_alarm; ++n) {
        (void)Square(n, &invocations);
        ++completed;
    }

    if (child_alarm) {
        printf("Child %d: %ld squares completed, %lu Square invocations, "
               "stopped by SIGALRM\n", id, completed, invocations);
        fflush(stdout);
        _exit(EXIT_FAILURE);
    }

    printf("Child %d: %ld squares completed, %lu Square invocations, "
           "finished normally\n", id, completed, invocations);
    fflush(stdout);
    _exit(EXIT_SUCCESS);
}

static void timer_process(const pid_t *children, int child_count,
                          unsigned int deadline, pid_t parent)
{
    unsigned int remaining = deadline;
    int i;

    while (remaining != 0) {
        remaining = sleep(remaining);
    }

    for (i = 0; i < child_count; ++i)
        (void)kill(children[i], SIGALRM);
    (void)kill(parent, SIGALRM);
    _exit(EXIT_SUCCESS);
}

static int parse_long(const char *text, long *value)
{
    char *end;

    errno = 0;
    *value = strtol(text, &end, 10);
    return errno == 0 && *text != '\0' && *end == '\0';
}

int main(int argc, char **argv)
{
    long thread_count_value;
    long deadline_value;
    long size;
    pid_t *children;
    pid_t timer;
    int child_count = 0;
    int status;
    int i;
    int j;
    int finished;
    pid_t result;

    if (argc != 4 || !parse_long(argv[1], &thread_count_value) ||
        !parse_long(argv[2], &deadline_value) || !parse_long(argv[3], &size) ||
        thread_count_value <= 0 || thread_count_value > INT_MAX ||
        deadline_value < 0 || deadline_value > UINT_MAX || size <= 0) {
        fprintf(stderr, "Usage: %s threads deadline size\n", argv[0]);
        return EXIT_FAILURE;
    }

    if ((unsigned long)thread_count_value > (size_t)-1 / sizeof(*children)) {
        fprintf(stderr, "too many threads\n");
        return EXIT_FAILURE;
    }

    children = calloc((size_t)thread_count_value, sizeof(*children));
    if (children == NULL) {
        perror("calloc");
        return EXIT_FAILURE;
    }

    if (install_handler(SIGALRM, parent_alarm_handler) == -1) {
        perror("sigaction");
        free(children);
        return EXIT_FAILURE;
    }

    for (i = 0; i < thread_count_value; ++i) {
        children[i] = fork();
        if (children[i] == -1) {
            perror("fork");
            for (j = 0; j < child_count; ++j)
                (void)kill(children[j], SIGALRM);
            break;
        }
        if (children[i] == 0)
            child_process(i + 1, size);
        ++child_count;
    }

    if (child_count != thread_count_value) {
        for (i = 0; i < child_count; ++i)
            (void)waitpid(children[i], NULL, 0);
        free(children);
        return EXIT_FAILURE;
    }

    timer = fork();
    if (timer == -1) {
        perror("fork timer");
        for (i = 0; i < child_count; ++i)
            (void)kill(children[i], SIGALRM);
        for (i = 0; i < child_count; ++i)
            (void)waitpid(children[i], NULL, 0);
        free(children);
        return EXIT_FAILURE;
    }
    if (timer == 0)
        timer_process(children, child_count, (unsigned int)deadline_value,
                      getppid());

    for (finished = 0; finished < child_count;) {
        result = wait(&status);
        if (result == -1) {
            if (errno == EINTR && parent_alarm)
                continue;
            perror("wait");
            break;
        }
        for (i = 0; i < child_count; ++i) {
            if (result == children[i]) {
                ++finished;
                break;
            }
        }
    }

    (void)kill(timer, SIGTERM);
    (void)waitpid(timer, NULL, 0);
    free(children);
    return EXIT_SUCCESS;
}
