#include <stdio.h>
#include <Windows.h>

#include "Interfaces.h"
#include "SDK/GameUI.h"

Interfaces interfaces;

static uintptr_t** find(const wchar_t* module, const char* name)
{
    typedef uintptr_t** (*CreateInterface)(const char*, int*);
    CreateInterface createInterface = (CreateInterface)GetProcAddress(GetModuleHandleW(module), "CreateInterface");
    uintptr_t** foundInterface = 0;

    if (createInterface)
        foundInterface = createInterface(name, 0);
    
    if (foundInterface)
        return foundInterface;
    else {
        char buf[100];
        sprintf_s(buf, sizeof(buf), "Failed to find %s interface!", name);
        MessageBox(NULL, buf, "Error", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }
}

void Interfaces_init(void)
{
    interfaces.client = find(L"client_panorama", "VClient018");
    interfaces.engine = find(L"engine", "VEngineClient014");
    interfaces.entityList = find(L"client_panorama", "VClientEntityList003");
    interfaces.gameUI = find(L"client_panorama", "GameUI011");
    interfaces.surface = find(L"vguimatsurface", "VGUI_Surface031");

    GameUI_messageBox("Huge success!", "Anubis has been succesfully loaded.");
}
