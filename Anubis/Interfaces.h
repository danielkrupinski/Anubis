#pragma once

#include <stdint.h>

struct Interfaces {
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
};

void Interfaces_init(void);
const struct Interfaces* Interfaces(void);
