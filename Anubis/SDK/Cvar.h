#pragma once

#include <Windows.h>

typedef struct ConVar ConVar;

ConVar* Cvar_findVar(PCSTR);
