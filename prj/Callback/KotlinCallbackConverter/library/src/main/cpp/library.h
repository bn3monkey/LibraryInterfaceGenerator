//
// Created by bn3mo on 2024-07-20.
//

#ifndef KOTLINCALLBACKCONVERTER_LIBRARY_H
#define KOTLINCALLBACKCONVERTER_LIBRARY_H

struct KotlinCallbackConverter_Context;

KotlinCallbackConverter_Context* KotlinCallbackConverter_initialize();
void KotlinCallbackConverter_release(KotlinCallbackConverter_Context* context);
void KotlinCallbackConverter_registerCallback(KotlinCallbackConverter_Context* context, void (*callback)(int, float));
void KotlinCallbackConverter_unregisterCallback(KotlinCallbackConverter_Context* context);
void KotlinCallbackConverter_runCallback(KotlinCallbackConverter_Context* context, int param1, float param2);

#endif //KOTLINCALLBACKCONVERTER_LIBRARY_H
