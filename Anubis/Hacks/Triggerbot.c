#include "../Config.h"
#include "../Memory.h"

#include "../SDK/ClassId.h"
#include "../SDK/ClientClass.h"
#include "../SDK/ConVar.h"
#include "../SDK/Cvar.h"
#include "../SDK/Engine.h"
#include "../SDK/EngineTrace.h"
#include "../SDK/Entity.h"
#include "../SDK/EntityList.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/ItemDefinitionIndex.h"
#include "../SDK/UserCmd.h"
#include "../SDK/Utils.h"
#include "../SDK/Vector.h"
#include "../SDK/WeaponData.h"

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

            UTILS_STATIC_VAR(ConVar*, weaponRecoilScale, Cvar_findVar("weapon_recoil_scale"));

            Vector aimPunch = *Entity_aimPunchAngle(localPlayer);
            aimPunch.x *= ConVar_getFloat(weaponRecoilScale);
            aimPunch.y *= ConVar_getFloat(weaponRecoilScale);
            aimPunch.z *= ConVar_getFloat(weaponRecoilScale);

            FLOAT maxRange = Entity_getWeaponData(activeWeapon)->range;

            Vector viewAngles = { cosf(DEG2RAD(cmd->viewangles.x + aimPunch.x)) * cosf(DEG2RAD(cmd->viewangles.y + aimPunch.y)) * maxRange,
                                  cosf(DEG2RAD(cmd->viewangles.x + aimPunch.x)) * sinf(DEG2RAD(cmd->viewangles.y + aimPunch.y)) * maxRange,
                                 -sinf(DEG2RAD(cmd->viewangles.x + aimPunch.x)) * maxRange };

            Ray ray;
            ray.isRay = true;
            Entity_getEyePosition(localPlayer, &ray.start);
            ray.delta = viewAngles;
            ray.isSwept = ray.delta.x || ray.delta.y || ray.delta.z;

            Trace trace;
            TraceFilter filter;
            TraceFilter_init(&filter);
            filter.skip = localPlayer;
            EngineTrace_traceRay(&ray, 0x46004009, &filter, &trace);

            if (trace.entity && Entity_getClientClass(trace.entity)->classId == ClassId_CSPlayer
                && (config.triggerbot[weaponIndex].friendlyFire
                    || Entity_isEnemy(trace.entity))
                && !*Entity_gunGameImmunity(trace.entity)
                && (!config.triggerbot[weaponIndex].hitgroup
                    || trace.hitgroup == config.triggerbot[weaponIndex].hitgroup)
                && (config.triggerbot[weaponIndex].ignoreSmoke
                    || !memory.lineGoesThroughSmoke(ray.start, Vector_add(&ray.start, &viewAngles), 1))
                && (config.triggerbot[weaponIndex].ignoreFlash
                    || !*Entity_flashDuration(localPlayer))
                && (!config.triggerbot[weaponIndex].scopedOnly
                    || !Entity_isSniperRifle(activeWeapon)
                    || *Entity_isScoped(localPlayer))) {
                cmd->buttons |= IN_ATTACK;
                lastTime = 0.0f;
            } else {
                lastTime = now;
            }
        }
    }
}
