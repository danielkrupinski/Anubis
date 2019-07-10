#include <ShlObj.h>
#include <Shlwapi.h>
#include <stdio.h>
#include "cJSON/cJSON.h"
#include "Config.h"

Config config;

static WCHAR path[MAX_PATH];

void Config_init(LPCWSTR name)
{
    PWSTR pathToDocuments;
    if (SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_Documents, 0, NULL, &pathToDocuments))) {
        swprintf(path, MAX_PATH, L"%s/%s/", pathToDocuments, name);
        if (!PathFileExistsW(path))
            CreateDirectoryW(path, NULL);
        CoTaskMemFree(pathToDocuments);
    }
}

void Config_save(void)
{
    cJSON* json = cJSON_CreateObject();

    cJSON* miscJson = cJSON_CreateObject();

    cJSON_AddBoolToObject(miscJson, "Auto strafe", config.misc.autostrafe);
    cJSON_AddBoolToObject(miscJson, "Bunny hop", config.misc.bunnyhop);
    cJSON_AddItemToObject(json, "Misc", miscJson);

    WCHAR filename[MAX_PATH];
    swprintf(filename, MAX_PATH, L"%s%s", path, L"config.json");

    if (!PathFileExistsW(path))
        CreateDirectoryW(path, NULL);

    FILE* out = _wfopen(filename, L"w");

    if (out) {
        fprintf(out, cJSON_Print(json));
        fclose(out);
    }

    cJSON_Delete(json);
}
