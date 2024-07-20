// dllmain.cpp : DLL 애플리케이션의 진입점을 정의합니다.
#include "pch.h"

#include <Windows.h>
#include <cstdio>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

struct CSharpTypeLibrary_Context
{
    void (*callback)(int, float) { nullptr };
};

static CSharpTypeLibrary_Context _context;

CSharpTypeLibrary_Context* CSharpTypeLibrary_initialize()
{
    return &_context;
}
void CSharpTypeLibrary_release(CSharpTypeLibrary_Context* context)
{
    context->callback = nullptr;
}
void CSharpTypeLibrary_registerCallback(CSharpTypeLibrary_Context* context, void (*callback)(int, float))
{
    context->callback = callback;
}
void CSharpTypeLibrary_unregisterCallback(CSharpTypeLibrary_Context* context)
{
    context->callback = nullptr;
}
void CSharpTypeLibrary_runCallback(CSharpTypeLibrary_Context* context, int param1, float param2)
{
    context->callback(param1, param2);
}