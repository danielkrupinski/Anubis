#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <Windows.h>

typedef struct GlowObjectManager GlowObjectManager;
typedef struct GlobalVars GlobalVars;
typedef struct Vector Vector;

struct Memory {
    PVOID* clientMode;
    VOID(__fastcall* loadSky)(PCSTR);
    PVOID** present;
    PVOID** reset;
    GlowObjectManager* glowObjectManager;
    GlobalVars* globalVars;
    bool(__fastcall* isOtherEnemy)(PVOID, PVOID, PVOID);
    bool(__cdecl* lineGoesThroughSmoke)(Vector, Vector, SHORT);
    void(__cdecl* debugMsg)(const char* msg, ...);
};

VOID Memory_init(VOID);
const struct Memory* Memory(void);
