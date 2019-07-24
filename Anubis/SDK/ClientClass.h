#pragma once

#include <Windows.h>

typedef struct ClientClass {
    PVOID(*createFunction)(INT, INT);
    PVOID createEventFunction;
    PSTR networkName;
    PVOID recvTable;
    ClientClass* next;
    INT classId;
} ClientClass;
