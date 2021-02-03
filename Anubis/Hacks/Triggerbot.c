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
        const FLOAT now = Memory()->globalVars->realTime;

        if ((GetAsyncKeyState(config.triggerbot[weaponIndex].key) || !config.triggerbot[weaponIndex].onKey)
            && now - lastTime >= config.triggerbot[weaponIndex].shotDelay / 1000.0f) {

            UTILS_STATIC_VAR(ConVar*, weaponRecoilScale, Cvar_findVar("weapon_recoil_scale"));
            const float recoilScale = ConVar_getFloat(weaponRecoilScale);

            Vector aimPunch = *Entity_aimPunchAngle(localPlayer);
            aimPunch.x *= recoilScale;
            aimPunch.y *= recoilScale;
            aimPunch.z *= recoilScale;

            CONST struct WeaponData* weaponData = Entity_getWeaponData(activeWeapon);

            Vector viewAngles = { cosf(DEG2RAD(cmd->viewangles.x + aimPunch.x)) * cosf(DEG2RAD(cmd->viewangles.y + aimPunch.y)) * weaponData->range,
                                  cosf(DEG2RAD(cmd->viewangles.x + aimPunch.x)) * sinf(DEG2RAD(cmd->viewangles.y + aimPunch.y)) * weaponData->range,
                                 -sinf(DEG2RAD(cmd->viewangles.x + aimPunch.x)) * weaponData->range };

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
                    || !Memory()->lineGoesThroughSmoke(ray.start, Vector_add(&ray.start, &viewAngles), 1))
                && (config.triggerbot[weaponIndex].ignoreFlash
                    || !*Entity_flashDuration(localPlayer))
                && (!config.triggerbot[weaponIndex].scopedOnly
                    || !Entity_isSniperRifle(activeWeapon)
                    || *Entity_isScoped(localPlayer))) {

                FLOAT damage = (*Entity_itemDefinitionIndex(activeWeapon) != ItemDefinitionIndex_Taser ? HitGroup_getDamageMultiplier(trace.hitgroup) : 1.0f) * weaponData->damage * powf(weaponData->rangeModifier, trace.fraction * weaponData->range / 500.0f);
                FLOAT armorRatio = weaponData->armorRatio / 2.0f;
                if (*Entity_itemDefinitionIndex(activeWeapon) != ItemDefinitionIndex_Taser && HitGroup_isArmored(trace.hitgroup, *Entity_hasHelmet(trace.entity)))
                    damage -= (*Entity_armor(trace.entity) < damage * armorRatio / 2.0f ? *Entity_armor(trace.entity) * 4.0f : damage) * (1.0f - armorRatio);

                if (damage >= (config.triggerbot[weaponIndex].killshot ? *Entity_health(trace.entity) : config.triggerbot[weaponIndex].minDamage)) {
                    cmd->buttons |= IN_ATTACK;
                    lastTime = 0.0f;
                }
            } else {
                lastTime = now;
            }
        }
    }
}
