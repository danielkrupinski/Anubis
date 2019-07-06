#include "EntityList.h"
#include "../Interfaces.h"
#include "Utils.h"

void* EntityList_getEntity(int index)
{
    CALL_VIRTUAL_METHOD_RETURN(void* (__fastcall*)(void*, void*, int), interfaces.entityList, 3, index);
}
