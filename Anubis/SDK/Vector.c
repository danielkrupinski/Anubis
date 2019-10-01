#include <math.h>
#include "Vector.h"

VOID Angles_normalize(Vector* vec)
{
    vec->x = isfinite(vec->x) ? remainderf(vec->x, 360.0f) : 0.0f;
    vec->y = isfinite(vec->y) ? remainderf(vec->y, 360.0f) : 0.0f;
    vec->z = 0.0f;
}

Vector Vector_multiply(Vector* v1, Vector* v2)
{
    Vector result = { v1->x * v2->x, v1->y * v2->y, v1->z * v2->z };
    return result;
}

Vector Vector_add(Vector* v1, Vector* v2)
{
    Vector result = { v1->x + v2->x, v1->y + v2->y, v1->z + v2->z };
    return result;
}

FLOAT Vector_dotProduct(Vector CONST* v1, Vector CONST* v2)
{
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}
