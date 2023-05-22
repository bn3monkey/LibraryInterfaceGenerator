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

jclass Bn3Monkey::Kotlin::KotlinTypeConverter::_KBoolean {nullptr};
jclass Bn3Monkey::Kotlin::KotlinTypeConverter::_KByte {nullptr};
jclass Bn3Monkey::Kotlin::KotlinTypeConverter::_KShort {nullptr};
jclass Bn3Monkey::Kotlin::KotlinTypeConverter::_KInt {nullptr};
jclass Bn3Monkey::Kotlin::KotlinTypeConverter::_KLong {nullptr};
jclass Bn3Monkey::Kotlin::KotlinTypeConverter::_KFloat {nullptr};
jclass Bn3Monkey::Kotlin::KotlinTypeConverter::_KDouble {nullptr};
jclass Bn3Monkey::Kotlin::KotlinTypeConverter::_KVector {nullptr}; // arraylist
jclass Bn3Monkey::Kotlin::KotlinTypeConverter::_KString {nullptr};

jmethodID Bn3Monkey::Kotlin::KotlinTypeConverter::_KBoolean_init {nullptr};
jmethodID Bn3Monkey::Kotlin::KotlinTypeConverter::_KBoolean_booleanValue {nullptr};

jmethodID Bn3Monkey::Kotlin::KotlinTypeConverter::_KByte_init {nullptr};
jmethodID Bn3Monkey::Kotlin::KotlinTypeConverter::_KByte_byteValue {nullptr};

jmethodID Bn3Monkey::Kotlin::KotlinTypeConverter::_KShort_init {nullptr};
jmethodID Bn3Monkey::Kotlin::KotlinTypeConverter::_KShort_shortValue {nullptr};

jmethodID Bn3Monkey::Kotlin::KotlinTypeConverter::_KInt_init {nullptr};
jmethodID Bn3Monkey::Kotlin::KotlinTypeConverter::_KInt_intValue {nullptr};

jmethodID Bn3Monkey::Kotlin::KotlinTypeConverter::_KLong_init {nullptr};
jmethodID Bn3Monkey::Kotlin::KotlinTypeConverter::_KLong_longValue {nullptr};

jmethodID Bn3Monkey::Kotlin::KotlinTypeConverter::_KFloat_init {nullptr};
jmethodID Bn3Monkey::Kotlin::KotlinTypeConverter::_KFloat_floatValue {nullptr};

jmethodID Bn3Monkey::Kotlin::KotlinTypeConverter::_KDouble_init {nullptr};
jmethodID Bn3Monkey::Kotlin::KotlinTypeConverter::_KDouble_doubleValue {nullptr};

