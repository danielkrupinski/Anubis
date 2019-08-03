#include "Utils.h"

UINT Utils_hashRuntime(PCSTR str)
{
    UINT hash = 0;
    for (SIZE_T i = 0; i < strlen(str); i++)
        hash = 65599 * hash + str[i];
    return hash ^ (hash >> 16);
}
