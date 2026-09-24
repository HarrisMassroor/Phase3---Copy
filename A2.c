#include <pthread.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "A2.h"

void *worker_main(void *arg)
{
    worker_t *worker = arg;
    long n;

    (void)pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    (void)pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    for (n = 1; n <= worker->size; ++n) {
        (void)Square(n, &worker->invocations);
        ++worker->completed;
    }

    return NULL;
}

int main(int argc, char **argv)
{
    int count;
    unsigned int deadline;
    long size;
    worker_t *workers;
    int created = 0;
    long deadline_value;
    int i;
    int rc;

    if (argc != 4) {
        fprintf(stderr, "Usage: %s threads deadline size\n", argv[0]);
        return EXIT_FAILURE;
    }

    count = atoi(argv[1]);
    deadline_value = atol(argv[2]);
    size = atol(argv[3]);
    if (count <= 0 || deadline_value < 0 ||
        deadline_value > UINT_MAX || size <= 0) {
        fprintf(stderr, "threads and size must be positive; "
                "deadline must not be negative\n");
        return EXIT_FAILURE;
    }
    deadline = (unsigned int)deadline_value;

    workers = calloc((size_t)count, sizeof(*workers));
    if (workers == NULL) {
        perror("calloc");
        free(workers);
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; ++i) {
        workers[i].id = i + 1;
        workers[i].size = size;
        rc = pthread_create(&workers[i].thread, NULL, worker_main, &workers[i]);
        if (rc != 0) {
            fprintf(stderr, "pthread_create failed for thread %d\n", i + 1);
            break;
        }
        ++created;
    }

    sleep(deadline);

    for (i = 0; i < created; ++i) {
        (void)pthread_cancel(workers[i].thread);
    }

    for (i = 0; i < created; ++i) {
        void *result;
        (void)pthread_join(workers[i].thread, &result);
        printf("Thread %d: %ld squares completed, %lu Square invocations, %s\n",
               workers[i].id, workers[i].completed, workers[i].invocations,
               result == PTHREAD_CANCELED ? "cancelled" : "finished normally");
    }

    free(workers);
    return created == count ? EXIT_SUCCESS : EXIT_FAILURE;
}