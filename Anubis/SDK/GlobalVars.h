#pragma once

#include <Windows.h>

struct UserCmd;

struct GlobalVars {
    const FLOAT realTime;
    const INT frameCount;
    const FLOAT absoluteFrameTime;
    const BYTE pad[4];
    const FLOAT currentTime;
    const FLOAT frameTime;
    const INT maxClients;
    const INT tickCount;
    const FLOAT intervalPerTick;
};

FLOAT GlobalVars_serverTime(struct UserCmd*);
