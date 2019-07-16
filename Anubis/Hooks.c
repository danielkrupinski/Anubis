#include <d3d9.h>
#include <stdbool.h>
#include <Windows.h>
#include <intrin.h>

#include "GUI.h"
#include "Hacks/Misc.h"
#include "Hooks.h"
#include "Interfaces.h"
#include "Memory.h"

#include "SDK/Engine.h"
#include "SDK/EntityList.h"
#include "SDK/Surface.h"
#include "SDK/UserCmd.h"
#include "SDK/Utils.h"

Hooks hooks;

static LRESULT WINAPI hookedWndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (GUI_handleInput(window, msg, wParam, lParam))
        return true;
    return CallWindowProc(hooks.originalWndProc, window, msg, wParam, lParam);
}

static HRESULT WINAPI hookedPresent(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND windowOverride, const RGNDATA* dirtyRegion)
{
    static bool init = false;
    if (!init) {
        GUI_init(device);
        init = true;
    }

    if (isGuiOpen) {
        IDirect3DDevice9_SetRenderState(device, D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
        IDirect3DVertexDeclaration9* vertexDeclaration;
        IDirect3DDevice9_GetVertexDeclaration(device, &vertexDeclaration);

        GUI_render();

        IDirect3DDevice9_SetVertexDeclaration(device, vertexDeclaration);
        IDirect3DVertexDeclaration9_Release(vertexDeclaration);
    }
    return hooks.originalPresent(device, src, dest, windowOverride, dirtyRegion);
}

static SIZE_T calculateLength(PUINT_PTR vmt)
{
    SIZE_T length = 0;
    MEMORY_BASIC_INFORMATION memoryInfo;
    while (VirtualQuery((LPCVOID)vmt[length], &memoryInfo, sizeof(memoryInfo)) && memoryInfo.Protect == PAGE_EXECUTE_READ)
        length++;
    return length;
}

static void hookVmt(PVOID base, VmtHook* vmtHook)
{
    vmtHook->base = base;
    vmtHook->oldVmt = *((PUINT_PTR*)base);
    vmtHook->length = calculateLength(vmtHook->oldVmt) + 1;
    vmtHook->newVmt = malloc(vmtHook->length * sizeof(uintptr_t));
    memcpy(vmtHook->newVmt, vmtHook->oldVmt - 1, vmtHook->length * sizeof(uintptr_t));
    *((PUINT_PTR*)base) = vmtHook->newVmt + 1;
}

static void restoreVmt(VmtHook* vmtHook)
{
    *((PUINT_PTR*)vmtHook->base) = vmtHook->oldVmt;
    free(vmtHook->newVmt);
}

static void hookMethod(VmtHook* vmtHook, SIZE_T index, PVOID function)
{
    if (index < vmtHook->length)
        vmtHook->newVmt[index + 1] = (UINT_PTR)function;
}

static bool __stdcall hookedCreateMove(FLOAT inputSampleTime, UserCmd* cmd)
{
    bool result;
    CALL_ORIGINAL_RETURN_TO_VARIABLE(bool(__fastcall*)(PVOID, PVOID, FLOAT, UserCmd*), memory.clientMode, hooks.clientMode.oldVmt, 24, result, inputSampleTime, cmd);
    
    if (!cmd->commandNumber)
        return result;

    bool* sendPacket = (bool*)(*((PUINT_PTR)_AddressOfReturnAddress() - 1) - 0x1C);

    Misc_autostrafe(cmd);
    Misc_bunnyhop(cmd);

    Angles_normalize(&cmd->viewangles);

    cmd->viewangles.x = min(89.0f, max(cmd->viewangles.x, -89.0f));
    cmd->viewangles.y = min(180.0f, max(cmd->viewangles.y, -180.0f));
    cmd->viewangles.z = 0.0f;

    return false;
}

static VOID __stdcall lockCursor(VOID)
{
    if (isGuiOpen) {
        Surface_unlockCursor();
        return;
    }
    CALL_ORIGINAL(VOID(__fastcall*)(PVOID, PVOID), interfaces.surface, hooks.surface.oldVmt, 67);
}

VOID Hooks_init(VOID)
{
    hookVmt(memory.clientMode, &hooks.clientMode);
    hookMethod(&hooks.clientMode, 24, hookedCreateMove);

    hookVmt(interfaces.surface, &hooks.surface);
    hookMethod(&hooks.surface, 67, lockCursor);

    HWND window = FindWindowA("Valve001", NULL);
    hooks.originalWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)hookedWndProc);

    hooks.originalPresent = **memory.present;
    **memory.present = hookedPresent;
}
