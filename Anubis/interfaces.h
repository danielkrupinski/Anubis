#pragma once

typedef struct Interfaces {
    void* client;
    void* gameUI;
} Interfaces;

void initializeInterfaces();

extern Interfaces interfaces;
