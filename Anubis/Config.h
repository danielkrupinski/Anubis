#pragma once

#include <stdbool.h>
#include <Windows.h>

typedef struct Config {
    struct {
        bool bunnyhop;
        bool autostrafe;
    } misc;

    size_t count;
    LPSTR* names;
} Config;

#ifdef __cplusplus
extern "C"
{
#endif

extern Config config;
void Config_init(LPCWSTR);
void Config_save(void);

#ifdef __cplusplus
}
#endif