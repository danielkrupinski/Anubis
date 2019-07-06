#include <stdbool.h>

#include "GameUI.h"
#include "../Interfaces.h"
#include "Utils.h"

void GameUI_messageBox(const char* title, const char* text)
{
    CALL_VIRTUAL_METHOD(void(__fastcall*)(void*, void*, const char*, const char*, bool, bool, const char*, const char*, const char*, const char*, void*), interfaces.gameUI, 20, title, text, true, false, 0, 0, 0, 0, 0);
}
