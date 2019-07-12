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
        cJSON* miscJson  = cJSON_GetObjectItem(json, "Misc");

        cJSON* autoStrafe = cJSON_GetObjectItem(miscJson, "Auto strafe");
        if (cJSON_IsBool(autoStrafe)) config.misc.autostrafe = cJSON_IsTrue(autoStrafe);
        cJSON* bunnyHop = cJSON_GetObjectItem(miscJson, "Bunny hop");
        if (cJSON_IsBool(bunnyHop)) config.misc.bunnyhop = cJSON_IsTrue(bunnyHop);

    }

    cJSON_Delete(json);
}

VOID Config_save(UINT id)
{
    cJSON* json = cJSON_CreateObject();

    cJSON* miscJson = cJSON_CreateObject();

    cJSON_AddBoolToObject(miscJson, "Auto strafe", config.misc.autostrafe);
    cJSON_AddBoolToObject(miscJson, "Bunny hop", config.misc.bunnyhop);
    cJSON_AddItemToObject(json, "Misc", miscJson);

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
    config.misc.autostrafe = false;
    config.misc.bunnyhop = false;
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
