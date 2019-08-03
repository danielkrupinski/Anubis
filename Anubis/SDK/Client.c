#include "Client.h"
#include "../Interfaces.h"
#include "Utils.h"

ClientClass* Client_getAllClasses(VOID)
{
    CALL_VIRTUAL_METHOD_RETURN(ClientClass*(__fastcall*)(PVOID, PVOID), interfaces.client, 8);
}