jmethodID Bn3Monkey::Kotlin::KotlinTypeConverter::_KVector_init {nullptr};
jmethodID Bn3Monkey::Kotlin::KotlinTypeConverter::_KVector_size {nullptr};
jmethodID Bn3Monkey::Kotlin::KotlinTypeConverter::_KVector_add {nullptr};
jmethodID Bn3Monkey::Kotlin::KotlinTypeConverter::_KVector_get {nullptr};

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
    do {
        auto ret = env->GetJavaVM(&jvm);
        if (jvm == nullptr) {
            LOG_E("%s : Cannot get JVM", __FUNCTION__);
            break;
        }

        _KBoolean = newGlobalClass(env, "java/lang/Boolean");
        if (_KBoolean == nullptr) {
            LOG_E("%s : Boolean Class cannot be loaded", __FUNCTION__);
            break;
        }
        _KBoolean_init = newGlobalMethodID(env, _KBoolean, "<init>", "(Z)V");
        if (_KBoolean_init == nullptr) {
            LOG_E("%s : Boolean Class / init method cannot be loaded", __FUNCTION__);
            break;
        }
        _KBoolean_booleanValue = newGlobalMethodID(env, _KBoolean, "booleanValue", "()Z");
        if (_KBoolean_booleanValue == nullptr) {
            LOG_E("%s : Boolean Class / booleanValue method cannot be loaded", __FUNCTION__);
            break;
        }

        _KByte = newGlobalClass(env, "java/lang/Byte");
        if (_KByte == nullptr) {
            LOG_E("%s : Byte Class cannot be loaded", __FUNCTION__);
            break;
        }
        _KByte_init = newGlobalMethodID(env, _KByte, "<init>", "(B)V");
        if (_KByte_init == nullptr) {
            LOG_E("%s : Byte Class / init method cannot be loaded", __FUNCTION__);
            break;
        }
        _KByte_byteValue = newGlobalMethodID(env, _KByte, "byteValue", "()B");
        if (_KByte_byteValue == nullptr) {
            LOG_E("%s : Byte Class / byteValue method cannot be loaded", __FUNCTION__);
            break;
        }

        _KShort = newGlobalClass(env, "java/lang/Short");
        if (_KShort == nullptr) {
            LOG_E("%s : Short Class cannot be loaded", __FUNCTION__);
            break;
        }
        _KShort_init = newGlobalMethodID(env, _KShort, "<init>", "(S)V");
        if (_KShort_init == nullptr) {
            LOG_E("%s : Short Class / init method cannot be loaded", __FUNCTION__);
            break;
        }
        _KShort_shortValue = newGlobalMethodID(env, _KShort, "shortValue", "()S");
        if (_KShort_shortValue == nullptr) {
            LOG_E("%s : Short Class / shortValue method cannot be loaded", __FUNCTION__);
            break;
        }

        _KInt = newGlobalClass(env, "java/lang/Integer");
        if (_KInt == nullptr) {
            LOG_E("%s : Int Class cannot be loaded", __FUNCTION__);
            break;
        }
        _KInt_init = newGlobalMethodID(env, _KInt, "<init>", "(I)V");
        if (_KInt_init == nullptr) {
            LOG_E("%s : Int Class / init method cannot be loaded", __FUNCTION__);
            break;
        }
        _KInt_intValue = newGlobalMethodID(env, _KInt, "intValue", "()I");
        if (_KInt_intValue == nullptr) {
            LOG_E("%s : Int Class / intValue method cannot be loaded", __FUNCTION__);
            break;
        }

        _KLong = newGlobalClass(env, "java/lang/Long");
        if (_KLong == nullptr) {
            LOG_E("%s : Long Class cannot be loaded", __FUNCTION__);
            break;
        }
        _KLong_init = newGlobalMethodID(env, _KLong, "<init>", "(J)V");
        if (_KLong_init == nullptr) {
            LOG_E("%s : Long Class / init method cannot be loaded", __FUNCTION__);
            break;
        }
        _KLong_longValue = newGlobalMethodID(env, _KLong, "longValue", "()J");
        if (_KLong_longValue == nullptr) {
            LOG_E("%s : Long Class / longValue method cannot be loaded", __FUNCTION__);
            break;
        }

        _KFloat = newGlobalClass(env, "java/lang/Float");
        if (_KFloat == nullptr) {
            LOG_E("%s : Float Class cannot be loaded", __FUNCTION__);
            break;
        }
        _KFloat_init = newGlobalMethodID(env, _KFloat, "<init>", "(F)V");
        if (_KFloat_init == nullptr) {
            LOG_E("%s : Float Class / init method cannot be loaded", __FUNCTION__);
            break;
        }
        _KFloat_floatValue = newGlobalMethodID(env, _KFloat, "floatValue", "()F");
        if (_KFloat_floatValue == nullptr) {
            LOG_E("%s : Float Class / floatValue method cannot be loaded", __FUNCTION__);
            break;
        }

        _KDouble = newGlobalClass(env, "java/lang/Double");
        if (_KDouble == nullptr) {
            LOG_E("%s : Double Class cannot be loaded", __FUNCTION__);
            break;
        }
        _KDouble_init = newGlobalMethodID(env, _KDouble, "<init>", "(D)V");
        if (_KDouble_init == nullptr) {
            LOG_E("%s : Double Class / init method cannot be loaded", __FUNCTION__);
            break;
        }
        _KDouble_doubleValue = newGlobalMethodID(env, _KDouble, "doubleValue", "()D");
        if (_KDouble_doubleValue == nullptr) {
            LOG_E("%s : Double Class / doubleValue method cannot be loaded", __FUNCTION__);
            break;
        }

        _KVector = newGlobalClass(env, "java/util/ArrayList");
        if (_KVector == nullptr)
        {
            LOG_E("%s : Array List Class cannot be loaded", __FUNCTION__);
            break;
        }
        _KVector_init = newGlobalMethodID(env, _KVector, "<init>", "()V");
        if (_KVector_init == nullptr) {
            LOG_E("%s : Array List Class / init method cannot be loaded", __FUNCTION__);
            break;
        }
        _KVector_add = newGlobalMethodID(env, _KVector, "add", "(Ljava/lang/Object;)Z");
        if (_KVector_add == nullptr) {
            LOG_E("%s : Array List Class / add method cannot be loaded", __FUNCTION__);
            break;
        }
        _KVector_size = newGlobalMethodID(env, _KVector, "size", "()I");
        if (_KVector_size == nullptr) {
            LOG_E("%s : Array List Class / size method cannot be loaded", __FUNCTION__);
            break;
        }
        _KVector_get = newGlobalMethodID(env, _KVector, "get", "(I)Ljava/lang/Object;");
        if (_KVector_get == nullptr) {
            LOG_E("%s : Array List Class / get method cannot be loaded", __FUNCTION__);
            break;
        }

        _KString = newGlobalClass(env, "java/lang/String");
        if (_KString == nullptr) {
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
    deleteClass(env, _KBoolean);
    deleteClass(env, _KByte);
    deleteClass(env, _KShort);
    deleteClass(env, _KInt);
    deleteClass(env, _KLong);
    deleteClass(env, _KFloat);
    deleteClass(env, _KDouble);
    deleteClass(env, _KVector);
    deleteClass(env, _KString);

    /*
    deleteMethod(env, _KBoolean_init);
    deleteMethod(env, _KBoolean_booleanValue);
    deleteMethod(env, _KByte_init);
    deleteMethod(env, _KByte_byteValue);
    deleteMethod(env, _KShort_init);
    deleteMethod(env, _KShort_shortValue);
    deleteMethod(env, _KInt_init);
    deleteMethod(env, _KInt_intValue);
    deleteMethod(env, _KLong_init);
    deleteMethod(env, _KLong_longValue);
    deleteMethod(env, _KFloat_init);
    deleteMethod(env, _KFloat_floatValue);
    deleteMethod(env, _KDouble_init);
    deleteMethod(env, _KDouble_doubleValue);
    deleteMethod(env, _KVector_init);
    deleteMethod(env, _KVector_add);
    deleteMethod(env, _KVector_get);
    deleteMethod(env, _KVector_size);
     */
}
