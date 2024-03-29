#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_KOTLINTYPECONVERTER_CPP__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_KOTLINTYPECONVERTER_CPP__
static constexpr char* KOTLINTYPECONVERTER_CPP = 
"#include \"KotlinTypeConverter.hpp\"\n"
"\n"
"#define BN3MONKEY_DEBUG\n"
"#ifdef BN3MONKEY_DEBUG\n"
"#define FOR_DEBUG(t) t\n"
"#else\n"
"#define FOR_DEBUG(t)\n"
"#endif\n"
"\n"
"#ifdef __BN3MONKEY_LOG__\n"
"#ifdef BN3MONKEY_DEBUG\n"
"#define LOG_D(text, ...) Bn3Monkey::Log::D(__FUNCTION__, text, ##__VA_ARGS__)\n"
"#else\n"
"#define LOG_D(text, ...)\n"
"#endif\n"
"#define LOG_V(text, ...) Bn3Monkey::Log::V(__FUNCTION__, text, ##__VA_ARGS__)\n"
"#define LOG_E(text, ...) Bn3Monkey::Log::E(__FUNCTION__, text, ##__VA_ARGS__)\n"
"#else\n"
"#define LOG_D(text, ...)\n"
"#define LOG_V(text, ...)\n"
"#define LOG_E(text, ...)\n"
"#endif\n"
"Bn3Monkey::Kotlin::KotlinTypeConverter::Helper Bn3Monkey::Kotlin::KotlinTypeConverter::helper {nullptr};\n"
"\n"
"bool Bn3Monkey::Kotlin::KotlinTypeConverter::isInitialized {false};\n"
"JavaVM* Bn3Monkey::Kotlin::KotlinTypeConverter::jvm;\n"
"\n"
"static inline jclass newGlobalClass(JNIEnv* env, const char* class_name)\n"
"{\n"
"    jclass clazz = env->FindClass(class_name);\n"
"    return reinterpret_cast<jclass>(env->NewGlobalRef(reinterpret_cast<jobject>(clazz)));\n"
"}\n"
"static inline jmethodID newGlobalMethodID(JNIEnv* env, jclass clazz, const char* method_name, const char* sig)\n"
"{\n"
"    jmethodID methodID = env->GetMethodID(clazz, method_name, sig);\n"
"    //return reinterpret_cast<jmethodID>(env->NewGlobalRef(reinterpret_cast<jobject>(methodID)));\n"
"    return methodID;\n"
"}\n"
"\n"
"bool Bn3Monkey::Kotlin::KotlinTypeConverter::initialize(JNIEnv* env)\n"
"{\n"
"    isInitialized = false;\n"
"\n"
"    do {\n"
"        auto ret = env->GetJavaVM(&jvm);\n"
"        if (jvm == nullptr) {\n"
"            LOG_E(\"%s : Cannot get JVM\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"\n"
"        helper._KBoolean = newGlobalClass(env, \"java/lang/Boolean\");\n"
"        if (helper._KBoolean == nullptr) {\n"
"            LOG_E(\"%s : Boolean Class cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KBoolean_init = newGlobalMethodID(env, helper._KBoolean, \"<init>\", \"(Z)V\");\n"
"        if (helper._KBoolean_init == nullptr) {\n"
"            LOG_E(\"%s : Boolean Class / init method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KBoolean_booleanValue = newGlobalMethodID(env, helper._KBoolean, \"booleanValue\", \"()Z\");\n"
"        if (helper._KBoolean_booleanValue == nullptr) {\n"
"            LOG_E(\"%s : Boolean Class / booleanValue method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"\n"
"        helper._KByte = newGlobalClass(env, \"java/lang/Byte\");\n"
"        if (helper._KByte == nullptr) {\n"
"            LOG_E(\"%s : Byte Class cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KByte_init = newGlobalMethodID(env, helper._KByte, \"<init>\", \"(B)V\");\n"
"        if (helper._KByte_init == nullptr) {\n"
"            LOG_E(\"%s : Byte Class / init method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KByte_byteValue = newGlobalMethodID(env, helper._KByte, \"byteValue\", \"()B\");\n"
"        if (helper._KByte_byteValue == nullptr) {\n"
"            LOG_E(\"%s : Byte Class / byteValue method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"\n"
"        helper._KShort = newGlobalClass(env, \"java/lang/Short\");\n"
"        if (helper._KShort == nullptr) {\n"
"            LOG_E(\"%s : Short Class cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KShort_init = newGlobalMethodID(env, helper._KShort, \"<init>\", \"(S)V\");\n"
"        if (helper._KShort_init == nullptr) {\n"
"            LOG_E(\"%s : Short Class / init method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KShort_shortValue = newGlobalMethodID(env, helper._KShort, \"shortValue\", \"()S\");\n"
"        if (helper._KShort_shortValue == nullptr) {\n"
"            LOG_E(\"%s : Short Class / shortValue method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"\n"
"        helper._KInt = newGlobalClass(env, \"java/lang/Integer\");\n"
"        if (helper._KInt == nullptr) {\n"
"            LOG_E(\"%s : Int Class cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KInt_init = newGlobalMethodID(env, helper._KInt, \"<init>\", \"(I)V\");\n"
"        if (helper._KInt_init == nullptr) {\n"
"            LOG_E(\"%s : Int Class / init method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KInt_intValue = newGlobalMethodID(env, helper._KInt, \"intValue\", \"()I\");\n"
"        if (helper._KInt_intValue == nullptr) {\n"
"            LOG_E(\"%s : Int Class / intValue method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"\n"
"        helper._KLong = newGlobalClass(env, \"java/lang/Long\");\n"
"        if (helper._KLong == nullptr) {\n"
"            LOG_E(\"%s : Long Class cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KLong_init = newGlobalMethodID(env, helper._KLong, \"<init>\", \"(J)V\");\n"
"        if (helper._KLong_init == nullptr) {\n"
"            LOG_E(\"%s : Long Class / init method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KLong_longValue = newGlobalMethodID(env, helper._KLong, \"longValue\", \"()J\");\n"
"        if (helper._KLong_longValue == nullptr) {\n"
"            LOG_E(\"%s : Long Class / longValue method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"\n"
"        helper._KFloat = newGlobalClass(env, \"java/lang/Float\");\n"
"        if (helper._KFloat == nullptr) {\n"
"            LOG_E(\"%s : Float Class cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KFloat_init = newGlobalMethodID(env, helper._KFloat, \"<init>\", \"(F)V\");\n"
"        if (helper._KFloat_init == nullptr) {\n"
"            LOG_E(\"%s : Float Class / init method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KFloat_floatValue = newGlobalMethodID(env, helper._KFloat, \"floatValue\", \"()F\");\n"
"        if (helper._KFloat_floatValue == nullptr) {\n"
"            LOG_E(\"%s : Float Class / floatValue method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"\n"
"        helper._KDouble = newGlobalClass(env, \"java/lang/Double\");\n"
"        if (helper._KDouble == nullptr) {\n"
"            LOG_E(\"%s : Double Class cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KDouble_init = newGlobalMethodID(env, helper._KDouble, \"<init>\", \"(D)V\");\n"
"        if (helper._KDouble_init == nullptr) {\n"
"            LOG_E(\"%s : Double Class / init method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KDouble_doubleValue = newGlobalMethodID(env, helper._KDouble, \"doubleValue\", \"()D\");\n"
"        if (helper._KDouble_doubleValue == nullptr) {\n"
"            LOG_E(\"%s : Double Class / doubleValue method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"\n"
"        helper._KVector = newGlobalClass(env, \"java/util/ArrayList\");\n"
"        if (helper._KVector == nullptr)\n"
"        {\n"
"            LOG_E(\"%s : Array List Class cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KVector_init = newGlobalMethodID(env, helper._KVector, \"<init>\", \"()V\");\n"
"        if (helper._KVector_init == nullptr) {\n"
"            LOG_E(\"%s : Array List Class / init method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KVector_add = newGlobalMethodID(env, helper._KVector, \"add\", \"(Ljava/lang/Object;)Z\");\n"
"        if (helper._KVector_add == nullptr) {\n"
"            LOG_E(\"%s : Array List Class / add method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KVector_size = newGlobalMethodID(env, helper._KVector, \"size\", \"()I\");\n"
"        if (helper._KVector_size == nullptr) {\n"
"            LOG_E(\"%s : Array List Class / size method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KVector_get = newGlobalMethodID(env, helper._KVector, \"get\", \"(I)Ljava/lang/Object;\");\n"
"        if (helper._KVector_get == nullptr) {\n"
"            LOG_E(\"%s : Array List Class / get method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"        helper._KVector_clear = newGlobalMethodID(env, helper._KVector, \"clear\", \"()V\");\n"
"        if (helper._KVector_clear == nullptr) {\n"
"            LOG_E(\"%s : Array List Class / clear method cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"\n"
"        helper._KString = newGlobalClass(env, \"java/lang/String\");\n"
"        if (helper._KString == nullptr) {\n"
"            LOG_E(\"%s : String class cannot be loaded\", __FUNCTION__);\n"
"            break;\n"
"        }\n"
"\n"
"        isInitialized = true;\n"
"    } while(false);\n"
"\n"
"    if (!isInitialized)\n"
"        release(env);\n"
"\n"
"    return isInitialized;\n"
"}\n"
"\n"
"static inline void deleteClass(JNIEnv* env, jclass& clazz)\n"
"{\n"
"    if (clazz)\n"
"    {\n"
"        env->DeleteGlobalRef(clazz);\n"
"        clazz = nullptr;\n"
"    }\n"
"}\n"
"/*\n"
"static inline void deleteMethod(JNIEnv* env, jmethodID& method)\n"
"{\n"
"    if (method)\n"
"    {\n"
"        env->DeleteGlobalRef(reinterpret_cast<jobject>(method));\n"
"        method = nullptr;\n"
"    }\n"
"}\n"
"*/\n"
"void Bn3Monkey::Kotlin::KotlinTypeConverter::release(JNIEnv* env)\n"
"{\n"
"    deleteClass(env, helper._KBoolean);\n"
"    deleteClass(env, helper._KByte);\n"
"    deleteClass(env, helper._KShort);\n"
"    deleteClass(env, helper._KInt);\n"
"    deleteClass(env, helper._KLong);\n"
"    deleteClass(env, helper._KFloat);\n"
"    deleteClass(env, helper._KDouble);\n"
"    deleteClass(env, helper._KVector);\n"
"    deleteClass(env, helper._KString);\n"
"\n"
"    /*\n"
"    deleteMethod(env, helper._KBoolean_init);\n"
"    deleteMethod(env, helper._KBoolean_booleanValue);\n"
"    deleteMethod(env, helper._KByte_init);\n"
"    deleteMethod(env, helper._KByte_byteValue);\n"
"    deleteMethod(env, helper._KShort_init);\n"
"    deleteMethod(env, helper._KShort_shortValue);\n"
"    deleteMethod(env, helper._KInt_init);\n"
"    deleteMethod(env, helper._KInt_intValue);\n"
"    deleteMethod(env, helper._KLong_init);\n"
"    deleteMethod(env, helper._KLong_longValue);\n"
"    deleteMethod(env, helper._KFloat_init);\n"
"    deleteMethod(env, helper._KFloat_floatValue);\n"
"    deleteMethod(env, helper._KDouble_init);\n"
"    deleteMethod(env, helper._KDouble_doubleValue);\n"
"    deleteMethod(env, helper._KVector_init);\n"
"    deleteMethod(env, helper._KVector_add);\n"
"    deleteMethod(env, helper._KVector_get);\n"
"    deleteMethod(env, helper._KVector_size);\n"
"     */\n"
"\n"
"}\n"
"\n";
#endif //__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_KOTLINTYPECONVERTER_CPP__
