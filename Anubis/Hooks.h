#pragma once

#include <stdint.h>
#include <Windows.h>

typedef struct {
    void* base;
    uintptr_t* oldVmt;
    uintptr_t* newVmt;
    size_t length;
} VmtHook;

typedef struct {
    WNDPROC originalWndProc;
    VmtHook clientMode;
} Hooks;

void initializeHooks(void);

extern Hooks hooks;
