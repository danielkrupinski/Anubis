#include <d3d9.h>
#include <stdbool.h>
#include <Windows.h>

#include "Hacks/Misc.h"
#include "Hooks.h"
#include "Memory.h"
#include "SDK/Engine.h"
#include "SDK/EntityList.h"
#include "SDK/UserCmd.h"
#include "GUI.h"

Hooks hooks;

static LRESULT __stdcall hookedWndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (GUI_handleInput(window, msg, wParam, lParam))
        return true;
    return CallWindowProc(hooks.originalWndProc, window, msg, wParam, lParam);
}

static HRESULT __stdcall hookedPresent(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND windowOverride, const RGNDATA* dirtyRegion)
{
    static bool init = false;
    if (!init) {
        GUI_init(device);
        init = true;
    }

    if (isGuiOpen) {
        IDirect3DDevice9_SetRenderState(device, D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
        IDirect3DVertexDeclaration9* vertexDeclaration;

        GUI_render();

        IDirect3DDevice9_SetVertexDeclaration(device, vertexDeclaration);
        IDirect3D9_Release(vertexDeclaration);
    }
    return hooks.originalPresent(device, src, dest, windowOverride, dirtyRegion);
}

static size_t calculateLength(uintptr_t* vmt)
{
    size_t length = 0;
    MEMORY_BASIC_INFORMATION memoryInfo;
    while (VirtualQuery((LPVOID)vmt[length], &memoryInfo, sizeof(memoryInfo)) && memoryInfo.Protect == PAGE_EXECUTE_READ)
        length++;
    return length;
}

static void hookVmt(void* const base, VmtHook* vmtHook)
{
    vmtHook->base = base;
    vmtHook->oldVmt = *((uintptr_t**)base);
    vmtHook->length = calculateLength(vmtHook->oldVmt) + 1;
    vmtHook->newVmt = malloc(vmtHook->length * sizeof(uintptr_t));
    memcpy(vmtHook->newVmt, vmtHook->oldVmt - 1, vmtHook->length * sizeof(uintptr_t));
    *((uintptr_t**)base) = vmtHook->newVmt + 1;
}

static void hookMethod(VmtHook* vmtHook, size_t index, void* function)
{
    if (index < vmtHook->length)
        vmtHook->newVmt[index + 1] = (uintptr_t)function;
}

static bool __stdcall hookedCreateMove(float inputSampleTime, UserCmd* cmd)
{
    bool result = ((bool(__stdcall*)(float, UserCmd*))hooks.clientMode.oldVmt[24])(inputSampleTime, cmd);
    
    if (!cmd->commandNumber)
        return result;

    Misc_autostrafe(cmd);
    Misc_bunnyhop(cmd);

    return false;
}

void initializeHooks(void)
{
    hookVmt(memory.clientMode, &hooks.clientMode);
    hookMethod(&hooks.clientMode, 24, hookedCreateMove);

    HWND window = FindWindowA("Valve001", NULL);
    hooks.originalWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)hookedWndProc);

    hooks.originalPresent = **memory.present;
    **memory.present = hookedPresent;
}
