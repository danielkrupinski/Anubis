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
void Config_add(PCSTR);
void Config_rename(size_t, PCSTR);
VOID Config_load(UINT);
void Config_save(UINT);
VOID Config_reset(VOID);

#ifdef __cplusplus
}
#endif