#include "../Config.h"
#include "Misc.h"
#include "../SDK/Engine.h"
#include "../SDK/Entity.h"
#include "../SDK/EntityList.h"
#include "../SDK/UserCmd.h"

VOID Misc_bunnyhop(UserCmd* cmd)
{
    if (config.misc.bunnyhop && !(*Entity_flags(EntityList_getEntity(Engine_getLocalPlayer())) & 1))
        cmd->buttons &= ~IN_JUMP;
}

VOID Misc_autostrafe(UserCmd* cmd)
{
    if (config.misc.autostrafe && !(*Entity_flags(EntityList_getEntity(Engine_getLocalPlayer())) & 1)) {
        if (cmd->mousedx < 0)
            cmd->sidemove = -450.0f;
        else if (cmd->mousedx > 0)
            cmd->sidemove = 450.0f;
    }
}

VOID Misc_moonwalk(UserCmd* cmd)
{
    if (config.misc.moonwalk && *Entity_moveType(EntityList_getEntity(Engine_getLocalPlayer())) != MoveType_Ladder)
        cmd->buttons ^= IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT;
}
