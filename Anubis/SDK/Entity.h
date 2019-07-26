#pragma once

#include <stdbool.h>
#include <Windows.h>

typedef struct ClientClass ClientClass;
typedef struct Vector Vector;

ClientClass* Entity_getClientClass(PVOID);
bool Entity_isDormant(PVOID);
bool Entity_isWeapon(PVOID);
VOID Entity_getEyePosition(PVOID, Vector*);
bool Entity_isEnemy(PVOID);
