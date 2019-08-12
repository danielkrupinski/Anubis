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
