#pragma once

#include <stdbool.h>
#include <Windows.h>

typedef struct Config {
    struct {
        bool bunnyhop;
        bool autostrafe;
    } misc;

    size_t count;
    PSTR* names;
} Config;

#ifdef __cplusplus
extern "C"
{
#endif

extern Config config;
void Config_init(PCSTR);
void Config_add(PSTR);
void Config_save(void);

#ifdef __cplusplus
}
#endif