#include <Windows.h>

#include "Hooks.h"
#include "Interfaces.h"
#include "Memory.h"

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        initializeInterfaces();
        initializeMemory();
        initializeHooks();
    }
    return TRUE;
}

