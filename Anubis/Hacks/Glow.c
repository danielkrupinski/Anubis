#include "Glow.h"
#include "../Memory.h"
#include "../SDK/Entity.h"
#include "../SDK/GlowObjectDefinition.h"

VOID Glow_render(VOID)
{
    for (int i = 0; i < memory.glowObjectManager->glowObjectDefinitions.size; i++) {
        GlowObjectDefinition* glowObject = (GlowObjectDefinition*)memory.glowObjectManager->glowObjectDefinitions.memory + i;
        PVOID entity = glowObject->entity;

        if (glowObject->nextFreeSlot != -2 || !entity || Entity_isDormant(entity))
            continue;
        glowObject->renderWhenOccluded = true;
        glowObject->renderWhenUnoccluded = false;
        glowObject->alpha = 1.0f;
        glowObject->glowStyle = 0;
        glowObject->bloomAmount = 1.0f;
        glowObject->glowColor.x = 1.0f;
        glowObject->glowColor.y = 1.0f;
        glowObject->glowColor.z = 1.0f;
    }
}
