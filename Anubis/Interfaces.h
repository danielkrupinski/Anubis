#pragma once

typedef struct {
    void** client;
    void** engine;
    void** entityList;
    void** gameUI;
} Interfaces;

void initializeInterfaces(void);

extern Interfaces interfaces;
