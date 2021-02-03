#include "Cvar.h"
#include "../Interfaces.h"
#include "Utils.h"

ConVar* Cvar_findVar(PCSTR name)
{
    return CALL_VIRTUAL_METHOD(ConVar* (__fastcall*)(PVOID, PVOID, PCSTR), Interfaces()->cvar, 15, name);
}
