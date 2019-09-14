#pragma once

#include <Windows.h>
#include "SDK/Utils.h"

SIZE_T Netvars_getOffset(UINT);

#define NETVAR_OFFSET(funcname, class_name, var_name, offset, type) \
type* Entity_##funcname(PVOID entity);

#define NETVAR(funcname, class_name, var_name, type) \
	NETVAR_OFFSET(funcname, class_name, var_name, 0, type)

#define NETVAR_OFFSET_IMPL(funcname, class_name, var_name, offset, type) \
type* Entity_##funcname(PVOID entity) \
{ \
    UTILS_STATIC_VAR(SIZE_T, netvarOffset, Netvars_getOffset(UTILS_HASH(class_name "->" var_name))); \
	return (type*)((PBYTE)entity + netvarOffset + offset); \
}

#define NETVAR_IMPL(funcname, class_name, var_name, type) \
	NETVAR_OFFSET_IMPL(funcname, class_name, var_name, 0, type)
