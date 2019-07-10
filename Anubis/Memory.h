#pragma once

#include <stdint.h>

typedef struct {
    void** clientMode;
    void(__fastcall* loadSky)(const char*);
    void*** present;
    void*** reset;
} Memory;

void Memory_init(void);

extern Memory memory;
