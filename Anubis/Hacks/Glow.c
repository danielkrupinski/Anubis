#include <math.h>

#include "../Config.h"
#include "Glow.h"
#include "../Memory.h"

#include "../SDK/ClassId.h"
#include "../SDK/ClientClass.h"
#include "../SDK/Entity.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/GlowObjectDefinition.h"

static VOID applyGlow(GlowObjectDefinition* glowObject, GlowConfig* glowConfig, INT health)
{
    if (glowConfig->enabled) {
        glowObject->renderWhenOccluded = true;
        glowObject->alpha = glowConfig->alpha;
        glowObject->glowStyle = glowConfig->style;
        glowObject->bloomAmount = glowConfig->thickness;

        if (glowConfig->healthBased && health) {
            glowObject->glowColor.x = 1.0f - health / 100.0f;
            glowObject->glowColor.y = health / 100.0f;
            glowObject->glowColor.z = 0.0f;
        } else if (glowConfig->rainbow) {
            glowObject->glowColor.x = sinf(0.6f * memory.globalVars->currentTime) * 0.5f + 0.5f;
            glowObject->glowColor.y = sinf(0.6f * memory.globalVars->currentTime + 2.0f) * 0.5f + 0.5f;
            glowObject->glowColor.z = sinf(0.6f * memory.globalVars->currentTime + 4.0f) * 0.5f + 0.5f;
        } else {
            glowObject->glowColor.x = glowConfig->color[0];
            glowObject->glowColor.y = glowConfig->color[1];
            glowObject->glowColor.z = glowConfig->color[2];
        }
    }
}

VOID Glow_render(VOID)
{
    for (int i = 0; i < memory.glowObjectManager->glowObjectDefinitions.size; i++) {
        GlowObjectDefinition* glowObject = (GlowObjectDefinition*)memory.glowObjectManager->glowObjectDefinitions.memory + i;
        PVOID entity = glowObject->entity;

        if (glowObject->nextFreeSlot != -2 || !entity || Entity_isDormant(entity))
            continue;

        switch (Entity_getClientClass(entity)->classId) {
        case ClassId_CSPlayer:
            glowObject->renderWhenOccluded = true;
            glowObject->alpha = 1.0f;
            glowObject->glowStyle = 0;
            glowObject->bloomAmount = 1.0f;
            glowObject->glowColor.x = 1.0f;
            glowObject->glowColor.y = 1.0f;
            glowObject->glowColor.z = 1.0f;
            break;
        }
    }
}
