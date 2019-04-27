#include <Windows.h>

#include "Hooks.h"

Hooks hooks;

static size_t calculateLength(uintptr_t* vmt)
{
    size_t length = 0;
    MEMORY_BASIC_INFORMATION memoryInfo;
    while (VirtualQuery((LPVOID)vmt[length], &memoryInfo, sizeof(memoryInfo)) && memoryInfo.Protect == PAGE_EXECUTE_READ)
        length++;
    return length;
}

void initializeHooks(void)
{

}
