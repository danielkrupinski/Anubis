#pragma once

typedef struct Interfaces {
    void* client;
} Interfaces;

void initializeInterfaces();

extern Interfaces interfaces;
