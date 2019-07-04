#include "EntityList.h"
#include "../Interfaces.h"

void* EntityList_getEntity(int index)
{
    typedef void*(__stdcall *getEntity)(int);
    __asm mov ecx, interfaces.engine
    return ((getEntity)interfaces.entityList[0][3])(index);
}
