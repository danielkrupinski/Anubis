#include "../Interfaces.h"
#include "Surface.h"
#include "Utils.h"

void Surface_unlockCursor(void)
{
    CALL_VIRTUAL_METHOD(void(__fastcall*)(void*, void*), interfaces.surface, 66);
}
