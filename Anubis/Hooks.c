#include <Windows.h>

#include "Hooks.h"
#include "Memory.h"

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
    vmtHook->oldVmt = *((uintptr_t * *)base);
    vmtHook->length = calculateLength(vmtHook->oldVmt) + 1;
    vmtHook->newVmt = malloc(vmtHook->length * sizeof(uintptr_t));
    memcpy(vmtHook->newVmt, vmtHook->oldVmt - 1, vmtHook->length * sizeof(uintptr_t));
    *((uintptr_t * *)base) = vmtHook->newVmt + 1;
}

void initializeHooks(void)
{
    hookVmt(memory.clientMode, &hooks.clientMode);
}
