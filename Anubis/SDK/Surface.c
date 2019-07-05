#include "../Interfaces.h"
#include "Surface.h"

void Surface_unlockCursor(void)
{
    typedef void(__stdcall* unlockCursor)();
    __asm mov ecx, interfaces.surface
    ((unlockCursor)interfaces.surface[0][66])();
}
