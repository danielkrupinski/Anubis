#include "Engine.h"
#include "Entity.h"
#include "EntityList.h"
#include "Utils.h"
#include "../Memory.h"

ClientClass* Entity_getClientClass(PVOID entity)
{
    CALL_VIRTUAL_METHOD_RETURN(ClientClass*(__fastcall*)(PVOID, PVOID), ((PUINT*)entity + 2), 2);
}

bool Entity_isDormant(PVOID entity)
{
    CALL_VIRTUAL_METHOD_RETURN(bool(__fastcall*)(PVOID, PVOID), ((PUINT*)entity + 2), 9);
}

bool Entity_isWeapon(PVOID entity)
{
    CALL_VIRTUAL_METHOD_RETURN(bool(__fastcall*)(PVOID, PVOID), ((PUINT*)entity), 163);
}

VOID Entity_getEyePosition(PVOID entity, Vector* out)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, Vector*), ((PUINT*)entity), 281, out);
}

bool Entity_isEnemy(PVOID entity)
{
    return memory.isOtherEnemy(entity, NULL, EntityList_getEntity(Engine_getLocalPlayer()));
}
