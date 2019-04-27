#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <Windows.h>
#include <Psapi.h>

#include "Interfaces.h"
#include "Memory.h"

Memory memory;

static void* findPattern(const char* module, const char* pattern, size_t offset)
{
    MODULEINFO moduleInfo;

    if (GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(module), &moduleInfo, sizeof(moduleInfo))) {
        char* begin = moduleInfo.lpBaseOfDll;
        char* end = begin + moduleInfo.SizeOfImage - strlen(pattern) + 1;

        for (char* c = begin; c != end; c++) {
            bool matched = true;
            const char* it = c;
            const char* patternIt = pattern;

            if (*(c + strlen(pattern) - 1) != pattern[strlen(pattern) - 1])
                continue;

            for (; *patternIt; patternIt++) {
                if (*patternIt != '?' && *it != *patternIt) {
                    matched = false;
                    break;
                }
                it++;
            }
            if (matched)
                return c + offset;
        }
    }
    char buf[100];
    sprintf_s(buf, sizeof(buf), "Failed to find pattern in %s!", module);
    MessageBox(NULL, buf, "Error", MB_OK | MB_ICONERROR);
    exit(EXIT_FAILURE);
}

void initializeMemory(void)
{
    memory.clientMode = **((void***)(((char*)*interfaces.client)[10] + 5));
    memory.loadSky = findPattern("engine", "\x55\x8B\xEC\x81\xEC????\x56\x57\x8B\xF9\xC7\x45", 0);
}
