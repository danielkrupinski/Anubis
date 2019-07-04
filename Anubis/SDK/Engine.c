#include "Engine.h"
#include "../Interfaces.h"

int Engine_getLocalPlayer(void)
{
    typedef int(__stdcall *getLocalPlayer)(void);
    __asm mov ecx, interfaces.engine
    return ((getLocalPlayer)interfaces.engine[0][12])();
}

bool Engine_isInGame(void)
{
    typedef bool(__stdcall *isInGame)(void);
    __asm mov ecx, interfaces.engine
    return ((isInGame)interfaces.engine[0][26])();
}
