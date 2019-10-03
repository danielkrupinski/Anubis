#pragma once

#include <Windows.h>

struct Vector {
    FLOAT x, y, z;
};

VOID Angles_normalize(struct Vector*);

struct Vector Vector_multiply(struct Vector CONST*, struct Vector CONST*);
struct Vector Vector_add(struct Vector CONST*, struct Vector CONST*);
FLOAT Vector_dotProduct(struct Vector CONST*, struct Vector CONST*);
