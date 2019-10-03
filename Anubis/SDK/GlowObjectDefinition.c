#include "GlowObjectDefinition.h"

bool GlowObjectManager_hasGlowEffect(struct GlowObjectManager* glowObjectManager, PVOID entity)
{
    for (int i = 0; i < glowObjectManager->glowObjectDefinitions.size; i++) {
        struct GlowObjectDefinition* glowObject = (struct GlowObjectDefinition*)glowObjectManager->glowObjectDefinitions.memory + i;
        if (glowObject->nextFreeSlot == -2 && glowObject->entity == entity)
            return true;
    }
    return false;
}

INT GlowObjectManager_registerGlowObject(struct GlowObjectManager* glowObjectManager, PVOID entity)
{
    int index = glowObjectManager->firstFreeSlot;
    if (index != -1) {
        struct GlowObjectDefinition* glowObject = (struct GlowObjectDefinition*)glowObjectManager->glowObjectDefinitions.memory + index;
        glowObjectManager->firstFreeSlot = glowObject->nextFreeSlot;
        glowObject->entity = entity;
        glowObject->fullBloomRender = false;
        glowObject->fullBloomStencilTestValue = 0;
        glowObject->splitScreenSlot = -1;
        glowObject->nextFreeSlot = -2;
    }
    return index;
}

VOID GlowObjectManager_unregisterGlowObject(struct GlowObjectManager* glowObjectManager, INT index)
{
    struct GlowObjectDefinition* glowObject = (struct GlowObjectDefinition*)glowObjectManager->glowObjectDefinitions.memory + index;
    glowObject->nextFreeSlot = glowObjectManager->firstFreeSlot;
    glowObject->entity = NULL;
    glowObject->renderWhenOccluded = false;
    glowObject->renderWhenUnoccluded = false;
    glowObjectManager->firstFreeSlot = index;
}
