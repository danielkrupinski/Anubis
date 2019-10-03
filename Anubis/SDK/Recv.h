#pragma once

#include <Windows.h>
#include "Vector.h"

typedef struct RecvProxyData {
    INT pad;
    union {
        FLOAT _float;
        INT _int;
        PSTR _string;
        PVOID data;
        struct Vector vector;
        INT64 int64;
    } value;
} RecvProxyData;

typedef struct RecvProp {
    PSTR name;
    INT type;
    INT flags;
    INT stringBufferSize;
    INT insideArray;
    LPCVOID extraData;
    struct RecvProp* arrayProp;
    PVOID arrayLengthProxy;
    VOID(*proxy)(RecvProxyData*, PVOID, PVOID);
    PVOID dataTableProxy;
    struct RecvTable* dataTable;
    INT offset;
    INT elementStride;
    INT elementCount;
    PCSTR parentArrayPropName;
} RecvProp;

typedef struct RecvTable {
    RecvProp* props;
    INT propCount;
    PVOID decoder;
    PSTR netTableName;
    bool isInitialized;
    bool isInMainList;
} RecvTable;
