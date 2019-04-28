#include <stdbool.h>
#include <Windows.h>

#include "Hooks.h"
#include "Memory.h"
#include "SDK/Engine.h"
#include "SDK/EntityList.h"
#include "SDK/UserCmd.h"

Hooks hooks;

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
    
    if (!cmd->command_number)
        return result;

    void* localPlayer = EntityList_getEntity(Engine_getLocalPlayer());
    if (!(*((int*)((char*)localPlayer + 0x104)) & 1))
        cmd->buttons &= ~IN_JUMP;

    return false;
}

void initializeHooks(void)
{
    hookVmt(memory.clientMode, &hooks.clientMode);
    hookMethod(&hooks.clientMode, 24, hookedCreateMove);
}
