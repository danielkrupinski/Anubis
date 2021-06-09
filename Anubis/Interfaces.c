#include <stdio.h>
#include <Windows.h>

#include "Interfaces.h"
#include "SDK/GameUI.h"

static struct Interfaces interfaces;

static uintptr_t** find(const wchar_t* module, const char* name)
{
    typedef uintptr_t** (*CreateInterface)(const char*, int*);
    HMODULE moduleHandle = GetModuleHandleW(module);

    if (moduleHandle) {
        CreateInterface createInterface = (CreateInterface)GetProcAddress(moduleHandle, "CreateInterface");
        if (createInterface) {
            uintptr_t** foundInterface = createInterface(name, 0);
            if (foundInterface)
                return foundInterface;
        }
    }

    char buf[100];
    sprintf_s(buf, sizeof(buf), "Failed to find %s interface!", name);
    MessageBoxA(NULL, buf, "Error", MB_OK | MB_ICONERROR);
    exit(EXIT_FAILURE);
}

void Interfaces_init(void)
{
    interfaces.client = find(L"client", "VClient018");
    interfaces.cvar = find(L"vstdlib", "VEngineCvar007");
    interfaces.debugOverlay = find(L"engine", "VDebugOverlay004");
    interfaces.engine = find(L"engine", "VEngineClient014");
    interfaces.engineTrace = find(L"engine", "EngineTraceClient004");
    interfaces.entityList = find(L"client", "VClientEntityList003");
    interfaces.gameUI = find(L"client", "GameUI011");
    interfaces.localize = find(L"localize", "Localize_001");
    interfaces.panel = find(L"vgui2", "VGUI_Panel009");
    interfaces.surface = find(L"vguimatsurface", "VGUI_Surface031");

    GameUI_messageBox("Huge success!", "Anubis has been succesfully loaded.");
}

const struct Interfaces* Interfaces(void)
{
    return &interfaces;
}
