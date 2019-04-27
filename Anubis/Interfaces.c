#include <stdio.h>
#include <Windows.h>

#include "interfaces.h"
#include "SDK/GameUI.h"

Interfaces interfaces;

static void* find(const char* module, const char* name)
{
    void**(*createInterface)(const char*, int*) = (void**(*)(const char*, int*))GetProcAddress(GetModuleHandle(module), "CreateInterface");
    void** foundInterface = 0;

    if (createInterface)
        foundInterface = createInterface(name, 0);
    
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
    interfaces.engine = find("engine", "VEngineClient014");
    interfaces.gameUI = find("client_panorama", "GameUI011");

    GameUI_messageBox("Huge success!", "Anubis has been succesfully loaded.");
}
