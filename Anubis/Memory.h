#pragma once

#include <stdint.h>

typedef struct {
    void** clientMode;
    void(__fastcall* loadSky)(const char*);
    void* present;
    void* reset;
} Memory;

void initializeMemory(void);

extern Memory memory;
