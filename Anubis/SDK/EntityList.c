#include "EntityList.h"
#include "../Interfaces.h"

void* EntityList_getEntity(int index)
{
    typedef void*(__stdcall *getEntity)(int);
    __asm mov ecx, interfaces.engine
    return ((getEntity)((int*)*interfaces.entityList)[3])(index);
}
