#include <d3d9.h>
#include <stdbool.h>
#include <Windows.h>
#include <intrin.h>

#include "GUI.h"
#include "Hacks/Esp.h"
#include "Hacks/Glow.h"
#include "Hacks/Misc.h"
#include "Hacks/Triggerbot.h"
#include "Hooks.h"
#include "Interfaces.h"
#include "Memory.h"

#include "SDK/Engine.h"
#include "SDK/EntityList.h"
#include "SDK/GlobalVars.h"
#include "SDK/Panel.h"
#include "SDK/Surface.h"
#include "SDK/UserCmd.h"
#include "SDK/Utils.h"

struct Hooks hooks;

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

    if (isGuiOpen)
        GUI_render();

    return hooks.originalPresent(device, src, dest, windowOverride, dirtyRegion);
}

static HRESULT WINAPI hookedReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
{
    GUI_invalidateDeviceObjects();
    HRESULT result = hooks.originalReset(device, params);
    GUI_createDeviceObjects();
    return result;
}

static SIZE_T calculateLength(PUINT_PTR vmt)
{
    SIZE_T length = 0;
    MEMORY_BASIC_INFORMATION memoryInfo;
    while (VirtualQuery((LPCVOID)vmt[length], &memoryInfo, sizeof(memoryInfo)) && memoryInfo.Protect == PAGE_EXECUTE_READ)
        length++;
    return length;
}

static void hookVmt(PVOID base, struct VmtHook* vmtHook)
{
    vmtHook->base = base;
    vmtHook->oldVmt = *((PUINT_PTR*)base);
    vmtHook->length = calculateLength(vmtHook->oldVmt) + 1;
    PVOID newVmt = malloc(vmtHook->length * sizeof(uintptr_t));
    if (newVmt) {
        vmtHook->newVmt = newVmt;
        memcpy(vmtHook->newVmt, vmtHook->oldVmt - 1, vmtHook->length * sizeof(uintptr_t));
        *((PUINT_PTR*)base) = vmtHook->newVmt + 1;
    }
}

static void restoreVmt(struct VmtHook* vmtHook)
{
    *((PUINT_PTR*)vmtHook->base) = vmtHook->oldVmt;
    free(vmtHook->newVmt);
}

static void hookMethod(struct VmtHook* vmtHook, SIZE_T index, PVOID function)
{
    if (index < vmtHook->length)
        vmtHook->newVmt[index + 1] = (UINT_PTR)function;
}

static bool __stdcall createMove(FLOAT inputSampleTime, UserCmd* cmd)
{
    bool result = CALL_ORIGINAL(bool(__fastcall*)(PVOID, PVOID, FLOAT, UserCmd*), Memory()->clientMode, hooks.clientMode.oldVmt, 24, inputSampleTime, cmd);

    if (!cmd->commandNumber)
        return result;

    bool* sendPacket = (bool*)(*((PUINT_PTR)_AddressOfReturnAddress() - 1) - 0x1C);

    GlobalVars_serverTime(cmd);
    Misc_autostrafe(cmd);
    Misc_bunnyhop(cmd);
    Misc_moonwalk(cmd);
    Triggerbot_run(cmd);

    Angles_normalize(&cmd->viewangles);

    cmd->viewangles.x = min(89.0f, max(cmd->viewangles.x, -89.0f));
    cmd->viewangles.y = min(180.0f, max(cmd->viewangles.y, -180.0f));
    cmd->viewangles.z = 0.0f;

    return false;
}

static INT __stdcall doPostScreenEffects(INT param)
{
    Glow_render();
    return CALL_ORIGINAL(INT(__fastcall*)(PVOID, PVOID, INT), Memory()->clientMode, hooks.clientMode.oldVmt, 44, param);
}

static VOID __stdcall lockCursor(VOID)
{
    if (isGuiOpen) {
        Surface_unlockCursor();
        return;
    }
    CALL_ORIGINAL(VOID(__fastcall*)(PVOID, PVOID), Interfaces()->surface, hooks.surface.oldVmt, 67);
}

static VOID __stdcall paintTraverse(UINT panel, BOOLEAN forceRepaint, BOOLEAN allowForce)
{
    if (!strcmp(Panel_getName(panel), "MatSystemTopPanel")) {
        Esp_render();
    }

    CALL_ORIGINAL(VOID(__fastcall*)(PVOID, PVOID, UINT, BOOLEAN, BOOLEAN), Interfaces()->panel, hooks.panel.oldVmt, 41, panel, forceRepaint, allowForce);
}

VOID Hooks_init(VOID)
{
    hookVmt(Memory()->clientMode, &hooks.clientMode);
    hookMethod(&hooks.clientMode, 24, createMove);
    hookMethod(&hooks.clientMode, 44, doPostScreenEffects);

    hookVmt(Interfaces()->panel, &hooks.panel);
    hookMethod(&hooks.panel, 41, paintTraverse);

    hookVmt(Interfaces()->surface, &hooks.surface);
    hookMethod(&hooks.surface, 67, lockCursor);

    HWND window = FindWindowA("Valve001", NULL);
    hooks.originalWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)hookedWndProc);

    hooks.originalPresent = **Memory()->present;
    **Memory()->present = hookedPresent;

    hooks.originalReset = **Memory()->reset;
    **Memory()->reset = hookedReset;
}

VOID Hooks_restore(VOID)
{
    restoreVmt(&hooks.clientMode);
    restoreVmt(&hooks.panel);
    restoreVmt(&hooks.surface);

    SetWindowLongPtr(FindWindowW(L"Valve001", NULL), GWLP_WNDPROC, (LONG_PTR)hooks.originalWndProc);

    **Memory()->present = hooks.originalPresent;
    **Memory()->reset = hooks.originalReset;
}
