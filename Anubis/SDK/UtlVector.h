#pragma once

#include <Windows.h>

struct UtlVector {
    PINT memory;
    INT allocationCount;
    INT growSize;
    INT size;
    PVOID elements;
};
