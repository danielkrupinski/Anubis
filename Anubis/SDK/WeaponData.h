#pragma once

#include <Windows.h>

struct WeaponData {
    BYTE pad[20];
    INT maxClip;
    BYTE pad1[112];
    PSTR name;
    BYTE pad1_[60];
    INT type;
    BYTE pad2[32];
    BOOLEAN fullAuto;
    BYTE pad3[3];
    INT damage;
    FLOAT armorRatio;
    INT bullets;
    FLOAT penetration;
    BYTE pad4[8];
    FLOAT range;
    FLOAT rangeModifier;
    BYTE pad5[16];
    BOOLEAN hasSilencer;
};
