#pragma once

#include <Windows.h>

struct Matrix3x4;

struct Vector {
    FLOAT x, y, z;
};

VOID Angles_normalize(struct Vector*);

struct Vector Vector_multiply(CONST struct Vector*, CONST struct Vector*);
struct Vector Vector_add(CONST struct Vector*, CONST struct Vector*);
FLOAT Vector_dotProduct(CONST struct Vector*, CONST struct Vector*);
struct Vector Vector_transform(CONST struct Vector*, CONST struct Matrix3x4*);
