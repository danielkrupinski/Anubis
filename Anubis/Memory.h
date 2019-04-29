#pragma once

#include <stdint.h>

typedef struct {
    void** clientMode;
    void(__fastcall* loadSky)(const char*);
    void* present;
} Memory;

void initializeMemory(void);

extern Memory memory;
