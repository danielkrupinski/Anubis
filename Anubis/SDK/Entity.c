#include "ClassId.h"
#include "ClientClass.h"
#include "Engine.h"
#include "EngineTrace.h"
#include "Entity.h"
#include "EntityList.h"
#include "Matrix3x4.h"
#include "../Memory.h"
#include "Utils.h"
#include "Vector.h"

struct ClientClass* Entity_getClientClass(PVOID entity)
{
    return CALL_VIRTUAL_METHOD(struct ClientClass*(__fastcall*)(PVOID, PVOID), (PBYTE)entity + 8, 2);
}

bool Entity_isDormant(PVOID entity)
{
    return CALL_VIRTUAL_METHOD(bool(__fastcall*)(PVOID, PVOID), (PBYTE)entity + 8, 9);
}

bool Entity_setupBones(PVOID entity, struct Matrix3x4* out, INT maxBones, INT boneMask, FLOAT currentTime)
{
    return CALL_VIRTUAL_METHOD(bool(__fastcall*)(PVOID, PVOID, struct Matrix3x4*, INT, INT, FLOAT), (PBYTE)entity + 4, 13, out, maxBones, boneMask, currentTime);
}

bool Entity_isWeapon(PVOID entity)
{
    return CALL_VIRTUAL_METHOD(bool(__fastcall*)(PVOID, PVOID), entity, 165);
}

PVOID Entity_getActiveWeapon(PVOID entity)
{
    return CALL_VIRTUAL_METHOD(PVOID(__fastcall*)(PVOID, PVOID), entity, 267);
}

VOID Entity_getEyePosition(PVOID entity, Vector* out)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, Vector*), entity, 283, out);
}

struct WeaponData* Entity_getWeaponData(PVOID entity)
{
    return CALL_VIRTUAL_METHOD(struct WeaponData*(__fastcall*)(PVOID, PVOID), entity, 456);
}

bool Entity_isEnemy(PVOID entity)
{
    return memory.isOtherEnemy(entity, NULL, EntityList_getEntity(Engine_getLocalPlayer()));
}

Vector Entity_getBonePosition(PVOID entity, INT bone)
{
    static struct Matrix3x4 boneMatrices[128];

     if (Entity_setupBones(entity, boneMatrices, 128, 256, 0.0f)) {
         Vector result = { boneMatrices[bone].m[0][3], boneMatrices[bone].m[1][3], boneMatrices[bone].m[2][3] };
         return result;
     } else {
         Vector result = { 0.0f, 0.0f, 0.0f };
         return result;
     }
}

bool Entity_isVisible(PVOID entity, const Vector* position)
{
    PVOID localPlayer = EntityList_getEntity(Engine_getLocalPlayer());
    Ray ray;
    Entity_getEyePosition(localPlayer, &ray.start);
    const Vector endPosition = position ? *position : Entity_getBonePosition(entity, 8);

    ray.delta.x = endPosition.x - ray.start.x;
    ray.delta.y = endPosition.y - ray.start.y;
    ray.delta.z = endPosition.z - ray.start.z;
    if (endPosition.x || endPosition.y || endPosition.z)
        ray.isSwept = true;

    Trace trace;
    TraceFilter filter;
    TraceFilter_init(&filter);
    filter.skip = localPlayer;
    EngineTrace_traceRay(&ray, 0x46004009, &filter, &trace);
    return trace.entity == entity;
}

bool Entity_isSniperRifle(PVOID entity)
{
    switch (Entity_getClientClass(entity)->classId) {
        case ClassId_Ssg08:
        case ClassId_Awp:
        case ClassId_Scar20:
        case ClassId_G3sg1:
            return true;
        default:
            return false;
    }
}

NETVAR_OFFSET_IMPL(moveType, "CBaseEntity", "m_nRenderMode", 1, enum MoveType);

NETVAR_IMPL(nextAttack, "CBaseCombatCharacter", "m_flNextAttack", FLOAT);

NETVAR_IMPL(clip, "CBaseCombatWeapon", "m_iClip1", INT);
NETVAR_IMPL(nextPrimaryAttack, "CBaseCombatWeapon", "m_flNextPrimaryAttack", FLOAT);

NETVAR_IMPL(itemDefinitionIndex, "CBaseAttributableItem", "m_iItemDefinitionIndex", SHORT);

NETVAR_IMPL(flags, "CBasePlayer", "m_fFlags", INT);
NETVAR_IMPL(health, "CBasePlayer", "m_iHealth", INT);
NETVAR_IMPL(tickBase, "CBasePlayer", "m_nTickBase", INT);
NETVAR_IMPL(aimPunchAngle, "CBasePlayer", "m_aimPunchAngle", Vector);

NETVAR_IMPL(isDefusing, "CCSPlayer", "m_bIsDefusing", bool);
NETVAR_IMPL(gunGameImmunity, "CCSPlayer", "m_bGunGameImmunity", bool);
NETVAR_IMPL(flashDuration, "CCSPlayer", "m_flFlashDuration", FLOAT);
NETVAR_IMPL(isScoped, "CCSPlayer", "m_bIsScoped", bool);
NETVAR_IMPL(hasHelmet, "CCSPlayer", "m_bHasHelmet", BOOLEAN);
NETVAR_IMPL(armor, "CCSPlayer", "m_ArmorValue", INT);

NETVAR_IMPL(c4startedArming, "CC4", "m_bStartedArming", bool);
