#pragma once

#include <Windows.h>

typedef struct UtlVector {
    PVOID memory;
    INT allocationCount;
    INT growSize;
    INT size;
    PVOID elements;
} UtlVector;
