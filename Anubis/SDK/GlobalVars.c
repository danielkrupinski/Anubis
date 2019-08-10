#include "GlobalVars.h"
#include "../Memory.h"
#include "UserCmd.h"

#include "../SDK/Engine.h"
#include "../SDK/Entity.h"
#include "../SDK/EntityList.h"

FLOAT GlobalVars_serverTime(UserCmd* cmd)
{
    static INT tick = 0;
    static UserCmd* lastCmd = NULL;

    if (cmd) {
        if (!lastCmd || lastCmd->hasbeenpredicted)
            tick = *Entity_tickBase(EntityList_getEntity(Engine_getLocalPlayer()));
       else
            tick++;
        lastCmd = cmd;
    }
   return tick * memory.globalVars->intervalPerTick;
}
