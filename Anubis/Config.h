#pragma once

#include <stdbool.h>

typedef struct Config {
    struct {
        bool bunnyhop;
        bool autostrafe;
    } misc;
} Config;

#ifdef __cplusplus
extern "C"
{
#endif

extern Config config;
void Config_save(void);

#ifdef __cplusplus
}
#endif