#pragma once

#include <Windows.h>

#include "Vector.h"

struct GlowObjectDefinition {
    PVOID entity;
    Vector glowColor;
    float   alpha;
    BYTE pad[4];
    float   m_flSomeFloat;
    float   bloomAmount;
    float   m_flAnotherFloat;
    bool    renderWhenOccluded;
    bool    renderWhenUnoccluded;
    bool    fullBloomRender;
    BYTE pad1;
    int fullBloomStencilTestValue;
    int glowStyle;
    int splitScreenSlot;
    int nextFreeSlot;
};
