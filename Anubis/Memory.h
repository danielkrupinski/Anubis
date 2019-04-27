#pragma once

typedef struct {
    void(__fastcall* loadSky)(const char*);
} Memory;

void initializeMemory(void);

extern Memory memory;
