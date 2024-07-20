#include <jni.h>
#include <string>
#include <library.h>

static JavaVM* _vm;

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    _vm = vm;
    return JNI_VERSION_1_6;// JavaVM을 전역 변수에 저장
}
static JNIEnv* getEnv(JavaVM* vm) {
    JNIEnv* env {nullptr};
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        // 현재 스레드에 JNIEnv가 첨부되지 않은 경우 첨부 시도
        if (vm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
            return nullptr;
        }
    }
    return env;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_wrapper_Wrapper_initalize(JNIEnv *env, jobject thiz) {
    auto ret = KotlinCallbackConverter_initialize();
    return (jlong)ret;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_wrapper_Wrapper_release(JNIEnv *env, jobject thiz, jlong context) {
    KotlinCallbackConverter_Context* _context = (KotlinCallbackConverter_Context*)context;
    KotlinCallbackConverter_release(_context);
}


static jobject _callback;
void __callback(int a, float b)
{
    if (auto* env = getEnv(_vm))
    {
        jclass callbackClass = env->GetObjectClass(_callback);
        jmethodID methodId = env->GetMethodID(callbackClass, "invoke",
                                              "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

        constexpr char* class_name = "java/lang/Integer";

        jclass IntClass = env->FindClass("java/lang/Integer");
        jclass FloatClass = env->FindClass("java/lang/Float");

        jmethodID intCreate = env->GetStaticMethodID(IntClass, "valueOf", "(I)Ljava/lang/Integer;");
        jmethodID floatCreate = env->GetStaticMethodID(FloatClass, "valueOf", "(F)Ljava/lang/Float;");

        jobject param1 = env->CallStaticObjectMethod(IntClass, intCreate, a);
        jobject param2 = env->CallStaticObjectMethod(FloatClass, floatCreate, b);

        env->CallObjectMethod(_callback, methodId, param1, param2);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_wrapper_Wrapper_registerCallback(JNIEnv *env, jobject thiz, jlong context,
                                                          jobject callback) {
    _callback = env->NewGlobalRef(callback);
    KotlinCallbackConverter_registerCallback((KotlinCallbackConverter_Context*)context, __callback);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_wrapper_Wrapper_unregisterCallback(JNIEnv *env, jobject thiz,
                                                            jlong context) {
    // TODO: implement unregisterCallback()
    KotlinCallbackConverter_unregisterCallback((KotlinCallbackConverter_Context*)context);
    env->DeleteGlobalRef(_callback);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_wrapper_Wrapper_runCallback(JNIEnv *env, jobject thiz, jlong context,
                                                     jint param1, jfloat param2) {
    KotlinCallbackConverter_runCallback((KotlinCallbackConverter_Context*)context, param1, param2);
}