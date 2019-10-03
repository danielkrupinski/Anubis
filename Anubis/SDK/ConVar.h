#pragma once

#include <Windows.h>

struct ConVar {
    BYTE pad[24];
    VOID(*changeCallback)(VOID);
    struct ConVar* parent;
    PCSTR defaultValue;
    PSTR string;
};

FLOAT ConVar_getFloat(struct ConVar*);
INT ConVar_getInt(struct ConVar*);
VOID ConVar_setValueString(struct ConVar*, PCSTR);
VOID ConVar_setValueFloat(struct ConVar*, FLOAT);
VOID ConVar_setValueInt(struct ConVar*, INT);
