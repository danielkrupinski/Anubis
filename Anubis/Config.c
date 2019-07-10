#include <stdio.h>

#include "cJSON/cJSON.h"
#include "Config.h"

Config config;

void Config_save(void)
{
    cJSON* json = cJSON_CreateObject();

    cJSON* miscJson = cJSON_CreateObject();

    cJSON_AddBoolToObject(miscJson, "Auto strafe", config.misc.autostrafe);
    cJSON_AddBoolToObject(miscJson, "Bunny hop", config.misc.bunnyhop);
    cJSON_AddItemToObject(json, "Misc", miscJson);

    FILE* out = fopen("testConfig.json", "w");

    fprintf(out, cJSON_Print(json));
    fclose(out);
}
