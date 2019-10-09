#pragma once

#include <stdint.h>

typedef struct {
    uintptr_t** client;
    uintptr_t** cvar;
    uintptr_t** debugOverlay;
    uintptr_t** engine;
    uintptr_t** engineTrace;
    uintptr_t** entityList;
    uintptr_t** gameUI;
    uintptr_t** localize;
    uintptr_t** panel;
    uintptr_t** surface;
} Interfaces;

void Interfaces_init(void);

extern Interfaces interfaces;
