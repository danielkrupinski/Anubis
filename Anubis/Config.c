#include <ShlObj.h>
#include <Shlwapi.h>
#include <stdio.h>
#include "cJSON/cJSON.h"
#include "Config.h"

struct Config config;

static CHAR path[MAX_PATH];

VOID Config_init()
{
    Config_reset();

    PWSTR pathToDocuments;
    if (SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_Documents, 0, NULL, &pathToDocuments))) {
        sprintf(path, "%ws/%s/", pathToDocuments, "Anubis");
        if (PathFileExistsA(path)) {
            strcat(path, "*");
            WIN32_FIND_DATAA foundData;
            HANDLE found = FindFirstFileA(path, &foundData);

            if (found != INVALID_HANDLE_VALUE) {
                do {
                    if (!(foundData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                        Config_add(foundData.cFileName);
                } while (FindNextFileA(found, &foundData));
            }
            path[strlen(path) - 1] = '\0';
        } else {
            CreateDirectoryA(path, NULL);
        }
        CoTaskMemFree(pathToDocuments);
    }
}

VOID Config_add(PCSTR name)
{
    config.count++;
    LPVOID newAlloc = realloc(config.names, config.count * sizeof(PSTR));
    if (newAlloc)
        config.names = newAlloc;
    newAlloc = malloc(strlen(name) + 1);
    if (newAlloc)
        config.names[config.count - 1] = newAlloc;
    strcpy(config.names[config.count - 1], name);
}

VOID Config_rename(size_t id, PCSTR newName)
{
    if (strlen(newName) > strlen(config.names[id])) {
        LPVOID newAlloc = realloc(config.names[id], strlen(config.names[id]) + 1);
        if (newAlloc)
            config.names[id] = newAlloc;
    }
    CHAR fileName[MAX_PATH], newFileName[MAX_PATH];
    sprintf(fileName, "%s%s", path, config.names[id]);
    sprintf(newFileName, "%s%s", path, newName);

    if (MoveFileA(fileName, newFileName))
        strcpy(config.names[id], newName);
}

VOID Config_load(UINT id)
{
    CHAR fileName[MAX_PATH];
    sprintf(fileName, "%s%s", path, config.names[id]);

    FILE* in = fopen(fileName, "r");

    if (!in)
        return;

    fseek(in, 0L, SEEK_END);
    LONG size = ftell(in);
    rewind(in);

    PSTR buffer = calloc(size + 1, sizeof(CHAR));
    size_t read = 0;
    if (buffer) read = fread(buffer, sizeof(CHAR), size, in);
    fclose(in);
    if (!read) return;

    cJSON* json = cJSON_Parse(buffer);
    if (json) {
        {
            const cJSON* triggerbotJson;
            INT i = 0;
            cJSON_ArrayForEach(triggerbotJson, cJSON_GetObjectItem(json, "Triggerbot")) {
                cJSON* enabled = cJSON_GetObjectItem(triggerbotJson, "Enabled");
                if (cJSON_IsBool(enabled)) config.triggerbot[i].enabled = cJSON_IsTrue(enabled);
                cJSON* onKey = cJSON_GetObjectItem(triggerbotJson, "On key");
                if (cJSON_IsBool(onKey)) config.triggerbot[i].onKey = cJSON_IsTrue(onKey);
                cJSON* key = cJSON_GetObjectItem(triggerbotJson, "Key");
                if (cJSON_IsNumber(key)) config.triggerbot[i].key = key->valueint;
                cJSON* friendlyFire = cJSON_GetObjectItem(triggerbotJson, "Friendly fire");
                if (cJSON_IsBool(friendlyFire)) config.triggerbot[i].friendlyFire = cJSON_IsTrue(friendlyFire);
                cJSON* scopedOnly = cJSON_GetObjectItem(triggerbotJson, "Scoped only");
                if (cJSON_IsBool(scopedOnly)) config.triggerbot[i].scopedOnly = cJSON_IsTrue(scopedOnly);
                cJSON* ignoreFlash = cJSON_GetObjectItem(triggerbotJson, "Ignore flash");
                if (cJSON_IsBool(ignoreFlash)) config.triggerbot[i].ignoreFlash = cJSON_IsTrue(ignoreFlash);
                cJSON* ignoreSmoke = cJSON_GetObjectItem(triggerbotJson, "Ignore smoke");
                if (cJSON_IsBool(ignoreSmoke)) config.triggerbot[i].ignoreSmoke = cJSON_IsTrue(ignoreSmoke);
                cJSON* hitgroup = cJSON_GetObjectItem(triggerbotJson, "Hitgroup");
                if (cJSON_IsNumber(hitgroup)) config.triggerbot[i].hitgroup = hitgroup->valueint;
                cJSON* shotDelay = cJSON_GetObjectItem(triggerbotJson, "Shot delay");
                if (cJSON_IsNumber(shotDelay)) config.triggerbot[i].shotDelay = shotDelay->valueint;
                cJSON* minDamage = cJSON_GetObjectItem(triggerbotJson, "Min damage");
                if (cJSON_IsNumber(minDamage)) config.triggerbot[i].minDamage = minDamage->valueint;
                cJSON* killshot = cJSON_GetObjectItem(triggerbotJson, "Killshot");
                if (cJSON_IsBool(killshot)) config.triggerbot[i].killshot = cJSON_IsTrue(killshot);

                i++;
            }
        }

        {
            const cJSON* glowJson;
            INT i = 0;
            cJSON_ArrayForEach(glowJson, cJSON_GetObjectItem(json, "Glow")) {
                cJSON* enabled = cJSON_GetObjectItem(glowJson, "Enabled");
                if (cJSON_IsBool(enabled)) config.glow[i].enabled = cJSON_IsTrue(enabled);
                cJSON* healthBased = cJSON_GetObjectItem(glowJson, "Health based");
                if (cJSON_IsBool(healthBased)) config.glow[i].healthBased = cJSON_IsTrue(healthBased);
                cJSON* rainbow = cJSON_GetObjectItem(glowJson, "Rainbow");
                if (cJSON_IsBool(rainbow)) config.glow[i].rainbow = cJSON_IsTrue(rainbow);
                cJSON* thickness = cJSON_GetObjectItem(glowJson, "Thickness");
                if (cJSON_IsNumber(thickness)) config.glow[i].thickness = (FLOAT)thickness->valuedouble;
                cJSON* alpha = cJSON_GetObjectItem(glowJson, "Alpha");
                if (cJSON_IsNumber(alpha)) config.glow[i].alpha = (FLOAT)alpha->valuedouble;
                cJSON* style = cJSON_GetObjectItem(glowJson, "Style");
                if (cJSON_IsNumber(style)) config.glow[i].style = style->valueint;
                const cJSON* colorJson;
                INT j = 0;
                cJSON_ArrayForEach(colorJson, cJSON_GetObjectItem(glowJson, "Color")) {
                    if (cJSON_IsNumber(colorJson)) config.glow[i].color[j] = (FLOAT)colorJson->valuedouble;
                    j++;
                }
                i++;
            }
        }

        {
            cJSON* miscJson = cJSON_GetObjectItem(json, "Misc");

            cJSON* autoStrafe = cJSON_GetObjectItem(miscJson, "Auto strafe");
            if (cJSON_IsBool(autoStrafe)) config.misc.autostrafe = cJSON_IsTrue(autoStrafe);
            cJSON* bunnyHop = cJSON_GetObjectItem(miscJson, "Bunny hop");
            if (cJSON_IsBool(bunnyHop)) config.misc.bunnyhop = cJSON_IsTrue(bunnyHop);
            cJSON* moonwalk = cJSON_GetObjectItem(miscJson, "Moonwalk");
            if (cJSON_IsBool(moonwalk)) config.misc.moonwalk = cJSON_IsTrue(moonwalk);
        }
    }

    cJSON_Delete(json);
}

VOID Config_save(UINT id)
{
    cJSON* json = cJSON_CreateObject();

    {
        cJSON* triggerbotJson = cJSON_AddArrayToObject(json, "Triggerbot");

        for (INT i = 0; i < sizeof(config.triggerbot) / sizeof(config.triggerbot[0]); i++) {
            cJSON* triggerbot = cJSON_CreateObject();

            cJSON_AddBoolToObject(triggerbot, "Enabled", config.triggerbot[i].enabled);
            cJSON_AddBoolToObject(triggerbot, "On key", config.triggerbot[i].onKey);
            cJSON_AddNumberToObject(triggerbot, "Key", config.triggerbot[i].key);
            cJSON_AddBoolToObject(triggerbot, "Friendly fire", config.triggerbot[i].friendlyFire);
            cJSON_AddBoolToObject(triggerbot, "Scoped only", config.triggerbot[i].scopedOnly);
            cJSON_AddBoolToObject(triggerbot, "Ignore flash", config.triggerbot[i].ignoreFlash);
            cJSON_AddBoolToObject(triggerbot, "Ignore smoke", config.triggerbot[i].ignoreSmoke);
            cJSON_AddNumberToObject(triggerbot, "Hitgroup", config.triggerbot[i].hitgroup);
            cJSON_AddNumberToObject(triggerbot, "Shot delay", config.triggerbot[i].shotDelay);
            cJSON_AddNumberToObject(triggerbot, "Min damage", config.triggerbot[i].minDamage);
            cJSON_AddBoolToObject(triggerbot, "Killshot", config.triggerbot[i].killshot);

            cJSON_AddItemToArray(triggerbotJson, triggerbot);
        }
    }

    {
        cJSON* glowJson = cJSON_AddArrayToObject(json, "Glow");

        for (INT i = 0; i < sizeof(config.glow) / sizeof(config.glow[0]); i++) {
            cJSON* glow = cJSON_CreateObject();

            cJSON_AddBoolToObject(glow, "Enabled", config.glow[i].enabled);
            cJSON_AddBoolToObject(glow, "Health based", config.glow[i].healthBased);
            cJSON_AddBoolToObject(glow, "Rainbow", config.glow[i].rainbow);
            cJSON_AddNumberToObject(glow, "Thickness", config.glow[i].thickness);
            cJSON_AddNumberToObject(glow, "Alpha", config.glow[i].alpha);
            cJSON_AddNumberToObject(glow, "Style", config.glow[i].style);
                
            cJSON* colorJson = cJSON_AddArrayToObject(glow, "Color");

            for (INT j = 0; j < 3; j++)
                cJSON_AddItemToArray(colorJson, cJSON_CreateNumber(config.glow[i].color[j]));

            cJSON_AddItemToArray(glowJson, glow);

        }
    }

    {
        cJSON* miscJson = cJSON_CreateObject();

        cJSON_AddBoolToObject(miscJson, "Auto strafe", config.misc.autostrafe);
        cJSON_AddBoolToObject(miscJson, "Bunny hop", config.misc.bunnyhop);
        cJSON_AddBoolToObject(miscJson, "Moonwalk", config.misc.moonwalk);
        cJSON_AddItemToObject(json, "Misc", miscJson);
    }

    CHAR fileName[MAX_PATH];
    sprintf(fileName, "%s%s", path, config.names[id]);

    if (!PathFileExistsA(path))
        CreateDirectoryA(path, NULL);

    FILE* out = fopen(fileName, "w");

    if (out) {
        fprintf(out, cJSON_Print(json));
        fclose(out);
    }

    cJSON_Delete(json);
}

VOID Config_reset(VOID)
{
    Config_resetTriggerbot();
    Config_resetGlow();
    Config_resetEsp();

    config.misc.autostrafe = false;
    config.misc.bunnyhop = false;
    config.misc.moonwalk = false;
}

VOID Config_resetTriggerbot(VOID)
{
    for (INT i = 0; i < sizeof(config.triggerbot) / sizeof(config.triggerbot[0]); i++) {
        config.triggerbot[i].enabled = false;
        config.triggerbot[i].onKey = false;
        config.triggerbot[i].key = 0;
        config.triggerbot[i].friendlyFire = false;
        config.triggerbot[i].scopedOnly = true;
        config.triggerbot[i].ignoreFlash = false;
        config.triggerbot[i].ignoreSmoke = false;
        config.triggerbot[i].hitgroup = 0;
        config.triggerbot[i].shotDelay = 0;
        config.triggerbot[i].minDamage = 1;
        config.triggerbot[i].killshot = false;
    }
}

VOID Config_resetGlow(VOID)
{
    for (INT i = 0; i < sizeof(config.glow) / sizeof(config.glow[0]); i++) {
        config.glow[i].enabled = false;
        config.glow[i].healthBased = false;
        config.glow[i].rainbow = false;
        config.glow[i].thickness = 1.0f;
        config.glow[i].alpha = 1.0f;
        config.glow[i].style = 0;
        config.glow[i].color[0] = 1.0f;
        config.glow[i].color[1] = 1.0f;
        config.glow[i].color[2] = 1.0f;
    }
}

VOID Config_resetEsp(VOID)
{
    for (INT i = 0; i < sizeof(config.esp.players) / sizeof(config.esp.players[0]); i++) {
        config.esp.players[i].enabled = false;
        config.esp.players[i].box = false;
        config.esp.players[i].boxColor[0] = 1.0f;
        config.esp.players[i].boxColor[1] = 1.0f;
        config.esp.players[i].boxColor[2] = 1.0f;
    }
    config.esp.weapon.enabled = false;
    config.esp.weapon.font = 0x1d;
    config.esp.weapon.box = false;
    config.esp.weapon.boxColor[0] = 1.0f;
    config.esp.weapon.boxColor[1] = 1.0f;
    config.esp.weapon.boxColor[2] = 1.0f;
    config.esp.weapon.boxType = 0;
    config.esp.weapon.snapLine = false;
    config.esp.weapon.snapLineColor[0] = 1.0f;
    config.esp.weapon.snapLineColor[1] = 1.0f;
    config.esp.weapon.snapLineColor[2] = 1.0f;
    config.esp.weapon.name = false;
    config.esp.weapon.nameColor[0] = 1.0f;
    config.esp.weapon.nameColor[1] = 1.0f;
    config.esp.weapon.nameColor[2] = 1.0f;
}

VOID Config_remove(UINT id)
{
    CHAR fileName[MAX_PATH];
    sprintf(fileName, "%s%s", path, config.names[id]);
    if (!DeleteFileA(fileName))
        return;

    free(config.names[id]);
    config.count--;

    for (SIZE_T i = id; i < config.count; i++)
        config.names[i] = config.names[i + 1];
}
