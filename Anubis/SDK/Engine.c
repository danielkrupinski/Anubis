#include "Engine.h"
#include "../Interfaces.h"
#include "Utils.h"

INT Engine_getLocalPlayer(VOID)
{
    return CALL_VIRTUAL_METHOD(INT(__fastcall*)(PVOID, PVOID), interfaces.engine, 12);
}

INT Engine_getMaxClients(VOID)
{
    return CALL_VIRTUAL_METHOD(INT(__fastcall*)(PVOID, PVOID), interfaces.engine, 20);
}

bool Engine_isInGame(VOID)
{
    return CALL_VIRTUAL_METHOD(bool(__fastcall*)(PVOID, PVOID), interfaces.engine, 26);
}
