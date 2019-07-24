#include "Entity.h"
#include "Utils.h"

bool Entity_isDormant(PVOID entity)
{
    CALL_VIRTUAL_METHOD_RETURN(bool(__fastcall*)(PVOID, PVOID), ((PUINT*)entity + 2), 9);
}
