#pragma once

#include <stdint.h>

typedef struct {
    void* base;
    void* oldVmt;
    void* newVmt;
    size_t length;
} VmtHook;

void initializeHooks(void);
