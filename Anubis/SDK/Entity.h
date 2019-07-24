#pragma once

#include <stdbool.h>
#include <Windows.h>

typedef struct ClientClass ClientClass;

ClientClass* Entity_getClientClass(PVOID);
bool Entity_isDormant(PVOID);
