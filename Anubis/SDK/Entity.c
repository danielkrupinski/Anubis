#include "Engine.h"
#include "Entity.h"
#include "EntityList.h"
#include "../Memory.h"
#include "Utils.h"
#include "Vector.h"

ClientClass* Entity_getClientClass(PVOID entity)
{
    CALL_VIRTUAL_METHOD_RETURN(ClientClass*(__fastcall*)(PVOID, PVOID), (PUINT*)entity + 2, 2);
}

bool Entity_isDormant(PVOID entity)
{
    CALL_VIRTUAL_METHOD_RETURN(bool(__fastcall*)(PVOID, PVOID), (PUINT*)entity + 2, 9);
}

bool Entity_setupBones(PVOID entity, Matrix3x4* out, INT maxBones, INT boneMask, FLOAT currentTime)
{
    CALL_VIRTUAL_METHOD_RETURN(bool(__fastcall*)(PVOID, PVOID, Matrix3x4*, INT, INT, FLOAT), (PUINT*)entity + 1, 13, out, maxBones, boneMask, currentTime);
}

bool Entity_isWeapon(PVOID entity)
{
    CALL_VIRTUAL_METHOD_RETURN(bool(__fastcall*)(PVOID, PVOID), (PUINT*)entity, 163);
}

PVOID Entity_getActiveWeapon(PVOID entity)
{
    CALL_VIRTUAL_METHOD_RETURN(PVOID(__fastcall*)(PVOID), (PUINT*)entity, 265);
}

VOID Entity_getEyePosition(PVOID entity, Vector* out)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, Vector*), (PUINT*)entity, 281, out);
}

bool Entity_isEnemy(PVOID entity)
{
    return memory.isOtherEnemy(entity, NULL, EntityList_getEntity(Engine_getLocalPlayer()));
}

Vector Entity_getBonePosition(PVOID entity, INT bone)
{
    static Matrix3x4 boneMatrices[128];

     if (Entity_setupBones(entity, boneMatrices, 128, 256, 0.0f)) {
         Vector result = { boneMatrices[bone][0][3], boneMatrices[bone][1][3], boneMatrices[bone][2][3] };
         return result;
     } else {
         Vector result = { 0.0f, 0.0f, 0.0f };
         return result;
     }
}

NETVAR_IMPL(flags, "CBasePlayer", "m_fFlags", INT);
NETVAR_IMPL(health, "CBasePlayer", "m_iHealth", INT);
