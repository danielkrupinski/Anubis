#pragma once

#include <d3d9.h>
#include <Windows.h>

typedef struct {
    PVOID base;
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

#ifdef __cplusplus
extern "C"
{
#endif

VOID Hooks_init(VOID);
VOID Hooks_restore(VOID);

extern Hooks hooks;

#ifdef __cplusplus
}
#endif
