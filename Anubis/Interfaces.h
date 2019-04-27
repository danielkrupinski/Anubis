#pragma once

typedef struct Interfaces {
    void** client;
    void** engine;
    void** entityList;
    void** gameUI;
} Interfaces;

void initializeInterfaces(void);

extern Interfaces interfaces;
