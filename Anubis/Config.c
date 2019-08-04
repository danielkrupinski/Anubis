#include <ShlObj.h>
#include <Shlwapi.h>
#include <stdio.h>
#include "cJSON/cJSON.h"
#include "Config.h"

Config config;

static CHAR path[MAX_PATH];

VOID Config_init(PCSTR name)
{
    Config_reset();

    PWSTR pathToDocuments;
    if (SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_Documents, 0, NULL, &pathToDocuments))) {
        sprintf(path, "%ws/%s/", pathToDocuments, name);
        if (PathFileExistsA(path)) {
            strcat(path, "*");
            WIN32_FIND_DATA foundData;
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
            const cJSON* glowJson;
            int i = 0;
            cJSON_ArrayForEach(glowJson, cJSON_GetObjectItem(json, "Glow")) {
                cJSON* enabled = cJSON_GetObjectItem(glowJson, "Enabled");
                if (cJSON_IsBool(enabled)) config.glow[i].enabled = cJSON_IsTrue(enabled);
                cJSON* healthBased = cJSON_GetObjectItem(glowJson, "Health based");
                if (cJSON_IsBool(healthBased)) config.glow[i].healthBased = cJSON_IsTrue(healthBased);
                cJSON* rainbow = cJSON_GetObjectItem(glowJson, "Rainbow");
                if (cJSON_IsBool(rainbow)) config.glow[i].rainbow = cJSON_IsTrue(rainbow);
                cJSON* thickness = cJSON_GetObjectItem(glowJson, "Thickness");
                if (cJSON_IsNumber(thickness)) config.glow[i].thickness = (float)thickness->valuedouble;
                cJSON* alpha = cJSON_GetObjectItem(glowJson, "Alpha");
                if (cJSON_IsNumber(alpha)) config.glow[i].alpha = (float)alpha->valuedouble;
                cJSON* style = cJSON_GetObjectItem(glowJson, "Style");
                if (cJSON_IsNumber(style)) config.glow[i].style = style->valueint;
                const cJSON* colorJson;
                int j = 0;
                cJSON_ArrayForEach(colorJson, cJSON_GetObjectItem(glowJson, "Color")) {
                    if (cJSON_IsNumber(colorJson)) config.glow[i].color[j] = (float)colorJson->valuedouble;
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
        }
    }

    cJSON_Delete(json);
}

VOID Config_save(UINT id)
{
    cJSON* json = cJSON_CreateObject();

    {
        cJSON* glowJson = cJSON_AddArrayToObject(json, "Glow");

        for (int i = 0; i < sizeof(config.glow) / sizeof(config.glow[0]); i++) {
            cJSON* glow = cJSON_CreateObject();

            cJSON_AddBoolToObject(glow, "Enabled", config.glow[i].enabled);
            cJSON_AddBoolToObject(glow, "Health based", config.glow[i].healthBased);
            cJSON_AddBoolToObject(glow, "Rainbow", config.glow[i].rainbow);
            cJSON_AddNumberToObject(glow, "Thickness", config.glow[i].thickness);
            cJSON_AddNumberToObject(glow, "Alpha", config.glow[i].alpha);
            cJSON_AddNumberToObject(glow, "Style", config.glow[i].style);
                
            cJSON* colorJson = cJSON_AddArrayToObject(glow, "Color");

            for (int j = 0; j < 3; j++)
                cJSON_AddItemToArray(colorJson, cJSON_CreateNumber(config.glow[i].color[j]));

            cJSON_AddItemToArray(glowJson, glow);

        }
    }

    {
        cJSON* miscJson = cJSON_CreateObject();

        cJSON_AddBoolToObject(miscJson, "Auto strafe", config.misc.autostrafe);
        cJSON_AddBoolToObject(miscJson, "Bunny hop", config.misc.bunnyhop);
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
    Config_resetGlow();
    for (int i = 0; i < sizeof(config.triggerbot) / sizeof(config.triggerbot[0]); i++) {
        config.triggerbot[i].enabled = false;
        config.triggerbot[i].onKey = false;
        config.triggerbot[i].key = 0;
        config.triggerbot[i].friendlyFire = false;
        config.triggerbot[i].scopedOnly = false;
        config.triggerbot[i].ignoreFlash = false;
        config.triggerbot[i].ignoreSmoke = false;
        config.triggerbot[i].hitgroup = 0;
        config.triggerbot[i].shotDelay = 0;
    }

    config.misc.autostrafe = false;
    config.misc.bunnyhop = false;
}

VOID Config_resetGlow(VOID)
{
    for (int i = 0; i < sizeof(config.glow) / sizeof(config.glow[0]); i++) {
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
