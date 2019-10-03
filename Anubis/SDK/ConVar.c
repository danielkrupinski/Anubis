#include "ConVar.h"
#include "Utils.h"

FLOAT ConVar_getFloat(struct ConVar* conVar)
{
    return CALL_VIRTUAL_METHOD(FLOAT(__fastcall*)(PVOID, PVOID), conVar, 12);
}

INT ConVar_getInt(struct ConVar* conVar)
{
    return CALL_VIRTUAL_METHOD(INT(__fastcall*)(PVOID, PVOID), conVar, 13);
}

VOID ConVar_setValueString(struct ConVar* conVar, PCSTR str)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, PCSTR), conVar, 14, str);
}

VOID ConVar_setValueFloat(struct ConVar* conVar, FLOAT _float)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, FLOAT), conVar, 15, _float);
}

VOID ConVar_setValueInt(struct ConVar* conVar, INT _int)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, INT), conVar, 16, _int);
}
