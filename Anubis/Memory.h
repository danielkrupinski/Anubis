#pragma once

#include <stdint.h>
#include <Windows.h>

typedef struct GlowObjectManager GlowObjectManager;
typedef struct GlobalVars GlobalVars;

typedef struct Memory {
    PVOID* clientMode;
    VOID(__fastcall* loadSky)(PCSTR);
    PVOID** present;
    PVOID** reset;
    GlowObjectManager* glowObjectManager;
    GlobalVars* globalVars;
} Memory;

VOID Memory_init(VOID);

extern Memory memory;
