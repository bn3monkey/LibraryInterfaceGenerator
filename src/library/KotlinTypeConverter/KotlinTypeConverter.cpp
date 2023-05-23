#include "KotlinTypeConverter.hpp"

#define BN3MONKEY_DEBUG
#ifdef BN3MONKEY_DEBUG
#define FOR_DEBUG(t) t
#else
#define FOR_DEBUG(t)
#endif

#ifdef __BN3MONKEY_LOG__
#ifdef BN3MONKEY_DEBUG
#define LOG_D(text, ...) Bn3Monkey::Log::D(__FUNCTION__, text, ##__VA_ARGS__)
#else
#define LOG_D(text, ...)
#endif
#define LOG_V(text, ...) Bn3Monkey::Log::V(__FUNCTION__, text, ##__VA_ARGS__)
#define LOG_E(text, ...) Bn3Monkey::Log::E(__FUNCTION__, text, ##__VA_ARGS__)
#else
#define LOG_D(text, ...)
#define LOG_V(text, ...)
#define LOG_E(text, ...)
#endif
Bn3Monkey::Kotlin::KotlinTypeConverter::Helper* Bn3Monkey::Kotlin::KotlinTypeConverter::helper {nullptr};

bool Bn3Monkey::Kotlin::KotlinTypeConverter::isInitialized {false};
JavaVM* Bn3Monkey::Kotlin::KotlinTypeConverter::jvm;

static inline jclass newGlobalClass(JNIEnv* env, const char* class_name)
{
    jclass clazz = env->FindClass(class_name);
    return reinterpret_cast<jclass>(env->NewGlobalRef(reinterpret_cast<jobject>(clazz)));
}
static inline jmethodID newGlobalMethodID(JNIEnv* env, jclass clazz, const char* method_name, const char* sig)
{
    jmethodID methodID = env->GetMethodID(clazz, method_name, sig);
    //return reinterpret_cast<jmethodID>(env->NewGlobalRef(reinterpret_cast<jobject>(methodID)));
    return methodID;
}

