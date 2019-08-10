#pragma once

#include <Windows.h>

typedef struct ConVar Convar;

ConVar* Cvar_findVar(PCSTR);
