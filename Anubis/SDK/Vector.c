#include <math.h>

#include "Matrix3x4.h"
#include "Vector.h"

VOID Angles_normalize(struct Vector* vec)
{
    vec->x = isfinite(vec->x) ? remainderf(vec->x, 360.0f) : 0.0f;
    vec->y = isfinite(vec->y) ? remainderf(vec->y, 360.0f) : 0.0f;
    vec->z = 0.0f;
}

struct Vector Vector_multiply(CONST struct Vector* v1, CONST struct Vector* v2)
{
    struct Vector result = { v1->x * v2->x, v1->y * v2->y, v1->z * v2->z };
    return result;
}

struct Vector Vector_add(CONST struct Vector* v1, CONST struct Vector* v2)
{
    struct Vector result = { v1->x + v2->x, v1->y + v2->y, v1->z + v2->z };
    return result;
}

FLOAT Vector_dotProduct(CONST struct Vector* v1, CONST struct Vector* v2)
{
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

struct Vector Vector_transform(CONST struct Vector* vec, CONST struct Matrix3x4* mat)
{
    struct Vector x = { mat->m[0][0], mat->m[0][1], mat->m[0][2] },
                  y = { mat->m[1][0], mat->m[1][1], mat->m[1][2] },
                  z = { mat->m[2][0], mat->m[2][1], mat->m[2][2] };
    struct Vector result = { Vector_dotProduct(vec, &x) + mat->m[0][3], Vector_dotProduct(vec, &y) + mat->m[1][3], Vector_dotProduct(vec, &z) + mat->m[2][3] };
    return result;
}