bool Bn3Monkey::Kotlin::KotlinTypeConverter::initialize(JNIEnv* env)
{
    isInitialized = false;

    helper = new Helper();

    do {
        auto ret = env->GetJavaVM(&jvm);
        if (jvm == nullptr) {
            LOG_E("%s : Cannot get JVM", __FUNCTION__);
            break;
        }

        helper->_KBoolean = newGlobalClass(env, "java/lang/Boolean");
        if (helper->_KBoolean == nullptr) {
            LOG_E("%s : Boolean Class cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KBoolean_init = newGlobalMethodID(env, helper->_KBoolean, "<init>", "(Z)V");
        if (helper->_KBoolean_init == nullptr) {
            LOG_E("%s : Boolean Class / init method cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KBoolean_booleanValue = newGlobalMethodID(env, helper->_KBoolean, "booleanValue", "()Z");
        if (helper->_KBoolean_booleanValue == nullptr) {
            LOG_E("%s : Boolean Class / booleanValue method cannot be loaded", __FUNCTION__);
            break;
        }

        helper->_KByte = newGlobalClass(env, "java/lang/Byte");
        if (helper->_KByte == nullptr) {
            LOG_E("%s : Byte Class cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KByte_init = newGlobalMethodID(env, helper->_KByte, "<init>", "(B)V");
        if (helper->_KByte_init == nullptr) {
            LOG_E("%s : Byte Class / init method cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KByte_byteValue = newGlobalMethodID(env, helper->_KByte, "byteValue", "()B");
        if (helper->_KByte_byteValue == nullptr) {
            LOG_E("%s : Byte Class / byteValue method cannot be loaded", __FUNCTION__);
            break;
        }

        helper->_KShort = newGlobalClass(env, "java/lang/Short");
        if (helper->_KShort == nullptr) {
            LOG_E("%s : Short Class cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KShort_init = newGlobalMethodID(env, helper->_KShort, "<init>", "(S)V");
        if (helper->_KShort_init == nullptr) {
            LOG_E("%s : Short Class / init method cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KShort_shortValue = newGlobalMethodID(env, helper->_KShort, "shortValue", "()S");
        if (helper->_KShort_shortValue == nullptr) {
            LOG_E("%s : Short Class / shortValue method cannot be loaded", __FUNCTION__);
            break;
        }

        helper->_KInt = newGlobalClass(env, "java/lang/Integer");
        if (helper->_KInt == nullptr) {
            LOG_E("%s : Int Class cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KInt_init = newGlobalMethodID(env, helper->_KInt, "<init>", "(I)V");
        if (helper->_KInt_init == nullptr) {
            LOG_E("%s : Int Class / init method cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KInt_intValue = newGlobalMethodID(env, helper->_KInt, "intValue", "()I");
        if (helper->_KInt_intValue == nullptr) {
            LOG_E("%s : Int Class / intValue method cannot be loaded", __FUNCTION__);
            break;
        }

        helper->_KLong = newGlobalClass(env, "java/lang/Long");
        if (helper->_KLong == nullptr) {
            LOG_E("%s : Long Class cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KLong_init = newGlobalMethodID(env, helper->_KLong, "<init>", "(J)V");
        if (helper->_KLong_init == nullptr) {
            LOG_E("%s : Long Class / init method cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KLong_longValue = newGlobalMethodID(env, helper->_KLong, "longValue", "()J");
        if (helper->_KLong_longValue == nullptr) {
            LOG_E("%s : Long Class / longValue method cannot be loaded", __FUNCTION__);
            break;
        }

        helper->_KFloat = newGlobalClass(env, "java/lang/Float");
        if (helper->_KFloat == nullptr) {
            LOG_E("%s : Float Class cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KFloat_init = newGlobalMethodID(env, helper->_KFloat, "<init>", "(F)V");
        if (helper->_KFloat_init == nullptr) {
            LOG_E("%s : Float Class / init method cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KFloat_floatValue = newGlobalMethodID(env, helper->_KFloat, "floatValue", "()F");
        if (helper->_KFloat_floatValue == nullptr) {
            LOG_E("%s : Float Class / floatValue method cannot be loaded", __FUNCTION__);
            break;
        }

        helper->_KDouble = newGlobalClass(env, "java/lang/Double");
        if (helper->_KDouble == nullptr) {
            LOG_E("%s : Double Class cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KDouble_init = newGlobalMethodID(env, helper->_KDouble, "<init>", "(D)V");
        if (helper->_KDouble_init == nullptr) {
            LOG_E("%s : Double Class / init method cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KDouble_doubleValue = newGlobalMethodID(env, helper->_KDouble, "doubleValue", "()D");
        if (helper->_KDouble_doubleValue == nullptr) {
            LOG_E("%s : Double Class / doubleValue method cannot be loaded", __FUNCTION__);
            break;
        }

        helper->_KVector = newGlobalClass(env, "java/util/ArrayList");
        if (helper->_KVector == nullptr)
        {
            LOG_E("%s : Array List Class cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KVector_init = newGlobalMethodID(env, helper->_KVector, "<init>", "()V");
        if (helper->_KVector_init == nullptr) {
            LOG_E("%s : Array List Class / init method cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KVector_add = newGlobalMethodID(env, helper->_KVector, "add", "(Ljava/lang/Object;)Z");
        if (helper->_KVector_add == nullptr) {
            LOG_E("%s : Array List Class / add method cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KVector_size = newGlobalMethodID(env, helper->_KVector, "size", "()I");
        if (helper->_KVector_size == nullptr) {
            LOG_E("%s : Array List Class / size method cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KVector_get = newGlobalMethodID(env, helper->_KVector, "get", "(I)Ljava/lang/Object;");
        if (helper->_KVector_get == nullptr) {
            LOG_E("%s : Array List Class / get method cannot be loaded", __FUNCTION__);
            break;
        }
        helper->_KVector_clear = newGlobalMethodID(env, helper->_KVector, "clear", "()V");
        if (helper->_KVector_clear == nullptr) {
            LOG_E("%s : Array List Class / clear method cannot be loaded", __FUNCTION__);
            break;
        }

        helper->_KString = newGlobalClass(env, "java/lang/String");
        if (helper->_KString == nullptr) {
            LOG_E("%s : String class cannot be loaded", __FUNCTION__);
            break;
        }

        isInitialized = true;
    } while(false);

    if (!isInitialized)
        release(env);

    return isInitialized;
}

static inline void deleteClass(JNIEnv* env, jclass& clazz)
{
    if (clazz)
    {
        env->DeleteGlobalRef(clazz);
        clazz = nullptr;
    }
}
/*
static inline void deleteMethod(JNIEnv* env, jmethodID& method)
{
    if (method)
    {
        env->DeleteGlobalRef(reinterpret_cast<jobject>(method));
        method = nullptr;
    }
}
*/
void Bn3Monkey::Kotlin::KotlinTypeConverter::release(JNIEnv* env)
{
    deleteClass(env, helper->_KBoolean);
    deleteClass(env, helper->_KByte);
    deleteClass(env, helper->_KShort);
    deleteClass(env, helper->_KInt);
    deleteClass(env, helper->_KLong);
    deleteClass(env, helper->_KFloat);
    deleteClass(env, helper->_KDouble);
    deleteClass(env, helper->_KVector);
    deleteClass(env, helper->_KString);

    /*
    deleteMethod(env, helper->_KBoolean_init);
    deleteMethod(env, helper->_KBoolean_booleanValue);
    deleteMethod(env, helper->_KByte_init);
    deleteMethod(env, helper->_KByte_byteValue);
    deleteMethod(env, helper->_KShort_init);
    deleteMethod(env, helper->_KShort_shortValue);
    deleteMethod(env, helper->_KInt_init);
    deleteMethod(env, helper->_KInt_intValue);
    deleteMethod(env, helper->_KLong_init);
    deleteMethod(env, helper->_KLong_longValue);
    deleteMethod(env, helper->_KFloat_init);
    deleteMethod(env, helper->_KFloat_floatValue);
    deleteMethod(env, helper->_KDouble_init);
    deleteMethod(env, helper->_KDouble_doubleValue);
    deleteMethod(env, helper->_KVector_init);
    deleteMethod(env, helper->_KVector_add);
    deleteMethod(env, helper->_KVector_get);
    deleteMethod(env, helper->_KVector_size);
     */
    if (helper) {
        delete helper;
        helper = nullptr;
    }
}
