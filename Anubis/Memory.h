#pragma once

#include <stdint.h>
#include <Windows.h>

typedef struct GlowObjectManager GlowObjectManager;

typedef struct Memory {
    PVOID* clientMode;
    VOID(__fastcall* loadSky)(PCSTR);
    PVOID** present;
    PVOID** reset;
    GlowObjectManager* glowObjectManager;
} Memory;

VOID Memory_init(VOID);

extern Memory memory;
