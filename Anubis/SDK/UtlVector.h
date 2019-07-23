#pragma once

#include <Windows.h>

typedef struct UtlVector {
    PINT memory;
    INT allocationCount;
    INT growSize;
    INT size;
    PVOID elements;
} UtlVector;
