#include <jni.h>
#include <vector>
#include <string>
#include <functional>
#include <mutex>
// Make Wrapper Return Value

std::mutex vm_mtx;
JavaVM* g_vm {nullptr};

inline std::function<void()> createReleaser(JNIEnv* env, jobject* reference)
{
    {
        std::lock_guard<std::mutex> lock(vm_mtx);
        if (g_vm == nullptr)
        {
            g_vm = env->GetJavaVM(&g_vam)
        }
    }

    jobject globalRef = env->NewGlobalRef(reference);
    auto ret = [globalRef]() {
        JNIEnv* env {nullptr};
        int getEnvStat = g_vm->GetEnv((void **)&env, JNI_VERSION_1_6);
        if (getEnvStat == JNI_EDETACHED) {
            if (g_vm->AttachCurrentThread((void **) &g_env, NULL) != 0) {
                return;
            }
        }
        else if (getEnvStat == JNI_EVERSION) {
            return;
        }
        else if (getEnvStat == JNI_OK) {
        } 

        jclass clazz = env->GetObjectClass(jobect);
        jemthodID release = env->GetMethodID(clazz, "close", "()V");
        env->CallVoidMethod();
        env->DeleteGlobalRef(globalRef);

        if (getEnvStat == JNI_EDETACHED)
            g_vm->DetachCurrentThread();
    }
    return ret;
}


inline jstring createWrapperString(JNIEnv* env, const std::string& value)
{
    return env->NewStringUTF(value.c_str());
}
inline jbooleanArray createWrapperBooleanArray(JNIEnv* env, const std::vector<bool>& value)
{
    auto ret = env->NewBooleanArray(value.size());
    if (!ret)
        return nullptr;
    auto* arr = env->GetBooleanArrayElements(ret, nullptr);
    if (!arr)
        return nullptr;

    for (size_t i = 0; i < value.size(); i++)
        arr[i] = (jboolean)value[i];

    env->ReleaseBooleanArrayElements(ret, arr, 0);
    return ret;
}
inline jbyteArray createWrapperInt8Array(JNIEnv* env, const std::vector<int8_t>& value)
{
    auto ret = env->NewByteArray(value.size());
    if (!ret)
        return nullptr;
    auto* arr = env->GetByteArrayElements(ret, nullptr);
    if (!arr)
        return nullptr;

    for (size_t i = 0; i < value.size(); i++)
        arr[i] = (jbyte)value[i];

    env->ReleaseByteArrayElements(ret, arr, 0);
    return ret;
}
inline jshortArray createWrapperInt16Array(JNIEnv* env, const std::vector<int16_t>& value)
{
    auto ret = env->NewShortArray(value.size());
    if (!ret)
        return nullptr;
    auto* arr = env->GetShortArrayElements(ret, nullptr);
    if (!arr)
        return nullptr;

    for (size_t i = 0; i < value.size(); i++)
        arr[i] = (jshort)value[i];

    env->ReleaseShortArrayElements(ret, arr, 0);
    return ret;
}
inline jintArray createWrapperInt32Array(JNIEnv* env, const std::vector<int32_t>& value) // intArray, enumArray
{
    auto ret = env->NewIntArray(value.size());
    if (!ret)
        return nullptr;
    auto* arr = env->GetIntArrayElements(ret, nullptr);
    if (!arr)
        return nullptr;

    for (size_t i = 0; i < value.size(); i++)
        arr[i] = (jint)value[i];

    env->ReleaseIntArrayElements(ret, arr, 0);
    return ret;
}
inline jlongArray createWrapperInt64Array(JNIEnv* env, const std::vector<int64_t>& value)
{
    auto ret = env->NewLongArray(value.size());
    if (!ret)
        return nullptr;
    auto* arr = env->GetLongArrayElements(ret, nullptr);
    if (!arr)
        return nullptr;

    for (size_t i = 0; i < value.size(); i++)
        arr[i] = (jlong)value[i];

    env->ReleaseLongArrayElements(ret, arr, 0);
    return ret;
}
inline jfloatArray createWrapperFloatArray(JNIEnv* env, const std::vector<float>& value)
{
    auto ret = env->NewFloatArray(value.size());
    if (!ret)
        return nullptr;
    auto* arr = env->GetFloatArrayElements(ret, nullptr);
    if (!arr)
        return nullptr;

    for (size_t i = 0; i < value.size(); i++)
        arr[i] = (jfloat)value[i];

    env->ReleaseFloatArrayElements(ret, arr, 0);
    return ret;
}
inline jdoubleArray createWrapperDoubleArray(JNIEnv* env, const std::vector<double>& value)
{
    auto ret = env->NewDoubleArray(value.size());
    if (!ret)
        return nullptr;
    auto* arr = env->GetDoubleArrayElements(ret, nullptr);
    if (!arr)
        return nullptr;

    for (size_t i = 0; i < value.size(); i++)
        arr[i] = (jdouble)value[i];

    env->ReleaseDoubleArrayElements(ret, arr, 0);
    return ret;
}
inline jobjectArray createWrapperStringArray(JNIEnv* env, const std::vector<std::string>& value) // stringArray
{
    auto jstringClazz = env->FindClass("java/lang/String");

    auto ret = env->NewObjectArray(value.size(), jstringClazz, nullptr);
    if (!ret)
        return nullptr;

    for (size_t i = 0; i < value.size(); i++)
    {
        auto jstring = createWrapperString(env, value[0]);
        env->SetObjectArrayElement(ret, i, jstring);
    }

    return ret;
}
inline jintArray createWrapperEnumArray(JNIEnv* env, const std::vector<int32_t>& value)
{
    auto ret = env->NewIntArray(value.size());
    if (!ret)
        return nullptr;
    auto* arr = env->GetIntArrayElements(ret, nullptr);
    if (!arr)
        return nullptr;

    for (size_t i = 0; i < value.size(); i++)
        arr[i] = (jint)value[i];

    env->ReleaseIntArrayElements(ret, arr, 0);
    return ret;
}

