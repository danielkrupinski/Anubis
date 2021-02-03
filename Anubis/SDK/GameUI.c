#include <stdbool.h>

#include "GameUI.h"
#include "../Interfaces.h"
#include "Utils.h"

VOID GameUI_messageBox(PCSTR title, PCSTR text)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, PCSTR, PCSTR, bool, bool, PCSTR, PCSTR, PCSTR, PCSTR, PVOID), Interfaces()->gameUI, 20, title, text, true, false, NULL, NULL, NULL, NULL, NULL);
}
