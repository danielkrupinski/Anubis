#include "Client.h"
#include "../Interfaces.h"
#include "Utils.h"

ClientClass* Client_getAllClasses(VOID)
{
    return CALL_VIRTUAL_METHOD(ClientClass*(__fastcall*)(PVOID, PVOID), interfaces.client, 8);
}