inline jlongArray createWrapperObjectArray(JNIEnv* env, const std::vector<void*>& value) // objectArray
{
    auto ret = env->NewLongArray(value.size());
    if (!ret)
        return nullptr;
    auto* arr = env->GetLongArrayElements(ret, nullptr);
    if (!arr)
        return nullptr;

    for (size_t i = 0; i < value.size(); i++)
        arr[i] = (jlong)value[i];

    env->ReleaseLongArrayElements(ret, arr, 0);
    return ret;
}
inline jobject createWrapperBooleanVector(JNIEnv* env, const std::vector<bool>& value)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodInit = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");

    jclass typeClass = env->FindClass("java/lang/Boolean");
    jmethodID methodTypeInit = env->GetMethodID(typeClass, "<init>", "(Z)V");

    jobject ret = env->NewObject(arrayListClass, methodInit);
    for (const auto& element : value)
    {
        jobject jelement = env->NewObject(typeClass, methodTypeInit, (jboolean)element);
        auto result = env->CallBooleanMethod(ret, methodAdd, jelement);
        if (result == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            env->DeleteLocalRef(ret);
            return nullptr;
        }
        env->DeleteLocalRef(jelement);
    }
    return ret;
}
inline jobject createWrapperInt8Vector(JNIEnv* env, const std::vector<int8_t>& value)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodInit = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");

    jclass typeClass = env->FindClass("java/lang/Byte");
    jmethodID methodTypeInit = env->GetMethodID(typeClass, "<init>", "(B)V");

    jobject ret = env->NewObject(arrayListClass, methodInit);
    for (const auto& element : value)
    {
        jobject jelement = env->NewObject(typeClass, methodTypeInit, (jbyte)element);
        auto result = env->CallBooleanMethod(ret, methodAdd, jelement);
        if (result == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            env->DeleteLocalRef(ret);
            return nullptr;
        }
        env->DeleteLocalRef(jelement);
    }
    return ret;
}
inline jobject createWrapperInt16Vector(JNIEnv* env, const std::vector<int16_t>& value)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodInit = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");

    jclass typeClass = env->FindClass("java/lang/Short");
    jmethodID methodTypeInit = env->GetMethodID(typeClass, "<init>", "(S)V");

    jobject ret = env->NewObject(arrayListClass, methodInit);
    for (const auto& element : value)
    {
        jobject jelement = env->NewObject(typeClass, methodTypeInit, (jshort)element);
        auto result = env->CallBooleanMethod(ret, methodAdd, jelement);
        if (result == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            env->DeleteLocalRef(ret);
            return nullptr;
        }
        env->DeleteLocalRef(jelement);
    }
    return ret;
}
inline jobject createWrapperInt32Vector(JNIEnv* env, const std::vector<int32_t>& value) // intVector, enumVector
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodInit = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");

    jclass typeClass = env->FindClass("java/lang/Integer");
    jmethodID methodTypeInit = env->GetMethodID(typeClass, "<init>", "(I)V");

    jobject ret = env->NewObject(arrayListClass, methodInit);
    for (const auto& element : value)
    {
        jobject jelement = env->NewObject(typeClass, methodTypeInit, (jint)element);
        auto result = env->CallBooleanMethod(ret, methodAdd, jelement);
        if (result == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            env->DeleteLocalRef(ret);
            return nullptr;
        }
        env->DeleteLocalRef(jelement);
    }
    return ret;
}
inline jobject createWrapperInt64Vector(JNIEnv* env, const std::vector<int64_t>& value) // intVector, enumVector
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodInit = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");

    jclass typeClass = env->FindClass("java/lang/Long");
    jmethodID methodTypeInit = env->GetMethodID(typeClass, "<init>", "(J)V");

    jobject ret = env->NewObject(arrayListClass, methodInit);
    for (const auto& element : value)
    {
        jobject jelement = env->NewObject(typeClass, methodTypeInit, (jlong)element);
        auto result = env->CallBooleanMethod(ret, methodAdd, jelement);
        if (result == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            env->DeleteLocalRef(ret);
            return nullptr;
        }
        env->DeleteLocalRef(jelement);
    }
    return ret;
}
inline jobject createWrapperFloatVector(JNIEnv* env, const std::vector<float>& value)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodInit = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");

    jclass typeClass = env->FindClass("java/lang/Float");
    jmethodID methodTypeInit = env->GetMethodID(typeClass, "<init>", "(F)V");

    jobject ret = env->NewObject(arrayListClass, methodInit);
    for (const auto& element : value)
    {
        jobject jelement = env->NewObject(typeClass, methodTypeInit, (jfloat)element);
        auto result = env->CallBooleanMethod(ret, methodAdd, jelement);
        if (result == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            env->DeleteLocalRef(ret);
            return nullptr;
        }
        env->DeleteLocalRef(jelement);
    }
    return ret;
}
inline jobject createWrapperDoubleVector(JNIEnv* env, const std::vector<double>& value)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodInit = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");

    jclass typeClass = env->FindClass("java/lang/Double");
    jmethodID methodTypeInit = env->GetMethodID(typeClass, "<init>", "(D)V");

    jobject ret = env->NewObject(arrayListClass, methodInit);
    for (const auto& element : value)
    {
        jobject jelement = env->NewObject(typeClass, methodTypeInit, (jdouble)element);
        auto result = env->CallBooleanMethod(ret, methodAdd, jelement);
        if (result == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            env->DeleteLocalRef(ret);
            return nullptr;
        }
        env->DeleteLocalRef(jelement);
    }
    return ret;
}
inline jobject createWrapperStringVector(JNIEnv* env, const std::vector<std::string>& value) // stringVector
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodInit = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");


    jobject ret = env->NewObject(arrayListClass, methodInit);
    for (const auto& element : value)
    {
        jobject jelement = createWrapperString(env, element);
        auto result = env->CallBooleanMethod(ret, methodAdd, jelement);
        if (result == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            env->DeleteLocalRef(ret);
            return nullptr;
        }
        env->DeleteLocalRef(jelement);
    }
    return ret;
}
inline jobject createWrapperEnumVector(JNIEnv* env, const std::vector<int32_t>& value)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodInit = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");

    jclass typeClass = env->FindClass("java/lang/Integer");
    jmethodID methodTypeInit = env->GetMethodID(typeClass, "<init>", "(I)V");

    jobject ret = env->NewObject(arrayListClass, methodInit);
    for (const auto& element : value)
    {
        jobject jelement = env->NewObject(typeClass, methodTypeInit, (jint)element);
        auto result = env->CallBooleanMethod(ret, methodAdd, jelement);
        if (result == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            env->DeleteLocalRef(ret);
            return nullptr;
        }
        env->DeleteLocalRef(jelement);
    }
    return ret;
}
inline jobject createWrapperObjectVector(JNIEnv* env, const std::vector<void*>& value) // objectVector
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodInit = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");

    jclass typeClass = env->FindClass("java/lang/Long");
    jmethodID methodTypeInit = env->GetMethodID(typeClass, "<init>", "(J)V");

    jobject ret = env->NewObject(arrayListClass, methodInit);
    for (const auto& element : value)
    {
        jobject jelement = env->NewObject(typeClass, methodTypeInit, (jlong)element);
        auto result = env->CallBooleanMethod(ret, methodAdd, jelement);
        if (result == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            env->DeleteLocalRef(ret);
            return nullptr;
        }
        env->DeleteLocalRef(jelement);
    }
    return ret;
}
// Make Native Input Value
inline std::string createNativeString(JNIEnv* env, jstring value)
{
    const char* temp = env->GetStringUTFChars(value, nullptr);
    if (temp == nullptr)
        return std::string();
    std::string ret{ temp };
    env->ReleaseStringUTFChars(value, temp);
    return ret;
}
inline std::vector<bool> createNativeBooleanArray(JNIEnv* env, jbooleanArray value)
{
    std::vector<bool> ret;
    auto* elements = env->GetBooleanArrayElements(value, nullptr);
    if (!elements)
        return ret;
    jint size = env->GetArrayLength(value);
    for (size_t i = 0; i < size; i++)
    {
        ret.push_back((bool)elements[i]);
    }
    env->ReleaseBooleanArrayElements(value, elements, 0);
    return ret;
}
inline std::vector<int8_t> createNativeInt8Array(JNIEnv* env, jbyteArray value)
{
    std::vector<int8_t> ret;
    auto* elements = env->GetByteArrayElements(value, nullptr);
    if (!elements)
        return ret;
    jint size = env->GetArrayLength(value);
    for (size_t i = 0; i < size; i++)
    {
        ret.push_back((int8_t)elements[i]);
    }
    env->ReleaseByteArrayElements(value, elements, 0);
    return ret;
}
inline std::vector<int16_t> createNativeInt16Array(JNIEnv* env, jshortArray value)
{
    std::vector<int16_t> ret;
    auto* elements = env->GetShortArrayElements(value, nullptr);
    if (!elements)
        return ret;
    jint size = env->GetArrayLength(value);
    for (size_t i = 0; i < size; i++)
    {
        ret.push_back((int16_t)elements[i]);
    }
    env->ReleaseShortArrayElements(value, elements, 0);
    return ret;
}
inline std::vector<int32_t> createNativeInt32Array(JNIEnv* env, jintArray value) // intArray, enumArray
{
    std::vector<int32_t> ret;
    auto* elements = env->GetIntArrayElements(value, nullptr);
    if (!elements)
        return ret;
    jint size = env->GetArrayLength(value);
    for (size_t i = 0; i < size; i++)
    {
        ret.push_back((int32_t)elements[i]);
    }
    env->ReleaseIntArrayElements(value, elements, 0);
    return ret;
}
inline std::vector<int64_t> createNativeInt64Array(JNIEnv* env, jlongArray value)
{
    std::vector<int64_t> ret;
    auto* elements = env->GetLongArrayElements(value, nullptr);
    if (!elements)
        return ret;
    jint size = env->GetArrayLength(value);
    for (size_t i = 0; i < size; i++)
    {
        ret.push_back((int64_t)elements[i]);
    }
    env->ReleaseLongArrayElements(value, elements, 0);
    return ret;
}
inline std::vector<float> createNativeFloatArray(JNIEnv* env, jfloatArray value)
{
    std::vector<float> ret;
    auto* elements = env->GetFloatArrayElements(value, nullptr);
    if (!elements)
        return ret;
    jint size = env->GetArrayLength(value);
    for (size_t i = 0; i < size; i++)
    {
        ret.push_back((float)elements[i]);
    }
    env->ReleaseFloatArrayElements(value, elements, 0);
    return ret;
}
inline std::vector<double> createNativeDoubleArray(JNIEnv* env, jdoubleArray value)
{
    std::vector<double> ret;
    auto* elements = env->GetDoubleArrayElements(value, nullptr);
    if (!elements)
        return ret;
    jint size = env->GetArrayLength(value);
    for (size_t i = 0; i < size; i++)
    {
        ret.push_back((double)elements[i]);
    }
    env->ReleaseDoubleArrayElements(value, elements, 0);
    return ret;
}
inline std::vector<std::string> createNativeStringArray(JNIEnv* env, jobjectArray value) // stringArray
{
    std::vector<std::string> ret;
    jint size = env->GetArrayLength(value);
    for (size_t i = 0; i < size; i++)
    {
        jstring jelement = (jstring)env->GetObjectArrayElement(value, i);
        auto element = createNativeString(env, jelement);
        ret.push_back(element);
    }
    return ret;
}
inline std::vector<int32_t> createNativeEnumArray(JNIEnv* env, jintArray value)
{
    std::vector<int32_t> ret;
    auto* elements = env->GetIntArrayElements(value, nullptr);
    if (!elements)
        return ret;
    jint size = env->GetArrayLength(value);
    for (size_t i = 0; i < size; i++)
    {
        ret.push_back((int32_t)elements[i]);
    }
    env->ReleaseIntArrayElements(value, elements, 0);
    return ret;
}
inline std::vector<void*> createNativeObjectArray(JNIEnv* env, jlongArray value) // objectArray
{
    std::vector<void*> ret;
    auto* elements = env->GetLongArrayElements(value, nullptr);
    if (!elements)
        return ret;
    jint size = env->GetArrayLength(value);
    for (size_t i = 0; i < size; i++)
    {
        ret.push_back((void*)elements[i]);
    }
    env->ReleaseLongArrayElements(value, elements, 0);
    return ret;
}
inline std::vector<bool> createNativeBooleanVector(JNIEnv* env, jobject value)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodSize = env->GetMethodID(arrayListClass, "size", "()I");
    jmethodID methodGet = env->GetMethodID(arrayListClass, "get", "(I)Ljava/lang/Object;");

    jclass typeClass = env->FindClass("java/lang/Boolean");
    jmethodID methodTypeValue = env->GetMethodID(typeClass, "booleanValue", "()Z");

    jint size = env->CallIntMethod(value, methodSize);

    std::vector<bool> ret;
    for (jint i = 0; i < size; i++)
    {
        jobject jelement = env->CallObjectMethod(value, methodGet, i);
        if (jelement == nullptr)
        {
            ret.clear();
            return ret;
        }
        auto element = static_cast<bool>(env->CallBooleanMethod(jelement, methodTypeValue));
        ret.push_back(element);
        env->DeleteLocalRef(jelement);
    }

    return ret;
}
inline std::vector<int8_t> createNativeInt8Vector(JNIEnv* env, jobject value)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodSize = env->GetMethodID(arrayListClass, "size", "()I");
    jmethodID methodGet = env->GetMethodID(arrayListClass, "get", "(I)Ljava/lang/Object;");

    jclass typeClass = env->FindClass("java/lang/Byte");
    jmethodID methodTypeValue = env->GetMethodID(typeClass, "byteValue", "()B");

    jint size = env->CallIntMethod(value, methodSize);

    std::vector<int8_t> ret;
    for (jint i = 0; i < size; i++)
    {
        jobject jelement = env->CallObjectMethod(value, methodGet, i);
        if (jelement == nullptr)
        {
            ret.clear();
            return ret;
        }
        auto element = static_cast<int8_t>(env->CallByteMethod(jelement, methodTypeValue));
        ret.push_back(element);
        env->DeleteLocalRef(jelement);
    }

    return ret;
}
inline std::vector<int16_t> createNativeInt16Vector(JNIEnv* env, jobject value)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodSize = env->GetMethodID(arrayListClass, "size", "()I");
    jmethodID methodGet = env->GetMethodID(arrayListClass, "get", "(I)Ljava/lang/Object;");

    jclass typeClass = env->FindClass("java/lang/Short");
    jmethodID methodTypeValue = env->GetMethodID(typeClass, "shortValue", "()S");

    jint size = env->CallIntMethod(value, methodSize);

    std::vector<int16_t> ret;
    for (jint i = 0; i < size; i++)
    {
        jobject jelement = env->CallObjectMethod(value, methodGet, i);
        if (jelement == nullptr)
        {
            ret.clear();
            return ret;
        }
        auto element = static_cast<int16_t>(env->CallShortMethod(jelement, methodTypeValue));
        ret.push_back(element);
        env->DeleteLocalRef(jelement);
    }

    return ret;
}
inline std::vector<int32_t> createNativeInt32Vector(JNIEnv* env, jobject value) // intVector, enumVector
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodSize = env->GetMethodID(arrayListClass, "size", "()I");
    jmethodID methodGet = env->GetMethodID(arrayListClass, "get", "(I)Ljava/lang/Object;");

    jclass typeClass = env->FindClass("java/lang/Integer");
    jmethodID methodTypeValue = env->GetMethodID(typeClass, "intValue", "()I");

    jint size = env->CallIntMethod(value, methodSize);

    std::vector<int32_t> ret;
    for (jint i = 0; i < size; i++)
    {
        jobject jelement = env->CallObjectMethod(value, methodGet, i);
        if (jelement == nullptr)
        {
            ret.clear();
            return ret;
        }
        auto element = static_cast<int32_t>(env->CallIntMethod(jelement, methodTypeValue));
        ret.push_back(element);
        env->DeleteLocalRef(jelement);
    }
    return ret;
}
inline std::vector<int64_t> createNativeInt64Vector(JNIEnv* env, jobject value) // intVector, enumVector
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodSize = env->GetMethodID(arrayListClass, "size", "()I");
    jmethodID methodGet = env->GetMethodID(arrayListClass, "get", "(I)Ljava/lang/Object;");

    jclass typeClass = env->FindClass("java/lang/Long");
    jmethodID methodTypeValue = env->GetMethodID(typeClass, "longValue", "()J");

    jint size = env->CallIntMethod(value, methodSize);

    std::vector<int64_t> ret;
    for (jint i = 0; i < size; i++)
    {
        jobject jelement = env->CallObjectMethod(value, methodGet, i);
        if (jelement == nullptr)
        {
            ret.clear();
            return ret;
        }
        auto element = static_cast<int64_t>(env->CallLongMethod(jelement, methodTypeValue));
        ret.push_back(element);
        env->DeleteLocalRef(jelement);
    }
    return ret;
}
inline std::vector<float> createNativeFloatVector(JNIEnv* env, jobject value)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodSize = env->GetMethodID(arrayListClass, "size", "()I");
    jmethodID methodGet = env->GetMethodID(arrayListClass, "get", "(I)Ljava/lang/Object;");

    jclass typeClass = env->FindClass("java/lang/Float");
    jmethodID methodTypeValue = env->GetMethodID(typeClass, "floatValue", "()F");

    jint size = env->CallIntMethod(value, methodSize);

    std::vector<float> ret;
    for (jint i = 0; i < size; i++)
    {
        jobject jelement = env->CallObjectMethod(value, methodGet, i);
        if (jelement == nullptr)
        {
            ret.clear();
            return ret;
        }
        auto element = static_cast<float>(env->CallFloatMethod(jelement, methodTypeValue));
        ret.push_back(element);
        env->DeleteLocalRef(jelement);
    }
    return ret;
}
inline std::vector<double> createNativeDoubleVector(JNIEnv* env, jobject value)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodSize = env->GetMethodID(arrayListClass, "size", "()I");
    jmethodID methodGet = env->GetMethodID(arrayListClass, "get", "(I)Ljava/lang/Object;");

    jclass typeClass = env->FindClass("java/lang/Double");
    jmethodID methodTypeValue = env->GetMethodID(typeClass, "doubleValue", "()D");

    jint size = env->CallIntMethod(value, methodSize);

    std::vector<double> ret;
    for (jint i = 0; i < size; i++)
    {
        jobject jelement = env->CallObjectMethod(value, methodGet, i);
        if (jelement == nullptr)
        {
            ret.clear();
            return ret;
        }
        auto element = static_cast<double>(env->CallDoubleMethod(jelement, methodTypeValue));
        ret.push_back(element);
        env->DeleteLocalRef(jelement);
    }
    return ret;
}
inline std::vector<std::string> createNativeStringVector(JNIEnv* env, jobject value) // stringVector
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodSize = env->GetMethodID(arrayListClass, "size", "()I");
    jmethodID methodGet = env->GetMethodID(arrayListClass, "get", "(I)Ljava/lang/Object;");

    jint size = env->CallIntMethod(value, methodSize);

    std::vector<std::string> ret;
    for (jint i = 0; i < size; i++)
    {
        jstring jelement = (jstring)env->CallObjectMethod(value, methodGet, i);
        if (jelement == nullptr)
        {
            ret.clear();
            return ret;
        }
        auto element = createNativeString(env, jelement);
        ret.push_back(element);
        env->DeleteLocalRef(jelement);
    }
    return ret;
}
inline std::vector<int32_t> createNativeEnumVector(JNIEnv* env, jobject value)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodSize = env->GetMethodID(arrayListClass, "size", "()I");
    jmethodID methodGet = env->GetMethodID(arrayListClass, "get", "(I)Ljava/lang/Object;");

    jclass typeClass = env->FindClass("java/lang/Integer");
    jmethodID methodTypeValue = env->GetMethodID(typeClass, "intValue", "()I");

    jint size = env->CallIntMethod(value, methodSize);

    std::vector<int32_t> ret;
    for (jint i = 0; i < size; i++)
    {
        jobject jelement = env->CallObjectMethod(value, methodGet, i);
        if (jelement == nullptr)
        {
            ret.clear();
            return ret;
        }
        auto element = static_cast<int32_t>(env->CallIntMethod(jelement, methodTypeValue));
        ret.push_back(element);
        env->DeleteLocalRef(jelement);
    }
    return ret;
}
inline std::vector<void*> createNativeObjectVector(JNIEnv* env, jobject value) // objectVector
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodSize = env->GetMethodID(arrayListClass, "size", "()I");
    jmethodID methodGet = env->GetMethodID(arrayListClass, "get", "(I)Ljava/lang/Object;");

    jclass typeClass = env->FindClass("java/lang/Long");
    jmethodID methodTypeValue = env->GetMethodID(typeClass, "longValue", "()J");

    jint size = env->CallIntMethod(value, methodSize);

    std::vector<void*> ret;
    for (jint i = 0; i < size; i++)
    {
        jobject jelement = env->CallObjectMethod(value, methodGet, i);
        if (jelement == nullptr)
        {
            ret.clear();
            return ret;
        }
        auto element = (void*)(env->CallLongMethod(jelement, methodTypeValue));
        ret.push_back(element);
        env->DeleteLocalRef(jelement);
    }
    return ret;
}
// Copy Native Output to Wrapper Output

