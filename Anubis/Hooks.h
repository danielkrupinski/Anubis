#pragma once

#include <d3d9.h>
#include <Windows.h>

typedef struct {
    LPCVOID base;
    PUINT_PTR oldVmt;
    PUINT_PTR newVmt;
    SIZE_T length;
} VmtHook;

typedef struct {
    HRESULT(__stdcall* originalPresent)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
    WNDPROC originalWndProc;
    VmtHook clientMode;
    VmtHook surface;
} Hooks;

VOID Hooks_init(VOID);

extern Hooks hooks;
