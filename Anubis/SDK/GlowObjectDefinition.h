#pragma once

#include <stdbool.h>
#include <Windows.h>

#include "UtlVector.h"
#include "Vector.h"

typedef struct GlowObjectDefinition {
    PVOID entity;
    Vector glowColor;
    float   alpha;
    BYTE pad[4];
    float m_flSomeFloat;
    float bloomAmount;
    float m_flAnotherFloat;
    bool renderWhenOccluded;
    bool renderWhenUnoccluded;
    bool fullBloomRender;
    BYTE pad1;
    int fullBloomStencilTestValue;
    int glowStyle;
    int splitScreenSlot;
    int nextFreeSlot;
} GlowObjectDefinition;

typedef struct GlowObjectManager {
    UtlVector glowObjectDefinitions;
    INT firstFreeSlot;
} GlowObjectManager;

bool GlowObjectManager_hasGlowEffect(GlowObjectManager*, PVOID);
