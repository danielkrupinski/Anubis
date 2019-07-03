#pragma once

#include <stdbool.h>

#include "Vector.h"

enum UserCmdButtons {
    IN_ATTACK = 1 << 0,
    IN_JUMP = 1 << 1,
    IN_ATTACK2 = 1 << 11,
    IN_SCORE = 1 << 16,
    IN_BULLRUSH = 1 << 22
};

typedef struct {
    int pad;
    int commandNumber;
    int tickCount;
    Vector viewangles;
    Vector aimdirection;
    float forwardmove;
    float sidemove;
    float upmove;
    int buttons;
    char impulse;
    int weaponselect;
    int weaponsubtype;
    int randomSeed;
    short mousedx;
    short mousedy;
    bool hasbeenpredicted;
} UserCmd;
