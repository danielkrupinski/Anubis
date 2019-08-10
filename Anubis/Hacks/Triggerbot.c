#include "../Config.h"
#include "../Memory.h"

#include "../SDK/Engine.h"
#include "../SDK/Entity.h"
#include "../SDK/EntityList.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/ItemDefinitionIndex.h"

#include "Triggerbot.h"

VOID Triggerbot_run(UserCmd* cmd)
{
    PVOID localPlayer = EntityList_getEntity(Engine_getLocalPlayer());
    if (*Entity_nextAttack(localPlayer) > GlobalVars_serverTime(NULL))
        return;

    PVOID activeWeapon = Entity_getActiveWeapon(localPlayer);
    if (!activeWeapon || !*Entity_clip(activeWeapon) || *Entity_nextPrimaryAttack(activeWeapon) > GlobalVars_serverTime(NULL))
        return;

    INT weaponIndex = getWeaponIndex(*Entity_itemDefinitionIndex(activeWeapon));
    if (!weaponIndex)
        return;

    if (!config.triggerbot[weaponIndex].enabled)
        weaponIndex = 0;

    static FLOAT lastTime = 0.0f;

    if (config.triggerbot[weaponIndex].enabled) {
        const FLOAT now = memory.globalVars->realTime;

        if ((GetAsyncKeyState(config.triggerbot[weaponIndex].key) || !config.triggerbot[weaponIndex].onKey)
            && now - lastTime >= config.triggerbot[weaponIndex].shotDelay / 1000.0f) {
        }
    }
}
