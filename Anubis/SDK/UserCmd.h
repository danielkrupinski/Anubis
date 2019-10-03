#pragma once

#include <stdbool.h>
#include <Windows.h>

#include "Vector.h"

enum UserCmdButtons {
    IN_ATTACK = 1 << 0,
    IN_JUMP = 1 << 1,
    IN_DUCK = 1 << 2,
    IN_FORWARD = 1 << 3,
    IN_BACK = 1 << 4,
    IN_USE = 1 << 5,
    IN_MOVELEFT = 1 << 9,
    IN_MOVERIGHT = 1 << 10,
    IN_ATTACK2 = 1 << 11,
    IN_SCORE = 1 << 16,
    IN_BULLRUSH = 1 << 22
};

struct UserCmd {
    INT pad;
    INT commandNumber;
    INT tickCount;
    struct Vector viewangles;
    struct Vector aimdirection;
    FLOAT forwardmove;
    FLOAT sidemove;
    FLOAT upmove;
    enum UserCmdButtons buttons;
    BYTE impulse;
    INT weaponselect;
    INT weaponsubtype;
    INT randomSeed;
    SHORT mousedx;
    SHORT mousedy;
    bool hasbeenpredicted;
};
