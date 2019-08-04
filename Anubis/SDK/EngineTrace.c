#include "EngineTrace.h"
#include "../Interfaces.h"

static bool __fastcall shouldHitEntity(TraceFilter* traceFilter, PVOID _1, PVOID entity, INT _2)
{
    return entity != traceFilter->skip;
}

static INT __fastcall getTraceType()
{
    return 0;
}

static PVOID traceFilterVmt[2] = { shouldHitEntity, getTraceType };

VOID TraceFilter_init(TraceFilter* traceFilter)
{
    traceFilter->vmt = traceFilterVmt;
}

VOID EngineTrace_traceRay(const Ray* ray, UINT mask, const TraceFilter* filter, Trace* trace)
{
    CALL_VIRTUAL_METHOD(VOID(__fastcall*)(PVOID, PVOID, const Ray*, UINT, const TraceFilter*, Trace*), interfaces.engineTrace, 5, ray, mask, filter, trace);
}
