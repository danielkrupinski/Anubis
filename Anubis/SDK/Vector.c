#include <math.h>
#include "Vector.h"

VOID Angles_normalize(struct Vector* vec)
{
    vec->x = isfinite(vec->x) ? remainderf(vec->x, 360.0f) : 0.0f;
    vec->y = isfinite(vec->y) ? remainderf(vec->y, 360.0f) : 0.0f;
    vec->z = 0.0f;
}

struct Vector Vector_multiply(struct Vector CONST* v1, struct Vector CONST* v2)
{
    struct Vector result = { v1->x * v2->x, v1->y * v2->y, v1->z * v2->z };
    return result;
}

struct Vector Vector_add(struct Vector CONST* v1, struct Vector CONST* v2)
{
    struct Vector result = { v1->x + v2->x, v1->y + v2->y, v1->z + v2->z };
    return result;
}

FLOAT Vector_dotProduct(struct Vector CONST* v1, struct Vector CONST* v2)
{
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}
