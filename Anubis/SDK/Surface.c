#include "../Interfaces.h"
#include "Surface.h"
#include "Utils.h"

VOID Surface_setDrawColor(INT r, INT g, INT b, INT a)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, INT, INT, INT, INT), interfaces.surface, 15, r, g, b, a);
}

VOID Surface_unlockCursor(VOID)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID), interfaces.surface, 66);
}
