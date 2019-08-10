#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

#define CALL_VIRTUAL_METHOD(type, this, index, ...) (((type)((this)[0][index]))(this, 0, __VA_ARGS__));
#define CALL_ORIGINAL(type, this, vmt, index, ...) (((type)((vmt)[index]))(this, 0, __VA_ARGS__));

#define UTILS_HASH_1(s, i, val) (val * 65599u + ((i) < strlen(s) ? s[strlen(s) - 1- (i)] : 0))
#define UTILS_HASH_4(s, i, val) UTILS_HASH_1(s, i, UTILS_HASH_1(s, i + 1, UTILS_HASH_1(s, i + 2, UTILS_HASH_1(s, i + 3, val))))
#define UTILS_HASH_16(s, i, val) UTILS_HASH_4(s, i, UTILS_HASH_4(s, i + 4, UTILS_HASH_4(s, i + 8, UTILS_HASH_4(s, i + 12, val))))
#define UTILS_HASH_64(s, i, val) UTILS_HASH_16(s, i, UTILS_HASH_16(s, i + 16, UTILS_HASH_16(s, i + 32, UTILS_HASH_16(s, i + 48, val))))
#define UTILS_HASH_256(s, i, val) UTILS_HASH_64(s, i, UTILS_HASH_64(s, i + 64, UTILS_HASH_64(s, i + 128, UTILS_HASH_64(s, i + 192, val))))

#define UTILS_HASH(s) ((UINT)(UTILS_HASH_256(s, 0, 0) ^ UTILS_HASH_256(s, 0, 0) >> 16))

UINT Utils_hashRuntime(PCSTR);

#define DEG2RAD(x) ((FLOAT)(x) * (FLOAT)M_PI / 180.0f)
#define RAD2DEG(x) ((FLOAT)(x) * 180.0f / (FLOAT)M_PI)

#define UTILS_STATIC_VAR(type, name, initializer) static type name = NULL; if (!name) name = initializer;