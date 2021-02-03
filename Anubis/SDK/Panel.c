#include "../Interfaces.h"
#include "Panel.h"

PCSTR Panel_getName(UINT panel)
{
    return CALL_VIRTUAL_METHOD(PCSTR(__fastcall*)(PVOID, PVOID, UINT), Interfaces()->panel, 36, panel);
}
