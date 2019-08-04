#pragma once

#include <stdint.h>

typedef struct {
    uintptr_t** client;
    uintptr_t** engine;
    uintptr_t** engineTrace;
    uintptr_t** entityList;
    uintptr_t** gameUI;
    uintptr_t** surface;
} Interfaces;

void Interfaces_init(void);

extern Interfaces interfaces;
