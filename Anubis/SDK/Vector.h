#pragma once

#include <Windows.h>

typedef struct {
    FLOAT x, y, z;
} Vector;

VOID Angles_normalize(Vector*);
