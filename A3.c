#define _POSIX_C_SOURCE 200809L

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "square.h"

static volatile int keepRunning = 1;

typedef struct {
    int id;
    long size;
} worker_data_t;

static void *worker_main(void *argument)
{
    worker_data_t *data = argument;
    unsigned long invocations = 0;
    long completed = 0;
    long n;

    for (n = 1; n <= data->size && keepRunning; ++n) {
        (void)Square(n, &invocations);
        ++completed;
    }

    printf("Thread %d: %ld squares completed, %lu Square invocations\n",
           data->id, completed, invocations);
    free(data);
    return NULL;
}

int main(int argc, char **argv)
{
    int thread_count;
    long deadline_seconds;
    long size;
    int i;
    pthread_t thread;
    worker_data_t *data;
    int result;
    struct timespec delay;

    if (argc != 4) {
        fprintf(stderr, "Usage: %s threads deadline size\n", argv[0]);
        return EXIT_FAILURE;
    }

    thread_count = atoi(argv[1]);
    deadline_seconds = atol(argv[2]);
    size = atol(argv[3]);
    if (thread_count <= 0 || deadline_seconds < 0 || size <= 0) {
        fprintf(stderr, "threads and size must be positive; "
                "deadline must not be negative\n");
        return EXIT_FAILURE;
    }

    for (i = 0; i < thread_count; ++i) {
        thread = (pthread_t)0;
        data = malloc(sizeof(*data));
        if (data == NULL) {
            perror("malloc");
            keepRunning = 0;
            break;
        }

        data->id = i + 1;
        data->size = size;
        result = pthread_create(&thread, NULL, worker_main, data);
        if (result != 0) {
            fprintf(stderr, "pthread_create failed for thread %d\n", i + 1);
            free(data);
            keepRunning = 0;
            break;
        }
        pthread_detach(thread);
    }

    delay.tv_sec = deadline_seconds;
    delay.tv_nsec = 0;
    nanosleep(&delay, NULL);
    keepRunning = 0;

    pthread_exit(NULL);
}
