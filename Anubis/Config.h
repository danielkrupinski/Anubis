#pragma once

#include <stdbool.h>

typedef struct Config {
    struct {
        bool bunnyhop;
        bool autostrafe;
    } misc;
} Config;

#ifdef __cplusplus
extern "C" Config config;
#else
extern Config config;
#endif
