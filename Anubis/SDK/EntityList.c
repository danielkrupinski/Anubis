#include "EntityList.h"
#include "../Interfaces.h"
#include "Utils.h"

PVOID EntityList_getEntity(INT index)
{
    return CALL_VIRTUAL_METHOD(PVOID(__fastcall*)(PVOID, PVOID, INT), interfaces.entityList, 3, index);
}
