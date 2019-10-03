#pragma once

#include <stdbool.h>
#include <Windows.h>

#include "../Netvars.h"

struct ClientClass;
struct Vector;
struct WeaponData;
struct Matrix3x4;

struct ClientClass* Entity_getClientClass(PVOID);
bool Entity_isDormant(PVOID);
bool Entity_setupBones(PVOID, struct Matrix3x4*, INT, INT, FLOAT);
bool Entity_isWeapon(PVOID);
PVOID Entity_getActiveWeapon(PVOID);
VOID Entity_getEyePosition(PVOID, struct Vector*);
struct WeaponData* Entity_getWeaponData(PVOID);
bool Entity_isEnemy(PVOID);
struct Vector Entity_getBonePosition(PVOID, INT);
bool Entity_isVisible(PVOID, const struct Vector*);
bool Entity_isSniperRifle(PVOID);

enum MoveType {
    MoveType_Noclip = 8,
    MoveType_Ladder = 9
} MoveType;

NETVAR_OFFSET(moveType, "CBaseEntity", "m_nRenderMode", 1, enum MoveType);

NETVAR(nextAttack, "CBaseCombatCharacter", "m_flNextAttack", FLOAT);

NETVAR(clip, "CBaseCombatWeapon", "m_iClip1", INT);
NETVAR(nextPrimaryAttack, "CBaseCombatWeapon", "m_flNextPrimaryAttack", FLOAT);

NETVAR(itemDefinitionIndex, "CBaseAttributableItem", "m_iItemDefinitionIndex", SHORT);

NETVAR(flags, "CBasePlayer", "m_fFlags", INT);
NETVAR(health, "CBasePlayer", "m_iHealth", INT);
NETVAR(tickBase, "CBasePlayer", "m_nTickBase", INT);
NETVAR(aimPunchAngle, "CBasePlayer", "m_aimPunchAngle", struct Vector);

NETVAR(isDefusing, "CCSPlayer", "m_bIsDefusing", BOOLEAN);
NETVAR(gunGameImmunity, "CCSPlayer", "m_bGunGameImmunity", BOOLEAN);
NETVAR(flashDuration, "CCSPlayer", "m_flFlashDuration", FLOAT);
NETVAR(isScoped, "CCSPlayer", "m_bIsScoped", BOOLEAN);
NETVAR(hasHelmet, "CCSPlayer", "m_bHasHelmet", BOOLEAN);
NETVAR(armor, "CCSPlayer", "m_ArmorValue", INT);

NETVAR(c4startedArming, "CC4", "m_bStartedArming", BOOLEAN);
