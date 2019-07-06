#include "Engine.h"
#include "../Interfaces.h"
#include "Utils.h"

int Engine_getLocalPlayer(void)
{
    CALL_VIRTUAL_METHOD_RETURN(int(__fastcall*)(void*, void*), interfaces.engine, 12);
}

bool Engine_isInGame(void)
{
    CALL_VIRTUAL_METHOD_RETURN(bool(__fastcall*)(void*, void*), interfaces.engine, 26);
}
