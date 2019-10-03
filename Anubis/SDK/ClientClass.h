#pragma once

#include <Windows.h>

typedef enum ClassId ClassId;

struct ClientClass {
    PVOID(*createFunction)(INT, INT);
    PVOID createEventFunction;
    PSTR networkName;
    PVOID recvTable;
    struct ClientClass* next;
    ClassId classId;
};
