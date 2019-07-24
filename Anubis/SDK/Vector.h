#pragma once

#include <Windows.h>

typedef struct Vector {
    FLOAT x, y, z;
} Vector;

VOID Angles_normalize(Vector*);
