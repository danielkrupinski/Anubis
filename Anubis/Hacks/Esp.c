#include "../Config.h"
#include "Esp.h"
#include "../SDK/DebugOverlay.h"
#include "../SDK/Engine.h"
#include "../SDK/Entity.h"
#include "../SDK/EntityList.h"
#include "../SDK/Surface.h"
#include "../SDK/Vector.h"

struct BoundingBox {
    FLOAT x0, y0;
    FLOAT x1, y1;
};

static BOOLEAN boundingBox(PVOID entity, struct BoundingBox* out)
{
    CONST struct Vector* min = Entity_obbMins(entity);
    CONST struct Vector* max = Entity_obbMaxs(entity);

    INT width, height;
    Surface_getScreenSize(&width, &height);

    out->x0 = (FLOAT)(width * 2);
    out->y0 = (FLOAT)(height * 2);
    out->x1 = -(FLOAT)(width * 2);
    out->y1 = -(FLOAT)(height * 2);

    for (INT i = 0; i < 8; i++) {
        CONST struct Vector vertex = { i & 1 ? max->x : min->x,
                                       i & 2 ? max->y : min->y,
                                       i & 4 ? max->z : min->z };

        struct Vector point;
        struct Vector transformed = Vector_transform(&vertex, Entity_coordinateFrame(entity));

        if (DebugOverlay_screenPosition(&transformed, &point))
            return FALSE;

        if (out->x0 > point.x)
            out->x0 = point.x;

        if (out->y0 > point.y)
            out->y0 = point.y;

        if (out->x1 < point.x)
            out->x1 = point.x;

        if (out->y1 < point.y)
            out->y1 = point.y;
    }
    return TRUE;
}

static VOID renderEspforWeapon(PVOID entity)
{
    if (config.esp.weapon.enabled) {
        struct BoundingBox bbox;
        if (boundingBox(entity, &bbox)) {
            if (config.esp.weapon.box) {
                Surface_setDrawColor2(config.esp.weapon.boxColor, 255);
                Surface_drawOutlinedRect(bbox.x0, bbox.y0, bbox.x1, bbox.y1);
            }
        }
    }
}

VOID Esp_render(VOID)
{
    if (Engine_isInGame()) {
        PVOID localPlayer = EntityList_getEntity(Engine_getLocalPlayer());
        for (INT i = 1; i <= Engine_getMaxClients(); i++) {
            PVOID entity = EntityList_getEntity(i);

            if (!entity || entity == localPlayer || Entity_isDormant(entity) || !Entity_isAlive(entity))
                continue;

        }

        for (INT i = Engine_getMaxClients() + 1; i <= EntityList_getHighestEntityIndex(); i++) {
            PVOID entity = EntityList_getEntity(i);

            if (!entity || Entity_isDormant(entity) || !Entity_isWeapon(entity) || *Entity_ownerEntity(entity) != -1)
                continue;

            renderEspforWeapon(entity);
        }

    }
}
