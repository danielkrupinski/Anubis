#include "Misc.h"
#include "../SDK/Engine.h"
#include "../SDK/EntityList.h"

void Misc_bunnyhop(UserCmd* cmd)
{
    if (!(*((int*)((char*)EntityList_getEntity(Engine_getLocalPlayer()) + 0x104)) & 1))
        cmd->buttons &= ~IN_JUMP;
}
