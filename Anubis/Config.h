#pragma once

#include <stdbool.h>
#include <Windows.h>

typedef struct GlowConfig {
    bool enabled;
    bool healthBased;
    bool rainbow;
    FLOAT thickness;
    FLOAT alpha;
    INT style;
    FLOAT color[3];
} GlowConfig;

typedef struct TriggerbotConfig {
    bool enabled;
    bool onKey;
    INT key;
    bool friendlyFire;
    bool scopedOnly;
    bool ignoreFlash;
    bool ignoreSmoke;
    INT hitgroup;
    INT shotDelay;
} TriggerbotConfig;

typedef struct Config {
    TriggerbotConfig triggerbot[35];
    GlowConfig glow[17];

    struct {
        bool bunnyhop;
        bool autostrafe;
        bool moonwalk;
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
VOID Config_resetTriggerbot(VOID);
VOID Config_resetGlow(VOID);
VOID Config_remove(UINT);

#ifdef __cplusplus
}
#endif