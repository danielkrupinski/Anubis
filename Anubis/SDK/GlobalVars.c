#include "GlobalVars.h"
#include "UserCmd.h"

FLOAT GlobalVars_serverTime(UserCmd* cmd)
{
    static int tick;
    static UserCmd* lastCmd;

    if (cmd) {
      //  if (!lastCmd || lastCmd->hasbeenpredicted)
       //     tick = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->getProperty<int>("m_nTickBase");
       // else
        //    tick++;
        lastCmd = cmd;
    }
   // return tick * intervalPerTick;

    return 0.0f;
}
