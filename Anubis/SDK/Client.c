#include "Client.h"
#include "../Interfaces.h"
#include "Utils.h"

struct ClientClass* Client_getAllClasses(VOID)
{
    return CALL_VIRTUAL_METHOD(struct ClientClass*(__fastcall*)(PVOID, PVOID), Interfaces()->client, 8);
}
