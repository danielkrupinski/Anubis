#pragma once

#include <Windows.h>

typedef enum ClassId ClassId;

typedef struct ClientClass {
    PVOID(*createFunction)(INT, INT);
    PVOID createEventFunction;
    PSTR networkName;
    PVOID recvTable;
    ClientClass* next;
    ClassId classId;
} ClientClass;
