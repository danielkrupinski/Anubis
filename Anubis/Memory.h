#pragma once

typedef struct {
    void** clientMode;
    void(__fastcall* loadSky)(const char*);
} Memory;

void initializeMemory(void);

extern Memory memory;
