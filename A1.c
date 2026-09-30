#include <windows.h
#include <stdio.h>
#include <stdlib.h>
#include "A1.h"
#include "square.h"

volatile BOOL keepRunning = TRUE;

/* Per-thread invocation counters */
unsigned long invocationCount[1024];

DWORD WINAPI Worker(void *parameter)
{
    WorkerData *data = (WorkerData *)parameter;
    long completed = 0;
    long n;

    for (n = 1; n <= data->size && keepRunning; ++n) {
        ++invocationCount[data->id - 1];   /* count Square() invocation */
        (void)Square((int)n);              /* correct Square() call */
        ++completed;
    }

    printf("Thread %d: %ld squares completed, %lu Square invocations\n",
           data->id, completed, invocationCount[data->id - 1]);

    return 0;
}
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "A1.h"
#include "square.h"

volatile BOOL keepRunning = TRUE;

/* Per-thread invocation counters */
unsigned long invocationCount[1024];

DWORD WINAPI Worker(void *parameter)
{
    WorkerData *data = (WorkerData *)parameter;
    long completed = 0;
    long n;

    for (n = 1; n <= data->size && keepRunning; ++n) {
        ++invocationCount[data->id - 1];   /* count Square() invocation */
        (void)Square((int)n);              /* correct Square() call */
        ++completed;
    }

    printf("Thread %d: %ld squares completed, %lu Square invocations\n",
           data->id, completed, invocationCount[data->id - 1]);

    return 0;
}

int main(int argc, char **argv)
{
    int count, i;
    unsigned long deadline;
    long size;
    HANDLE *threads;
    WorkerData *data;
    SYSTEMTIME start;

    if (argc != 4) {
        fprintf(stderr, "Usage: %s threads deadline size\n", argv[0]);
        return EXIT_FAILURE;
    }

    count = atoi(argv[1]);
    deadline = strtoul(argv[2], NULL, 10);
    size = strtol(argv[3], NULL, 10);

    if (count <= 0 || size <= 0) {
        fprintf(stderr, "threads and size must be positive\n");
        return EXIT_FAILURE;
    }

    threads = (HANDLE *)calloc((size_t)count, sizeof(*threads));
    data = (WorkerData *)calloc((size_t)count, sizeof(*data));

    if (threads == NULL || data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(threads);
        free(data);
        return EXIT_FAILURE;
    }

    GetSystemTime(&start);
    printf("Parent started at %02u:%02u:%02u.%03u UTC\n",
           start.wHour, start.wMinute, start.wSecond, start.wMilliseconds);

    for (i = 0; i < count; ++i) {
        data[i].id = i + 1;
        data[i].size = size;

        threads[i] = CreateThread(NULL, 0, Worker, &data[i], 0, NULL);
        if (threads[i] == NULL) {
            fprintf(stderr, "CreateThread failed for thread %d\n", i + 1);
            keepRunning = FALSE;
            break;
        }
    }

    if (deadline > 4294967UL) {
        Sleep(INFINITE);
    } else {
        Sleep((DWORD)(deadline * 1000UL));
    }

    keepRunning = FALSE;

    for (i = 0; i < count; ++i) {
        if (threads[i] != NULL) {
            WaitForSingleObject(threads[i], INFINITE);
            CloseHandle(threads[i]);
        }
    }

    free(data);
    free(threads);

    return EXIT_SUCCESS;
}
