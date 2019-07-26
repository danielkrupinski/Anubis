#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <Windows.h>
#include <Psapi.h>

#include "Interfaces.h"
#include "Memory.h"

Memory memory;

static void* findPattern(PCWSTR module, PCSTR pattern, SIZE_T offset)
{
    MODULEINFO moduleInfo;
    HMODULE moduleHandle = GetModuleHandleW(module);

    if (moduleHandle && GetModuleInformation(GetCurrentProcess(), moduleHandle, &moduleInfo, sizeof(moduleInfo))) {
        PCHAR begin = moduleInfo.lpBaseOfDll;
        PCHAR end = begin + moduleInfo.SizeOfImage;

        for (PCHAR c = begin; c != end; c++) {
            bool matched = true;

            for (PCSTR patternIt = pattern, it = c; *patternIt; patternIt++, it++) {
                if (*patternIt != '?' && *it != *patternIt) {
                    matched = false;
                    break;
                }
            }
            if (matched)
                return c + offset;
        }
    }
    WCHAR buf[100];
    swprintf(buf, sizeof(buf) / sizeof(WCHAR), L"Failed to find pattern in %s.dll!", module);
    MessageBoxW(NULL, buf, L"Error", MB_OK | MB_ICONERROR);
    exit(EXIT_FAILURE);
}

static PVOID relativeToAbsolute(int* address)
{
    return (PBYTE)(address + 1) + *address;
}

VOID Memory_init(VOID)
{
    memory.clientMode = **((PVOID**)(interfaces.client[0][10] + 5));
    memory.loadSky = findPattern(L"engine", "\x55\x8B\xEC\x81\xEC????\x56\x57\x8B\xF9\xC7\x45", 0);
    memory.present = findPattern(L"gameoverlayrenderer", "\xFF\x15????\x8B\xF8\x85\xDB", 2);
    memory.reset = findPattern(L"gameoverlayrenderer", "\xC7\x45?????\xFF\x15????\x8B\xF8", 9);
    memory.glowObjectManager = *(GlowObjectManager**)findPattern(L"client_panorama", "\x0F\x11\x05????\x83\xC8\x01", 3);
    memory.globalVars = **((PVOID**)(interfaces.client[0][11] + 10));
    memory.isOtherEnemy = relativeToAbsolute(findPattern(L"client_panorama", "\xE8????\x02\xC0", 1));
    memory.lineGoesThroughSmoke = relativeToAbsolute(findPattern(L"client_panorama", "\xE8????\x8B\x4C\x24\x30\x33\xD2", 1));
}
