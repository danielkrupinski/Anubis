#pragma once

#include <stdbool.h>

typedef struct Config {
    struct {
        bool bunnyhop;
        bool autostrafe;
    } misc;
} Config;

extern Config config;
