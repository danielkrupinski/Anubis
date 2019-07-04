#include <stdbool.h>

#include "GameUI.h"
#include "../Interfaces.h"

void GameUI_messageBox(const char* title, const char* text)
{
    typedef void(__stdcall *messageBox)(const char*, const char*, bool, bool, const char*, const char*, const char*, const char*, void*);
    __asm mov ecx, interfaces.gameUI
    ((messageBox)interfaces.gameUI[0][20])(title, text, true, false, 0, 0, 0, 0, 0);
}
