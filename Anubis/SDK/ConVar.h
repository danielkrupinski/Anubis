#pragma once

#include <Windows.h>

typedef struct ConVar {
    BYTE pad[24];
    VOID(*changeCallback)(VOID);
    struct ConVar* parent;
    PCSTR defaultValue;
    PSTR string;
} ConVar;

FLOAT ConVar_getFloat(ConVar*);
INT ConVar_getInt(ConVar*);
VOID ConVar_setValueString(ConVar*, PCSTR);
VOID ConVar_setValueFloat(ConVar*, FLOAT);
VOID ConVar_setValueInt(ConVar*, INT);
