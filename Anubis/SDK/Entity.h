#pragma once

#include <stdbool.h>
#include <Windows.h>

#include "../Netvars.h"

typedef struct ClientClass ClientClass;
typedef struct Vector Vector;
typedef float Matrix3x4[3][4];

ClientClass* Entity_getClientClass(PVOID);
bool Entity_isDormant(PVOID);
bool Entity_setupBones(PVOID, Matrix3x4*, INT, INT, FLOAT);
bool Entity_isWeapon(PVOID);
PVOID Entity_getActiveWeapon(PVOID);
VOID Entity_getEyePosition(PVOID, Vector*);
bool Entity_isEnemy(PVOID);
Vector Entity_getBonePosition(PVOID, INT);
bool Entity_isVisible(PVOID, const Vector*);

NETVAR(flags, "CBasePlayer", "m_fFlags", INT);
NETVAR(health, "CBasePlayer", "m_iHealth", INT);

NETVAR(isDefusing, "CCSPlayer", "m_bIsDefusing", bool);

NETVAR(c4startedArming, "CC4", "m_bStartedArming", bool);
