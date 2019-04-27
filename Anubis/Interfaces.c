#include <stdio.h>
#include <Windows.h>

#include "interfaces.h"

Interfaces interfaces;

static void* find(const char* module, const char* name)
{
    void*(*createInterface)(const char*, int*) = (void*(*)(const char*, int*))GetProcAddress(GetModuleHandle(module), "CreateInterface");
    void* foundInterface = NULL;

    if (createInterface)
        foundInterface = createInterface(name, NULL);
    
    if (foundInterface)
        return foundInterface;
    else {
        char buf[100];
        sprintf_s(buf, sizeof(buf), "Failed to find %s in %s!", name, module);
        MessageBox(NULL, buf, "Error", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }
}

void initializeInterfaces()
{
    interfaces.client = find("client_panorama", "VClient018");
    interfaces.gameUI = find("client_panorama", "GameUI011");
}
