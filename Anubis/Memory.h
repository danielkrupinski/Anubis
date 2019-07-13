#pragma once

#include <stdint.h>
#include <Windows.h>

typedef struct Memory {
    PVOID* clientMode;
    VOID(__fastcall* loadSky)(PCSTR);
    PVOID** present;
    PVOID** reset;
} Memory;

VOID Memory_init(VOID);

extern Memory memory;
