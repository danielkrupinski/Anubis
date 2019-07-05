#pragma once

#include <d3d9.h>
#include <stdint.h>
#include <Windows.h>

typedef struct {
    void* base;
    uintptr_t* oldVmt;
    uintptr_t* newVmt;
    size_t length;
} VmtHook;

typedef struct {
    HRESULT(__stdcall* originalPresent)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
    WNDPROC originalWndProc;
    VmtHook clientMode;
    VmtHook surface;
} Hooks;

void initializeHooks(void);

extern Hooks hooks;
