#pragma once

#include <stdbool.h>
#include <Windows.h>

#include "UtlVector.h"
#include "Vector.h"

struct GlowObjectDefinition {
    int nextFreeSlot;
    PVOID entity;
    struct Vector glowColor;
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
};

struct GlowObjectManager {
    struct UtlVector glowObjectDefinitions;
    INT firstFreeSlot;
};

bool GlowObjectManager_hasGlowEffect(struct GlowObjectManager*, PVOID);
INT GlowObjectManager_registerGlowObject(struct GlowObjectManager*, PVOID);
VOID GlowObjectManager_unregisterGlowObject(struct GlowObjectManager*, INT);
