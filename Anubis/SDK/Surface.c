#include "../Interfaces.h"
#include "Surface.h"
#include "Utils.h"

VOID Surface_setDrawColor(INT r, INT g, INT b, INT a)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, INT, INT, INT, INT), interfaces.surface, 15, r, g, b, a);
}

VOID Surface_setDrawColor2(FLOAT color[3], INT a)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, INT, INT, INT, INT), interfaces.surface, 15, (INT)(color[0] * 255), (INT)(color[1] * 255), (INT)(color[2] * 255), a);
}

VOID Surface_drawOutlinedRect(FLOAT x0, FLOAT y0, FLOAT x1, FLOAT y1)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, INT, INT, INT, INT), interfaces.surface, 18, (INT)x0, (INT)y0, (INT)x1, (INT)y1);
}

VOID Surface_drawLine(FLOAT x0, FLOAT y0, FLOAT x1, FLOAT y1)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, INT, INT, INT, INT), interfaces.surface, 19, (INT)x0, (INT)y0, (INT)x1, (INT)y1);
}

VOID Surface_setTextFont(UINT font)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, UINT), interfaces.surface, 23, font);
}

VOID Surface_setTextColor(FLOAT color[3], INT a)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, INT, INT, INT, INT), interfaces.surface, 25, (INT)(color[0] * 255), (INT)(color[1] * 255), (INT)(color[2] * 255), a);
}

VOID Surface_setTextPosition(FLOAT x, FLOAT y)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, INT, INT), interfaces.surface, 26, (INT)x, (INT)y);
}

VOID Surface_printText(PCWSTR text)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, PCWSTR, INT, INT), interfaces.surface, 28, text, wcslen(text), 0);
}

VOID Surface_getScreenSize(INT* width, INT* height)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, INT*, INT*), interfaces.surface, 44, width, height);
}

VOID Surface_unlockCursor(VOID)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID), interfaces.surface, 66);
}

VOID Surface_getTextSize(UINT font, PCWSTR text, INT* width, INT* height)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, UINT, PCWSTR, INT*, INT*), interfaces.surface, 79, font, text, width, height);
}
