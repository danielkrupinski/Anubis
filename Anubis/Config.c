#include <ShlObj.h>
#include <Shlwapi.h>
#include <stdio.h>
#include "cJSON/cJSON.h"
#include "Config.h"

Config config;

static CHAR path[MAX_PATH];

void Config_init(PCSTR name)
{
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

void Config_add(PCSTR name)
{
    config.count++;
    void* newAlloc = realloc(config.names, config.count * sizeof(PSTR));
    if (newAlloc)
        config.names = newAlloc;
    newAlloc = malloc(strlen(name) + 1);
    if (newAlloc)
        config.names[config.count - 1] = newAlloc;
    strcpy(config.names[config.count - 1], name);
}

void Config_rename(size_t id, PCSTR newName)
{
    if (strlen(newName) > strlen(config.names[id])) {
        void* newAlloc = realloc(config.names[id], strlen(config.names[id]) + 1);
        if (newAlloc)
            config.names[id] = newAlloc;
    }
    CHAR fileName[MAX_PATH], newFileName[MAX_PATH];
    sprintf(fileName, "%s%s", path, config.names[id]);
    sprintf(newFileName, "%s%s", path, newName);

    if (MoveFileA(fileName, newFileName))
        strcpy(config.names[id], newName);
}

void Config_save(void)
{
    cJSON* json = cJSON_CreateObject();

    cJSON* miscJson = cJSON_CreateObject();

    cJSON_AddBoolToObject(miscJson, "Auto strafe", config.misc.autostrafe);
    cJSON_AddBoolToObject(miscJson, "Bunny hop", config.misc.bunnyhop);
    cJSON_AddItemToObject(json, "Misc", miscJson);

    CHAR filename[MAX_PATH];
    sprintf(filename, "%s%s", path, "config.json");

    if (!PathFileExistsA(path))
        CreateDirectoryA(path, NULL);

    FILE* out = fopen(filename, "w");

    if (out) {
        fprintf(out, cJSON_Print(json));
        fclose(out);
    }

    cJSON_Delete(json);
}
