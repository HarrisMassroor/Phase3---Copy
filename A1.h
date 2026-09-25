#ifndef A1_H
#define A1_H

#include <windows.h>

typedef struct {
    int id;
    long size;
} WorkerData;

extern volatile BOOL keepRunning;
extern unsigned long invocationCount[1024];

DWORD WINAPI Worker(void *parameter);

#endif
