#pragma once

#define CALL_VIRTUAL_METHOD(type, this, index, ...) ((type)((this)[0][index]))(this, 0, __VA_ARGS__);
#define CALL_VIRTUAL_METHOD_RETURN(type, this, index, ...) return ((type)((this)[0][index]))(this, 0, __VA_ARGS__);
#define CALL_VIRTUAL_METHOD_RETURN_TO_VARIABLE(type, this, index, result, ...) result = ((type)((this)[0][index]))(this, 0, __VA_ARGS__);

#define CALL_ORIGINAL(type, this, vmt, index, ...) ((type)((vmt)[index]))(this, 0, __VA_ARGS__);
#define CALL_ORIGINAL_RETURN(type, this, vmt, index, ...) return ((type)((vmt)[index]))(this, 0, __VA_ARGS__);
#define CALL_ORIGINAL_RETURN_TO_VARIABLE(type, this, vmt, index, result, ...) result = ((type)((vmt)[index]))(this, 0, __VA_ARGS__);
