#include "Engine.h"
#include "../Interfaces.h"

int Engine_getLocalPlayer(void)
{
    typedef int(__stdcall *getLocalPlayer)(void);
    __asm mov ecx, interfaces.engine
    return ((getLocalPlayer)((int*)*interfaces.engine)[12])();
}

bool Engine_isInGame(void)
{
    typedef bool(__stdcall *isInGame)(void);
    __asm mov ecx, interfaces.engine
    return ((isInGame)((int*)*interfaces.engine)[26])();
}
