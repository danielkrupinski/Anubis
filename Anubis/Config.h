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
VOID Config_init(PCSTR);
VOID Config_add(PCSTR);
VOID Config_rename(size_t, PCSTR);
VOID Config_load(UINT);
VOID Config_save(UINT);
VOID Config_reset(VOID);
VOID Config_remove(UINT);

#ifdef __cplusplus
}
#endif