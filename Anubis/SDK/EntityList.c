#include "EntityList.h"
#include "../Interfaces.h"
#include "Utils.h"

PVOID EntityList_getEntity(INT index)
{
    return CALL_VIRTUAL_METHOD(PVOID(__fastcall*)(PVOID, PVOID, INT), Interfaces()->entityList, 3, index);
}

INT EntityList_getHighestEntityIndex(VOID)
{
    return CALL_VIRTUAL_METHOD(INT(__fastcall*)(PVOID, PVOID), Interfaces()->entityList, 6);
}
