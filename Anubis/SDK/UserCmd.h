#pragma once

#include <stdbool.h>
#include <Windows.h>

#include "Vector.h"

enum UserCmdButtons {
    IN_ATTACK = 1 << 0,
    IN_JUMP = 1 << 1,
    IN_ATTACK2 = 1 << 11,
    IN_SCORE = 1 << 16,
    IN_BULLRUSH = 1 << 22
};

typedef struct {
    INT pad;
    INT commandNumber;
    INT tickCount;
    Vector viewangles;
    Vector aimdirection;
    FLOAT forwardmove;
    FLOAT sidemove;
    FLOAT upmove;
    INT buttons;
    BYTE impulse;
    INT weaponselect;
    INT weaponsubtype;
    INT randomSeed;
    SHORT mousedx;
    SHORT mousedy;
    bool hasbeenpredicted;
} UserCmd;
