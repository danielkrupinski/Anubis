#include "EngineTrace.h"
#include "../Interfaces.h"

VOID EngineTrace_traceRay(const Ray* ray, UINT mask, const TraceFilter* filter, Trace* trace)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, const Ray*, UINT, const TraceFilter*, Trace*), interfaces.engineTrace, 5, ray, mask, filter, trace);
}
