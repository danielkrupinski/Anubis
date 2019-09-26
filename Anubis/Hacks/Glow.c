#include <math.h>

#include "../Config.h"
#include "Glow.h"
#include "../Memory.h"

#include "../SDK/ClassId.h"
#include "../SDK/ClientClass.h"
#include "../SDK/Engine.h"
#include "../SDK/Entity.h"
#include "../SDK/EntityList.h"
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

static VOID applyPlayerGlow(GlowObjectDefinition* glowObject, GlowConfig* glowConfigAll, GlowConfig* glowConfigVisible, GlowConfig* glowConfigOccluded, PVOID entity)
{
    Vector localPlayerEyePosition;
    Entity_getEyePosition(EntityList_getEntity(Engine_getLocalPlayer()), &localPlayerEyePosition);
    if (glowConfigAll->enabled) applyGlow(glowObject, glowConfigAll, *Entity_health(entity));
    else if ((glowConfigVisible->style || Entity_isVisible(entity, NULL)) && !memory.lineGoesThroughSmoke(localPlayerEyePosition, Entity_getBonePosition(entity, 8), 1)) applyGlow(glowObject, glowConfigVisible, *Entity_health(entity));
    else applyGlow(glowObject, glowConfigOccluded, *Entity_health(entity));

}

VOID Glow_render(VOID)
{
    for (int i = 0; i < memory.glowObjectManager->glowObjectDefinitions.size; i++) {
        GlowObjectDefinition* glowObject = (GlowObjectDefinition*)memory.glowObjectManager->glowObjectDefinitions.memory + i;
        PVOID entity = glowObject->entity;

        if (glowObject->nextFreeSlot != -2 || !entity || Entity_isDormant(entity))
            continue;

        switch (Entity_getClientClass(entity)->classId) {
        case ClassId_CSPlayer: {
            PVOID activeWeapon = Entity_getActiveWeapon(entity);
            if (activeWeapon && Entity_getClientClass(entity)->classId == ClassId_C4 && *Entity_c4startedArming(activeWeapon))
                applyPlayerGlow(glowObject, config.glow + 6, config.glow + 7, config.glow + 8, entity);
            else if (*Entity_isDefusing(entity))
                applyPlayerGlow(glowObject, config.glow + 9, config.glow + 10, config.glow + 11, entity);
            else if (entity == EntityList_getEntity(Engine_getLocalPlayer()))
                applyGlow(glowObject, config.glow + 12, *Entity_health(entity));
            else if (Entity_isEnemy(entity))
                applyPlayerGlow(glowObject, config.glow + 3, config.glow + 4, config.glow + 5, entity);
            else
                applyPlayerGlow(glowObject, config.glow + 0, config.glow + 1, config.glow + 2, entity);
            break;
        }
        case ClassId_C4:
            applyGlow(glowObject, config.glow + 14, 0);
            break;
        case ClassId_PlantedC4:
            applyGlow(glowObject, config.glow + 15, 0);
            break;
        case ClassId_Chicken:
            applyGlow(glowObject, config.glow + 16, 0);
            break;
        default:
            if (Entity_isWeapon(entity)) {
                applyGlow(glowObject, config.glow + 13, 0);
                if (!config.glow[13].enabled) glowObject->renderWhenOccluded = false;
            }
        }
    }
}
