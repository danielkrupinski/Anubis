#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <Windows.h>
#include <Psapi.h>

#include "Interfaces.h"
#include "Memory.h"

Memory memory;

static void* findPattern(PCSTR module, PCSTR pattern, SIZE_T offset)
{
    MODULEINFO moduleInfo;

    if (GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(module), &moduleInfo, sizeof(moduleInfo))) {
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
    CHAR buf[100];
    sprintf_s(buf, sizeof(buf), "Failed to find pattern in %s!", module);
    MessageBox(NULL, buf, "Error", MB_OK | MB_ICONERROR);
    exit(EXIT_FAILURE);
}

VOID Memory_init(VOID)
{
    memory.clientMode = **((PVOID**)(interfaces.client[0][10] + 5));
    memory.loadSky = findPattern("engine", "\x55\x8B\xEC\x81\xEC????\x56\x57\x8B\xF9\xC7\x45", 0);
    memory.present = findPattern("gameoverlayrenderer", "\xFF\x15????\x8B\xF8\x85\xDB", 2);
    memory.reset = findPattern("gameoverlayrenderer", "\xC7\x45?????\xFF\x15????\x8B\xF8", 9);
}
