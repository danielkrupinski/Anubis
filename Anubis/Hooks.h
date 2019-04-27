#pragma once

#include <stdint.h>

typedef struct {
    void* base;
    uintptr_t* oldVmt;
    uintptr_t* newVmt;
    size_t length;
} VmtHook;

typedef struct {
    VmtHook clientMode;
} Hooks;

void initializeHooks(void);

extern Hooks hooks;
