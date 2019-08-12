#include "GlowObjectDefinition.h"

bool GlowObjectManager_hasGlowEffect(GlowObjectManager* glowObjectManager, PVOID entity)
{
    for (int i = 0; i < glowObjectManager->glowObjectDefinitions.size; i++) {
        GlowObjectDefinition* glowObject = (GlowObjectDefinition*)glowObjectManager->glowObjectDefinitions.memory + i;
        if (glowObject->nextFreeSlot == -2 && glowObject->entity == entity)
            return true;
    }
    return false;
}

INT GlowObjectManager_registerGlowObject(GlowObjectManager* glowObjectManager, PVOID entity)
{
    int index = glowObjectManager->firstFreeSlot;
    if (index != -1) {
        GlowObjectDefinition* glowObject = (GlowObjectDefinition*)glowObjectManager->glowObjectDefinitions.memory + index;
        glowObjectManager->firstFreeSlot = glowObject->nextFreeSlot;
        glowObject->entity = entity;
        glowObject->fullBloomRender = false;
        glowObject->fullBloomStencilTestValue = 0;
        glowObject->splitScreenSlot = -1;
        glowObject->nextFreeSlot = -2;
    }
    return index;
}

VOID GlowObjectManager_unregisterGlowObject(GlowObjectManager* glowObjectManager, INT index)
{
    GlowObjectDefinition* glowObject = (GlowObjectDefinition*)glowObjectManager->glowObjectDefinitions.memory + index;
    glowObject->nextFreeSlot = glowObjectManager->firstFreeSlot;
    glowObject->entity = NULL;
    glowObject->renderWhenOccluded = false;
    glowObject->renderWhenUnoccluded = false;
    glowObjectManager->firstFreeSlot = index;
}