inline bool copyBooleanArray(JNIEnv* env, const std::vector<bool>& src, jbooleanArray dest)
{
    jsize jsize = env->GetArrayLength(dest);
    if (src.size() != jsize)
        return false;

    auto* jelements = env->GetBooleanArrayElements(dest, nullptr);
    if (!jelements)
        return false;

    for (size_t i = 0; i < src.size(); i++)
    {
        jelements[i] = (jboolean)src[i];
    }

    env->ReleaseBooleanArrayElements(dest, jelements, 0);
    return true;
}
inline bool copyInt8Array(JNIEnv* env, const std::vector<int8_t>& src, jbyteArray dest)
{
    jsize jsize = env->GetArrayLength(dest);
    if (src.size() != jsize)
        return false;

    auto* jelements = env->GetByteArrayElements(dest, nullptr);
    if (!jelements)
        return false;

    for (size_t i = 0; i < src.size(); i++)
    {
        jelements[i] = (jbyte)src[i];
    }

    env->ReleaseByteArrayElements(dest, jelements, 0);
    return true;
}
inline bool copyInt16Array(JNIEnv* env, const std::vector<int16_t>& src, jshortArray dest)
{
    jsize jsize = env->GetArrayLength(dest);
    if (src.size() != jsize)
        return false;

    auto* jelements = env->GetShortArrayElements(dest, nullptr);
    if (!jelements)
        return false;

    for (size_t i = 0; i < src.size(); i++)
    {
        jelements[i] = (jshort)src[i];
    }

    env->ReleaseShortArrayElements(dest, jelements, 0);
    return true;
}
inline bool copyInt32Array(JNIEnv* env, const std::vector<int32_t>& src, jintArray dest)
{
    jsize jsize = env->GetArrayLength(dest);
    if (src.size() != jsize)
        return false;

    auto* jelements = env->GetIntArrayElements(dest, nullptr);
    if (!jelements)
        return false;

    for (size_t i = 0; i < src.size(); i++)
    {
        jelements[i] = (jint)src[i];
    }

    env->ReleaseIntArrayElements(dest, jelements, 0);
    return true;
}
inline bool copyInt64Array(JNIEnv* env, const std::vector<int64_t>& src, jlongArray dest)
{
    jsize jsize = env->GetArrayLength(dest);
    if (src.size() != jsize)
        return false;

    auto* jelements = env->GetLongArrayElements(dest, nullptr);
    if (!jelements)
        return false;

    for (size_t i = 0; i < src.size(); i++)
    {
        jelements[i] = (jlong)src[i];
    }

    env->ReleaseLongArrayElements(dest, jelements, 0);
    return true;
}
inline bool copyFloatArray(JNIEnv* env, const std::vector<float>& src, jfloatArray dest)
{
    jsize jsize = env->GetArrayLength(dest);
    if (src.size() != jsize)
        return false;

    auto* jelements = env->GetFloatArrayElements(dest, nullptr);
    if (!jelements)
        return false;

    for (size_t i = 0; i < src.size(); i++)
    {
        jelements[i] = (jfloat)src[i];
    }

    env->ReleaseFloatArrayElements(dest, jelements, 0);
    return true;
}
inline bool copyDoubleArray(JNIEnv* env, const std::vector<double>& src, jdoubleArray dest)
{
    jsize jsize = env->GetArrayLength(dest);
    if (src.size() != jsize)
        return false;

    auto* jelements = env->GetDoubleArrayElements(dest, nullptr);
    if (!jelements)
        return false;

    for (size_t i = 0; i < src.size(); i++)
    {
        jelements[i] = (jdouble)src[i];
    }

    env->ReleaseDoubleArrayElements(dest, jelements, 0);
    return true;
}
inline bool copyStringArray(JNIEnv* env, const std::vector<std::string>& src, jobjectArray dest)
{
    jsize jsize = env->GetArrayLength(dest);
    if (src.size() != jsize)
        return false;

    for (size_t i = 0; i < src.size(); i++)
    {
        jstring jelement = createWrapperString(env, src[i]);
        if (!jelement)
        {
            return false;
        }
        env->SetObjectArrayElement(dest, i, jelement);
        env->DeleteLocalRef(jelement);
    }

    return true;
}
inline bool copyEnumArray(JNIEnv* env, const std::vector<int32_t>& src, jintArray dest)
{
    jsize jsize = env->GetArrayLength(dest);
    if (src.size() != jsize)
        return false;

    auto* jelements = env->GetIntArrayElements(dest, nullptr);
    if (!jelements)
        return false;

    for (size_t i = 0; i < src.size(); i++)
    {
        jelements[i] = (jint)src[i];
    }

    env->ReleaseIntArrayElements(dest, jelements, 0);
    return true;
}
inline bool copyObjectArray(JNIEnv* env, const std::vector<void*>& src, jlongArray dest)
{
    jsize jsize = env->GetArrayLength(dest);
    if (src.size() != jsize)
        return false;

    auto* jelements = env->GetLongArrayElements(dest, nullptr);
    if (!jelements)
        return false;

    for (size_t i = 0; i < src.size(); i++)
    {
        jelements[i] = (jlong)src[i];
    }

    env->ReleaseLongArrayElements(dest, jelements, 0);
    return true;
}
inline bool copyBooleanVector(JNIEnv* env, const std::vector<bool>& src, jobject dest)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jmethodID methodClear = env->GetMethodID(arrayListClass, "clear", "()V");

    jclass typeClass = env->FindClass("java/lang/Boolean");
    jmethodID methodTypeInit = env->GetMethodID(typeClass, "<init>", "(Z)V");

    env->CallVoidMethod(dest, methodClear);
    for (const auto& element : src)
    {
        jobject jelement = env->NewObject(typeClass, methodTypeInit, (jboolean)element);
        if (!jelement)
            return false;

        auto ret = env->CallBooleanMethod(dest, methodAdd, jelement);
        if (ret == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            return false;
        }

        env->DeleteLocalRef(jelement);
    }
    return true;
}
inline bool copyInt8Vector(JNIEnv* env, const std::vector<int8_t>& src, jobject dest)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jmethodID methodClear = env->GetMethodID(arrayListClass, "clear", "()V");

    jclass typeClass = env->FindClass("java/lang/Byte");
    jmethodID methodTypeInit = env->GetMethodID(typeClass, "<init>", "(B)V");

    env->CallVoidMethod(dest, methodClear);
    for (const auto& element : src)
    {
        jobject jelement = env->NewObject(typeClass, methodTypeInit, (jbyte)element);
        if (!jelement)
            return false;

        auto ret = env->CallBooleanMethod(dest, methodAdd, jelement);
        if (ret == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            return false;
        }

        env->DeleteLocalRef(jelement);
    }
    return true;
}
inline bool copyInt16Vector(JNIEnv* env, const std::vector<int16_t>& src, jobject dest)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jmethodID methodClear = env->GetMethodID(arrayListClass, "clear", "()V");

    jclass typeClass = env->FindClass("java/lang/Short");
    jmethodID methodTypeInit = env->GetMethodID(typeClass, "<init>", "(S)V");

    env->CallVoidMethod(dest, methodClear);
    for (const auto& element : src)
    {
        jobject jelement = env->NewObject(typeClass, methodTypeInit, (jshort)element);
        if (!jelement)
            return false;

        auto ret = env->CallBooleanMethod(dest, methodAdd, jelement);
        if (ret == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            return false;
        }

        env->DeleteLocalRef(jelement);
    }
    return true;
}
inline bool copyInt32Vector(JNIEnv* env, const std::vector<int32_t>& src, jobject dest)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jmethodID methodClear = env->GetMethodID(arrayListClass, "clear", "()V");

    jclass typeClass = env->FindClass("java/lang/Integer");
    jmethodID methodTypeInit = env->GetMethodID(typeClass, "<init>", "(I)V");

    env->CallVoidMethod(dest, methodClear);
    for (const auto& element : src)
    {
        jobject jelement = env->NewObject(typeClass, methodTypeInit, (jint)element);
        if (!jelement)
            return false;

        auto ret = env->CallBooleanMethod(dest, methodAdd, jelement);
        if (ret == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            return false;
        }

        env->DeleteLocalRef(jelement);
    }
    return true;
}
inline bool copyInt64Vector(JNIEnv* env, const std::vector<int64_t>& src, jobject dest)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jmethodID methodClear = env->GetMethodID(arrayListClass, "clear", "()V");

    jclass typeClass = env->FindClass("java/lang/Long");
    jmethodID methodTypeInit = env->GetMethodID(typeClass, "<init>", "(J)V");

    env->CallVoidMethod(dest, methodClear);
    for (const auto& element : src)
    {
        jobject jelement = env->NewObject(typeClass, methodTypeInit, (jlong)element);
        if (!jelement)
            return false;

        auto ret = env->CallBooleanMethod(dest, methodAdd, jelement);
        if (ret == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            return false;
        }

        env->DeleteLocalRef(jelement);
    }
    return true;
}
inline bool copyFloatVector(JNIEnv* env, const std::vector<float>& src, jobject dest)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jmethodID methodClear = env->GetMethodID(arrayListClass, "clear", "()V");

    jclass typeClass = env->FindClass("java/lang/Float");
    jmethodID methodTypeInit = env->GetMethodID(typeClass, "<init>", "(F)V");

    env->CallVoidMethod(dest, methodClear);
    for (const auto& element : src)
    {
        jobject jelement = env->NewObject(typeClass, methodTypeInit, (jfloat)element);
        if (!jelement)
            return false;

        auto ret = env->CallBooleanMethod(dest, methodAdd, jelement);
        if (ret == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            return false;
        }

        env->DeleteLocalRef(jelement);
    }
    return true;
}
inline bool copyDoubleVector(JNIEnv* env, const std::vector<double>& src, jobject dest)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jmethodID methodClear = env->GetMethodID(arrayListClass, "clear", "()V");

    jclass typeClass = env->FindClass("java/lang/Float");
    jmethodID methodTypeInit = env->GetMethodID(typeClass, "<init>", "(D)V");

    env->CallVoidMethod(dest, methodClear);
    for (const auto& element : src)
    {
        jobject jelement = env->NewObject(typeClass, methodTypeInit, (jdouble)element);
        if (!jelement)
            return false;

        auto ret = env->CallBooleanMethod(dest, methodAdd, jelement);
        if (ret == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            return false;
        }

        env->DeleteLocalRef(jelement);
    }
    return true;
}
inline bool copyStringVector(JNIEnv* env, const std::vector<std::string>& src, jobject dest)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jmethodID methodClear = env->GetMethodID(arrayListClass, "clear", "()V");

    env->CallVoidMethod(dest, methodClear);
    for (const auto& element : src)
    {
        jstring jelement = createWrapperString(env, element);
        if (!jelement)
            return false;

        auto ret = env->CallBooleanMethod(dest, methodAdd, jelement);
        if (ret == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            return false;
        }

        env->DeleteLocalRef(jelement);
    }
    return true;
}
inline bool copyEnumVector(JNIEnv* env, const std::vector<int32_t>& src, jobject dest)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jmethodID methodClear = env->GetMethodID(arrayListClass, "clear", "()V");

    jclass typeClass = env->FindClass("java/lang/Integer");
    jmethodID methodTypeInit = env->GetMethodID(typeClass, "<init>", "(I)V");

    env->CallVoidMethod(dest, methodClear);
    for (const auto& element : src)
    {
        jobject jelement = env->NewObject(typeClass, methodTypeInit, (jint)element);
        if (!jelement)
            return false;

        auto ret = env->CallBooleanMethod(dest, methodAdd, jelement);
        if (ret == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            return false;
        }

        env->DeleteLocalRef(jelement);
    }
    return true;
}
inline bool copyObjectVector(JNIEnv* env, const std::vector<void*>& src, jobject dest)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID methodAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jmethodID methodClear = env->GetMethodID(arrayListClass, "clear", "()V");

    jclass typeClass = env->FindClass("java/lang/Long");
    jmethodID methodTypeInit = env->GetMethodID(typeClass, "<init>", "(J)V");

    env->CallVoidMethod(dest, methodClear);
    for (const auto& element : src)
    {
        jobject jelement = env->NewObject(typeClass, methodTypeInit, (jlong)element);
        if (!jelement)
            return false;

        auto ret = env->CallBooleanMethod(dest, methodAdd, jelement);
        if (ret == JNI_FALSE)
        {
            env->DeleteLocalRef(jelement);
            return false;
        }

        env->DeleteLocalRef(jelement);
    }
    return true;
}