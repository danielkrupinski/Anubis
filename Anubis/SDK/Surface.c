#include "../Interfaces.h"
#include "Surface.h"
#include "Utils.h"

VOID Surface_unlockCursor(VOID)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID), interfaces.surface, 66);
}
