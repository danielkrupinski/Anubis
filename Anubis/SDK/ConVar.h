#pragma once

#include <Windows.h>

typedef struct ConVar {
    BYTE pad[24];
    VOID(*changeCallback)(VOID);
    struct ConVar* parent;
    PCSTR defaultValue;
    PSTR string;
} Convar;
