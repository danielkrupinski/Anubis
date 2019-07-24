#pragma once

#include <Windows.h>

typedef struct GlobalVars {
    const FLOAT realTime;
    const INT frameCount;
    const FLOAT absoluteFrameTime;
    const BYTE pad[4];
    const FLOAT currentTime;
    const FLOAT frameTime;
    const INT maxClients;
    const INT tickCount;
    const FLOAT intervalPerTick;
} GlobalVars;
