#pragma once

#include <d3d9.h>
#include <Windows.h>

struct VmtHook {
    PVOID base;
    PUINT_PTR oldVmt;
    PUINT_PTR newVmt;
    SIZE_T length;
};

struct Hooks {
    HRESULT(WINAPI* originalPresent)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
    HRESULT(WINAPI* originalReset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
    WNDPROC originalWndProc;
    struct VmtHook clientMode;
    struct VmtHook panel;
    struct VmtHook surface;
};

#ifdef __cplusplus
extern "C"
{
#endif

VOID Hooks_init(VOID);
VOID Hooks_restore(VOID);

#ifdef __cplusplus
}
#endif
