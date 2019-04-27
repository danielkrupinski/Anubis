#pragma once

#include <stdint.h>

typedef struct {
    void* base;
    void* oldVmt;
    void* newVmt;
    size_t length;
} VmtHook;

typedef struct {
    VmtHook clientMode;
} Hooks;

void initializeHooks(void);

extern Hooks hooks;
