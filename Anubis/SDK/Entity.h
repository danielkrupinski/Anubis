#pragma once

#include <stdbool.h>
#include <Windows.h>

typedef struct ClientClass ClientClass;
typedef struct Vector Vector;
typedef float Matrix3x4[3][4];

ClientClass* Entity_getClientClass(PVOID);
bool Entity_isDormant(PVOID);
bool Entity_setupBones(PVOID, Matrix3x4*, INT, INT, FLOAT);
bool Entity_isWeapon(PVOID);
VOID Entity_getEyePosition(PVOID, Vector*);
bool Entity_isEnemy(PVOID);
