#include "Engine.h"
#include "../Interfaces.h"

int Engine_getLocalPlayer(void)
{
    typedef int(__stdcall *getLocalPlayer)(void);
    __asm mov ecx, interfaces.gameUI
    return ((getLocalPlayer)((int*)*interfaces.engine)[12])();
}
