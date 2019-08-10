#include "../Config.h"
#include "../Memory.h"

#include "../SDK/ConVar.h"
#include "../SDK/Cvar.h"
#include "../SDK/Engine.h"
#include "../SDK/Entity.h"
#include "../SDK/EntityList.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/ItemDefinitionIndex.h"
#include "../SDK/UserCmd.h"
#include "../SDK/Utils.h"
#include "../SDK/Vector.h"

#include "Triggerbot.h"

#define MAX_RANGE 8192.0f

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

            UTILS_STATIC_VAR(ConVar*, weaponRecoilScale, Cvar_findVar("weapon_recoil_scale"));

            Vector aimPunch = *Entity_aimPunchAngle(localPlayer);
            aimPunch.x *= ConVar_getFloat(weaponRecoilScale);
            aimPunch.y *= ConVar_getFloat(weaponRecoilScale);
            aimPunch.z *= ConVar_getFloat(weaponRecoilScale);

            Vector viewAngles = { cosf(DEG2RAD(cmd->viewangles.x + aimPunch.x)) * cosf(DEG2RAD(cmd->viewangles.y + aimPunch.y)) * MAX_RANGE,
                                  cosf(DEG2RAD(cmd->viewangles.x + aimPunch.x)) * sinf(DEG2RAD(cmd->viewangles.y + aimPunch.y)) * MAX_RANGE,
                                 -sinf(DEG2RAD(cmd->viewangles.x + aimPunch.x)) * MAX_RANGE };
        }
    }
}
