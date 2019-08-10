#include "ConVar.h"
#include "Utils.h"

FLOAT ConVar_getFloat(ConVar* conVar)
{
    return CALL_VIRTUAL_METHOD(FLOAT(__fastcall*)(PVOID, PVOID), (PUINT*)conVar, 12);
}

INT ConVar_getInt(ConVar* conVar)
{
    return CALL_VIRTUAL_METHOD(INT(__fastcall*)(PVOID, PVOID), (PUINT*)conVar, 13);
}

VOID ConVar_setValueString(ConVar* conVar, PCSTR str)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, PCSTR), (PUINT*)conVar, 14, str);
}

VOID ConVar_setValueFloat(ConVar* conVar, FLOAT _float)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, FLOAT), (PUINT*)conVar, 15, _float);
}

VOID ConVar_setValueInt(ConVar* conVar, INT _int)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, INT), (PUINT*)conVar, 16, _int);
}
