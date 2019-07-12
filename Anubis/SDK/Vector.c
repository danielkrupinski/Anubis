#include <math.h>
#include "Vector.h"

VOID Angles_normalize(Vector* vec)
{
    vec->x = isfinite(vec->x) ? remainderf(vec->x, 360.0f) : 0.0f;
    vec->y = isfinite(vec->y) ? remainderf(vec->y, 360.0f) : 0.0f;
    vec->z = 0.0f;
}
