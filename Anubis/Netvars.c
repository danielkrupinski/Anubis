#include <stdbool.h>
#include <stdio.h>

#include "Netvars.h"
#include "SDK/Client.h"
#include "SDK/ClientClass.h"
#include "SDK/Recv.h"
#include "SDK/Utils.h"

typedef struct Offset {
    UINT nameHash;
    SIZE_T offset;
    struct Offset* next;
} Offset;

static Offset firstOffset;

static VOID traverseTable(bool unload, const char* networkName, RecvTable* recvTable, const SIZE_T offset)
{
    Offset* currentOffset = &firstOffset;

    for (int i = 0; i < recvTable->propCount; i++) {
        RecvProp* prop = recvTable->props + i;

        if (isdigit(prop->name[0]))
            continue;

        if (Utils_hashRuntime(prop->name) == UTILS_HASH("baseclass"))
            continue;

        if (prop->type == 6
            && prop->dataTable
            && prop->dataTable->netTableName[0] == 'D')
            traverseTable(unload, networkName, prop->dataTable, prop->offset + offset);

        CHAR name[256];
        sprintf_s(name, sizeof(name), "%s->%s", networkName, prop->name);
        const UINT hash = Utils_hashRuntime(name);

        if (!unload) {
            currentOffset->nameHash = hash;
            currentOffset->offset = offset + prop->offset;
            currentOffset->next = calloc(1, sizeof(Offset));
            currentOffset = currentOffset->next;
        } else {

        }
    }
}

VOID Netvars_init(VOID)
{
    for (ClientClass* clientClass = Client_getAllClasses(); clientClass; clientClass = clientClass->next)
        traverseTable(false, clientClass->networkName, clientClass->recvTable, 0);
}

SIZE_T Netvars_getOffset(UINT hash)
{
    for (Offset* offset = &firstOffset; offset; offset = offset->next)
        if (hash == offset->nameHash)
            return offset->offset;
    return 0;
}
