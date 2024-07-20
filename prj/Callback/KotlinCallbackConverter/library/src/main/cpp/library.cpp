#include "library.h"
#include <jni.h>
#include <string>

struct KotlinCallbackConverter_Context
{
    void (*callback)(int, float) { nullptr };
};

static KotlinCallbackConverter_Context _context;

KotlinCallbackConverter_Context* KotlinCallbackConverter_initialize()
{
    return &_context;
}
void KotlinCallbackConverter_release(KotlinCallbackConverter_Context* context)
{
    context->callback = nullptr;
}
void KotlinCallbackConverter_registerCallback(KotlinCallbackConverter_Context* context, void (*callback)(int, float))
{
    context->callback = callback;
}
void KotlinCallbackConverter_unregisterCallback(KotlinCallbackConverter_Context* context)
{
    context->callback = nullptr;
}
void KotlinCallbackConverter_runCallback(KotlinCallbackConverter_Context* context, int param1, float param2)
{
    context->callback(param1, param2);
}