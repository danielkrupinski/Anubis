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

    const struct Vector points[] = {
        { min->x, min->y, min->z },
        { min->x, max->y, min->z },
        { max->x, max->y, min->z },
        { max->x, min->y, min->z },
        { min->x, min->y, max->z },
        { min->x, max->y, max->z },
        { max->x, max->y, max->z },
        { max->x, min->y, max->z }
    };

    INT width, height;
    Surface_getScreenSize(&width, &height);

    out->x0 = (FLOAT)(width * 2);
    out->y0 = (FLOAT)(height * 2);
    out->x1 = -(FLOAT)(width * 2);
    out->y1 = -(FLOAT)(height * 2);

    for (INT i = 0; i < 8; i++) {
        struct Vector screenPoint;
        struct Vector transformed = Vector_transform(points + i, Entity_coordinateFrame(entity));

        if (DebugOverlay_screenPosition(&transformed, &screenPoint))
            return FALSE;

        if (out->x0 > screenPoint.x)
            out->x0 = screenPoint.x;

        if (out->y0 > screenPoint.y)
            out->y0 = screenPoint.y;

        if (out->x1 < screenPoint.x)
            out->x1 = screenPoint.x;

        if (out->y1 < screenPoint.y)
            out->y1 = screenPoint.y;
    }
    return TRUE;
}

VOID Esp_render(VOID)
{

}
