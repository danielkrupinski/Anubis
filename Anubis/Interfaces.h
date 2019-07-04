#pragma once

#include <stdint.h>

typedef struct {
    uintptr_t** client;
    uintptr_t** engine;
    uintptr_t** entityList;
    uintptr_t** gameUI;
} Interfaces;

void initializeInterfaces(void);

extern Interfaces interfaces;
