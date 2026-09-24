#ifndef A2_H
#define A2_H

#include <pthread.h>
#include "square.h"

typedef struct {
    pthread_t thread;
    int id;
    long size;
    long completed;
    unsigned long invocations;
} worker_t;

void *worker_main(void *arg);

#endif