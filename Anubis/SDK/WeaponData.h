#pragma once

#include <Windows.h>

typedef struct WeaponData {
    BYTE pad[20];
    int maxClip;
    BYTE pad1[176];
    int type;
    BYTE pad2[32];
    bool fullAuto;
    BYTE pad3[3];
    int damage;
    float armorRatio;
    int bullets;
    float penetration;
    BYTE pad4[8];
    float range;
    float rangeModifier;
    BYTE pad5[16];
    bool hasSilencer;
} WeaponData;
