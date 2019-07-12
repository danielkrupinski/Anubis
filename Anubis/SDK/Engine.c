#include "Engine.h"
#include "../Interfaces.h"
#include "Utils.h"

INT Engine_getLocalPlayer(VOID)
{
    CALL_VIRTUAL_METHOD_RETURN(INT(__fastcall*)(PVOID, PVOID), interfaces.engine, 12);
}

bool Engine_isInGame(VOID)
{
    CALL_VIRTUAL_METHOD_RETURN(bool(__fastcall*)(PVOID, PVOID), interfaces.engine, 26);
}
