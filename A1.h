#ifndef A1_H
#define A1_H

#include <windows.h>
#include "square.h"

typedef struct {
    int id;
    long size;
} WorkerData;

extern volatile BOOL keepRunning;

DWORD WINAPI Worker(void *parameter);

#endif