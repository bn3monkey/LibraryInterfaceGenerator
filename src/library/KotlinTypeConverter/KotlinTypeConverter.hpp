#ifndef __BN3MONKEY_KOTLIN_TYPE_CONVERTER__
#define __BN3MONKEY_KOTLIN_TYPE_CONVERTER__

#include <jni.h>
#include <vector>
#include <functional>
#include <algorithm>
#include <cstdio>
#include <cassert>
#include <string>
#include <array>

#include "../Log/Log.hpp"

namespace Bn3Monkey
{
    namespace Kotlin
    {
        class KotlinTypeConverter {
        public:
            static bool initialize(JNIEnv* env);
            static void release(JNIEnv* env);
            virtual const char* signature() = 0;

            struct Helper {
                jclass _KBoolean;
                jclass _KByte;
                jclass _KShort;
                jclass _KInt;
                jclass _KLong;
                jclass _KFloat;
                jclass _KDouble;
                jclass _KString;
                jclass _KVector; // arraylist

                jmethodID _KBoolean_init;
                jmethodID _KBoolean_booleanValue;

                jmethodID _KByte_init;
                jmethodID _KByte_byteValue;

                jmethodID _KShort_init;
                jmethodID _KShort_shortValue;

                jmethodID _KInt_init;
                jmethodID _KInt_intValue;

                jmethodID _KLong_init;
                jmethodID _KLong_longValue;

                jmethodID _KFloat_init;
                jmethodID _KFloat_floatValue;

                jmethodID _KDouble_init;
                jmethodID _KDouble_doubleValue;

                jmethodID _KVector_init;
                jmethodID _KVector_size;
                jmethodID _KVector_add;
                jmethodID _KVector_get;
                jmethodID _KVector_clear;
            };
            
        protected:
            static bool isInitialized;
            static JavaVM* jvm;


            static Helper helper;
        };

        class KVoid : public KotlinTypeConverter
        {
        public:
            using ManagedType = void;
            using KotlinType = void;
            using KotlinWrapperType = void;
            const char* signature() override { return "V"; }
        };

        class KBoolean : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "Z"; }

            using ManagedType = bool;
            using KotlinType = jboolean;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                return static_cast<ManagedType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return env->CallBooleanMethod(value, helper._KBoolean_booleanValue);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return env->NewObject(helper._KBoolean, helper._KBoolean_init, value);
            }
        private:    
        };
        class KInt8 : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "B"; }

            using ManagedType = int8_t;
            using KotlinType = jbyte;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                return static_cast<ManagedType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return env->CallByteMethod(value, helper._KByte_byteValue);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return env->NewObject(helper._KByte, helper._KByte_init, value);
            }
        };
        class KInt16 : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "S"; }

            using ManagedType = int16_t;
            using KotlinType = jshort;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                return static_cast<ManagedType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return env->CallShortMethod(value, helper._KShort_shortValue);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return env->NewObject(helper._KShort, helper._KShort_init, value);
            }
        };
        class KInt32 : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "I"; }

            using ManagedType = int32_t;
            using KotlinType = jint;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                return static_cast<ManagedType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return env->CallIntMethod(value, helper._KInt_intValue);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return env->NewObject(helper._KInt, helper._KInt_init, value);
            }
        };
        class KInt64 : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "J"; }

            using ManagedType = int64_t;
            using KotlinType = jlong;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                return static_cast<ManagedType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return env->CallLongMethod(value, helper._KLong_longValue);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return env->NewObject(helper._KLong, helper._KLong_init, value);
            }
        };
        class KFloat : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "F"; }

            using ManagedType = float;
            using KotlinType = jfloat;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                return static_cast<ManagedType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return env->CallFloatMethod(value, helper._KFloat_floatValue);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return env->NewObject(helper._KFloat, helper._KFloat_init, value);
            }
        };
        class KDouble : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "D"; }

            using ManagedType = double;
            using KotlinType = jdouble;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                return static_cast<ManagedType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return env->CallDoubleMethod(value, helper._KDouble_doubleValue);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return env->NewObject(helper._KDouble, helper._KDouble_init, value);
            }
        };
        class KString : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "Ljava/lang/String;"; }

            using ManagedType = std::string;
            using KotlinType = jstring;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                const char* ret = env->GetStringUTFChars(value, nullptr);
                return ManagedType(ret);
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                return env->NewStringUTF(value.c_str());
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return reinterpret_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return reinterpret_cast<KotlinWrapperType>(value);
            }
        };

        // class Enum; (생성 규칙에 의해 만들어짐)
        class KEnum : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "I"; }

            using ManagedType = int32_t;
            using KotlinType = jint;
            // Kotlin.Int
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                return static_cast<ManagedType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return env->CallIntMethod(value, helper._KInt_intValue);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return env->NewObject(helper._KInt, helper._KInt_init, value);
            }
        };


        // class Object; (생성 규칙에 의해 만들어짐)
        class KObject : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "J"; }

            using ManagedType = void*;
            using KotlinType = jlong;
            // Kotlin.Long
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                return reinterpret_cast<ManagedType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                return reinterpret_cast<jlong>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return env->CallLongMethod(value, helper._KLong_longValue);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return env->NewObject(helper._KLong, helper._KLong_init, value);
            }
        };

        template<class KotlinReturnType, class... KotlinArgumentType>
        class KCallback : public KotlinTypeConverter
        {
        public:
            const char* signature() override
            {
                assert("Kotlin Callback cannot be used as a Callback, Array, or Vector argument.");
                return nullptr;
            }

            static_assert(std::is_base_of<KotlinTypeConverter, KotlinReturnType>::value, "KotlinReturnType must be derived from KotlinTypeConverter");
            static_assert(std::conjunction<std::is_base_of<KotlinTypeConverter, KotlinArgumentType>...>::value, "All KotlinArgumentType must be derived from KotlinTypeConverter");

            using ManagedType = std::function<typename KotlinReturnType::ManagedType(typename KotlinArgumentType::ManagedType...)>;
            using KotlinType = jobject;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value)
            {
                auto jcallback = env->NewGlobalRef(value);

                return [&, jcallback](typename KotlinArgumentType::ManagedType... args) -> typename KotlinReturnType::ManagedType
                {
                    size_t arg_size = sizeof...(KotlinArgumentType);
                    size_t index {1};
                    const char* sig = "Ljava/lang/Object;";
                    size_t sig_size = strlen(sig);
                    char invoke_signature[256]{ 0 };
                    invoke_signature[0] = '(';
                    for (size_t i =0;i<arg_size;i++)
                    {
                        std::strcpy(invoke_signature + index, sig);
                        index += sig_size;
                    }
                    invoke_signature[index++] = ')';
                    std::strcpy(invoke_signature + index, sig);

                    bool load_error {false};

                    JNIEnv* env {nullptr};
                    int getEnvStat = jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
                    if (getEnvStat == JNI_EDETACHED) {
                        if (jvm->AttachCurrentThread(&env, NULL) != 0) {
                            load_error= true;
                        }
                    }
                    else if (getEnvStat == JNI_EVERSION) {
                        load_error = true;
                    }
                    else if (getEnvStat == JNI_OK) {
                    }

                    if (load_error) {
                        if constexpr (std::is_same_v<KotlinReturnType, KVoid>) {
                            return;
                        } else {
                            return typename KotlinReturnType::ManagedType();
                        }
                    }

                    jclass clazz = env->GetObjectClass(jcallback);
                    jmethodID callback = env->GetMethodID(clazz, "invoke", invoke_signature);


                    if constexpr(std::is_same_v<KotlinReturnType, KVoid>)
                    {
                        env->CallObjectMethod(jcallback, callback,
                                            KotlinArgumentType().toKotlinWrapperType(env, KotlinArgumentType().toKotlinType(env, args))...);
                        if (getEnvStat == JNI_EDETACHED)
                            jvm->DetachCurrentThread();
                        return;
                    }
                    else
                    {
                        typename KotlinReturnType::ManagedType ret;

                        auto _ret = env->CallObjectMethod(jcallback, callback, KotlinArgumentType().toKotlinWrapperType(env, KotlinArgumentType().toKotlinType(env, args))...);
                        ret = KotlinReturnType().toManagedType(env, KotlinReturnType().toKotlinType(env, _ret));

                        if (getEnvStat == JNI_EDETACHED)
                            jvm->DetachCurrentThread();
                        return ret;
                    }

               };
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value)
            {
                assert("Managed Callback cannot be chanaged as Kotlin Callback.");
                return nullptr;
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return value;
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return value;
            }
        private:


        };

        template<typename KotlinElementType, size_t size, typename = void >
        class KArray : public KotlinTypeConverter
        {
        public:
            virtual const char* signature() = 0;
        };

        template<size_t size>
        class KArray<KBoolean, size> : public KotlinTypeConverter
        {
        public:
            using ManagedType = std::array<bool, size>;
            using KotlinType = jbooleanArray;

            const char* signature() override { return "[Z"; };

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto src = env->GetBooleanArrayElements(value, nullptr);
                size_t length = env->GetArrayLength(value);
                ManagedType ret;
                std::copy(src, src + length, ret.data());
                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewBooleanArray(value.size());
                auto* arr = env->GetBooleanArrayElements(ret, nullptr);
                if (!arr)
                    return nullptr;

                std::copy(value.begin(), value.end(), arr);
                env->ReleaseBooleanArrayElements(ret, arr, 0);
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                auto* arr = env->GetBooleanArrayElements(dest, nullptr);
                if (!arr)
                    return;

                std::copy(src.begin(), src.end(), arr);
                env->ReleaseBooleanArrayElements(dest, arr, 0);
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<jobject>(value);
            }
        };

        template<size_t size>
        class KArray<KInt8, size> : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "[B"; };

            using ManagedType = std::array<int8_t, size>;
            using KotlinType = jbyteArray;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto src = env->GetByteArrayElements(value, nullptr);
                size_t length = env->GetArrayLength(value);
                ManagedType ret;
                std::copy(src, src + length, ret.data());
                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewByteArray(value.size());
                auto* arr = env->GetByteArrayElements(ret, nullptr);
                if (!arr)
                    return nullptr;

                std::copy(value.begin(), value.end(), arr);
                env->ReleaseByteArrayElements(ret, arr, 0);
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                auto* arr = env->GetByteArrayElements(dest, nullptr);
                if (!arr)
                    return;

                std::copy(src.begin(), src.end(), arr);
                env->ReleaseByteArrayElements(dest, arr, 0);
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<jobject>(value);
            }
        };

        template<size_t size>
        class KArray<KInt16, size> : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "[S"; };

            using ManagedType = std::array<int16_t, size>;
            using KotlinType = jshortArray;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto src = env->GetShortArrayElements(value, nullptr);
                size_t length = env->GetArrayLength(value);
                ManagedType ret;
                std::copy(src, src + length, ret.data());
                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewShortArray(value.size());
                auto* arr = env->GetShortArrayElements(ret, nullptr);
                if (!arr)
                    return nullptr;

                std::copy(value.begin(), value.end(), arr);
                env->ReleaseShortArrayElements(ret, arr, 0);
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                auto* arr = env->GetShortArrayElements(dest, nullptr);
                if (!arr)
                    return;

                std::copy(src.begin(), src.end(), arr);
                env->ReleaseShortArrayElements(dest, arr, 0);
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<jobject>(value);
            }
        };

        template<size_t size>
        class KArray<KInt32, size> : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "[I"; };

            using ManagedType = std::array<int32_t, size>;
            using KotlinType = jintArray;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto src = env->GetIntArrayElements(value, nullptr);
                size_t length = env->GetArrayLength(value);
                ManagedType ret;
                std::copy(src, src + length, ret.data());
                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewIntArray(value.size());
                auto* arr = env->GetIntArrayElements(ret, nullptr);
                if (!arr)
                    return nullptr;

                std::copy(value.begin(), value.end(), arr);
                env->ReleaseIntArrayElements(ret, arr, 0);
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                auto* arr = env->GetIntArrayElements(dest, nullptr);
                if (!arr)
                    return;

                std::copy(src.begin(), src.end(), arr);
                env->ReleaseIntArrayElements(dest, arr, 0);
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<jobject>(value);
            }
        };

        template<size_t size>
        class KArray<KInt64, size> : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "[J"; };

            using ManagedType = std::array<int64_t, size>;
            using KotlinType = jlongArray;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto src = env->GetLongArrayElements(value, nullptr);
                size_t length = env->GetArrayLength(value);
                ManagedType ret;
                std::copy(src, src + length, ret.data());
                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewLongArray(value.size());
                auto* arr = env->GetLongArrayElements(ret, nullptr);
                if (!arr)
                    return nullptr;

                std::copy(value.begin(), value.end(), arr);
                env->ReleaseLongArrayElements(ret, arr, 0);
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                auto* arr = env->GetLongArrayElements(dest, nullptr);
                if (!arr)
                    return;

                std::copy(src.begin(), src.end(), arr);
                env->ReleaseLongArrayElements(dest, arr, 0);
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<jobject>(value);
            }
        };

        template<size_t size>
        class KArray<KFloat, size> : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "[F"; };

            using ManagedType = std::array<float, size>;
            using KotlinType = jfloatArray;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto src = env->GetFloatArrayElements(value, nullptr);
                size_t length = env->GetArrayLength(value);
                ManagedType ret;
                std::copy(src, src + length, ret.data());
                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewFloatArray(value.size());
                auto* arr = env->GetFloatArrayElements(ret, nullptr);
                if (!arr)
                    return nullptr;

                std::copy(value.begin(), value.end(), arr);
                env->ReleaseFloatArrayElements(ret, arr, 0);
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                auto* arr = env->GetFloatArrayElements(dest, nullptr);
                if (!arr)
                    return;

                std::copy(src.begin(), src.end(), arr);
                env->ReleaseFloatArrayElements(dest, arr, 0);
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<jobject>(value);
            }
        };

        template<size_t size>
        class KArray<KDouble, size> : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "[D"; };

            using ManagedType = std::array<double, size>;
            using KotlinType = jdoubleArray;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto src = env->GetDoubleArrayElements(value, nullptr);
                size_t length = env->GetArrayLength(value);
                ManagedType ret;
                std::copy(src, src + length, ret.data());
                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewDoubleArray(value.size());
                auto* arr = env->GetDoubleArrayElements(ret, nullptr);
                if (!arr)
                    return nullptr;

                std::copy(value.begin(), value.end(), arr);
                env->ReleaseDoubleArrayElements(ret, arr, 0);
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                auto* arr = env->GetDoubleArrayElements(dest, nullptr);
                if (!arr)
                    return;

                std::copy(src.begin(), src.end(), arr);
                env->ReleaseDoubleArrayElements(dest, arr, 0);
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<jobject>(value);
            }
        };

        template<size_t size>
        class KArray<KString, size> : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "[Ljava/lang/String;"; };

            using ManagedType = std::array<std::string, size>;
            using KotlinType = jobjectArray;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                size_t length = env->GetArrayLength(value);

                ManagedType ret;
                for (size_t i = 0; i < length; i++)
                {
                    auto element = env->GetObjectArrayElement(value, i);
                    auto new_element = KString().toManagedType(env, (jstring)element);
                    ret[i] = new_element;
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObjectArray(value.size(), helper._KString, nullptr);
                for (size_t i =0;i<value.size();i++)
                {
                    auto element = value[i];
                    auto new_element = KString().toKotlinType(env, element);
                    env->SetObjectArrayElement(ret, i, new_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                for (size_t i =0;i<src.size();i++)
                {
                    auto element = src[i];
                    auto new_element = KString().toKotlinType(env, element);
                    env->SetObjectArrayElement(dest, i, new_element);
                }
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<jobject>(value);
            }
        };

        template<size_t size>
        class KArray<KEnum, size> : public KotlinTypeConverter
        {
        public:
            const char* signature() override {
                return "[I";
            }

            using ManagedType = std::array<int32_t, size>;
            using KotlinType = jintArray;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto src = env->GetIntArrayElements(value, nullptr);
                size_t length = env->GetArrayLength(value);
                ManagedType ret;
                std::copy(src, src + length, ret.data());
                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewIntArray(value.size());
                auto* arr = env->GetIntArrayElements(ret, nullptr);
                if (!arr)
                    return nullptr;

                std::copy(value.begin(), value.end(), arr);
                env->ReleaseIntArrayElements(ret, arr, 0);
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                auto* arr = env->GetIntArrayElements(dest, nullptr);
                if (!arr)
                    return;

                std::copy(src.begin(), src.end(), arr);
                env->ReleaseIntArrayElements(dest, arr, 0);
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<jobject>(value);
            }
        };
        
        template<typename KObject, size_t size>
        class KArray<KObject, size> : public KotlinTypeConverter
        {
        public:
            const char* signature() override {
                return "[J";
            }

            using ManagedType = std::array<void*, size>;
            using KotlinType = jlongArray;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto src = env->GetLongArrayElements(value, nullptr);
                size_t length = env->GetArrayLength(value);
                ManagedType ret;

                for (size_t i=0;i<length;i++)
                    ret[i]= KObject().toManagedType(env, src[i]);

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewLongArray(value.size());
                auto* arr = env->GetLongArrayElements(ret, nullptr);
                if (!arr)
                    return nullptr;

                for (size_t i =0; i <value.size(); i++)
                    arr[i] = KObject().toKotlinType(env, value[i]);

                env->ReleaseLongArrayElements(ret, arr, 0);
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest) {
                auto *arr = env->GetLongArrayElements(dest, nullptr);
                if (!arr)
                    return;

                for (size_t i = 0; i < src.size(); i++) {
                    arr[i] = KObject().toKotlinType(env, src[i]);
                }

                env->ReleaseLongArrayElements(dest, arr, 0);
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<jobject>(value);
            }
        };

        template<size_t size, class KotlinReturnType, class... KotlinArgumentType>
        class KArray<KCallback<KotlinReturnType, KotlinArgumentType...>, size> : public KotlinTypeConverter
        {
        public:

            using Callback = KCallback<KotlinReturnType, KotlinArgumentType...>;
            using ManagedType = std::vector<void*>;
            using KotlinType = jobjectArray;

            const char* signature() override {
                assert("Kotlin Callback cannot be used as a Callback, Array, or Vector argument.");
                return nullptr;
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                size_t length = env->GetArrayLength(value);

                ManagedType ret;
                ret.reserve(length);
                for (size_t i = 0; i < length; i++)
                {
                    auto element = env->GetObjectArrayElement(value, i);
                    auto new_element = Callback().toManagedType(element);
                    ret.push_back(new_element);
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                auto clazz = env->FindClass(Callback().className());
                KotlinType ret = env->NewObjectArray(value.size(), clazz, nullptr);
                for (size_t i =0;i<value.size();i++)
                {
                    auto element = value[i];
                    auto new_element = Callback().toKotlinType(element);
                    auto* arr = env->SetObjectArrayElement(ret, i, new_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                for (size_t i =0;i<src.size();i++)
                {
                    auto element = src[i];
                    auto new_element = Callback().toKotlinType(element);
                    auto* arr = env->SetObjectArrayElement(dest, i, new_element);
                }
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<KotlinWrapperType>(value);
            }
        };


        template<class KotlinElementType, typename = void>
        class KVector : public KotlinTypeConverter
        {
            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }
        };

        template<>
        struct KVector<KBoolean> : public KotlinTypeConverter
        {
            using ManagedType = std::vector<bool>;
            using KotlinType = jobject;
            using KotlinElementType = KBoolean;
            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }
            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, helper._KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, helper._KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(helper._KVector, helper._KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, helper._KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, helper._KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, helper._KVector_add, ko_element);
                }
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<KotlinWrapperType>(value);
            }
        };

        template<>
        struct KVector<KInt8> : public KotlinTypeConverter
        {
            using ManagedType = std::vector<int8_t>;
            using KotlinType = jobject;
            using KotlinElementType = KInt8;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, helper._KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, helper._KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(helper._KVector, helper._KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, helper._KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, helper._KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, helper._KVector_add, ko_element);
                }
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<KotlinWrapperType>(value);
            }
        };

        template<>
        struct KVector<KInt16> : public KotlinTypeConverter
        {
            using ManagedType = std::vector<int16_t>;
            using KotlinType = jobject;
            using KotlinElementType = KInt16;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, helper._KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, helper._KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(helper._KVector, helper._KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, helper._KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, helper._KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, helper._KVector_add, ko_element);
                }
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<KotlinWrapperType>(value);
            }
        };

        template<>
        struct KVector<KInt32> : public KotlinTypeConverter
        {
            using ManagedType = std::vector<int32_t>;
            using KotlinType = jobject;
            using KotlinElementType = KInt32;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, helper._KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, helper._KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(helper._KVector, helper._KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, helper._KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, helper._KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, helper._KVector_add, ko_element);
                }
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<KotlinWrapperType>(value);
            }
        };

        template<>
        struct KVector<KInt64> : public KotlinTypeConverter
        {
            using ManagedType = std::vector<int64_t>;
            using KotlinType = jobject;
            using KotlinElementType = KInt64;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, helper._KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, helper._KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(helper._KVector, helper._KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, helper._KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, helper._KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, helper._KVector_add, ko_element);
                }
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<KotlinWrapperType>(value);
            }
        };

        template<>
        struct KVector<KFloat> : public KotlinTypeConverter
        {
            using ManagedType = std::vector<float>;
            using KotlinType = jobject;
            using KotlinElementType = KFloat;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, helper._KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, helper._KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(helper._KVector, helper._KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, helper._KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, helper._KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, helper._KVector_add, ko_element);
                }
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<KotlinWrapperType>(value);
            }
        };

        template<>
        struct KVector<KDouble> : public KotlinTypeConverter
        {
            using ManagedType = std::vector<double>;
            using KotlinType = jobject;
            using KotlinElementType = KDouble;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, helper._KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, helper._KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(helper._KVector, helper._KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, helper._KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, helper._KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, helper._KVector_add, ko_element);
                }
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<KotlinWrapperType>(value);
            }
        };

        template<>
        struct KVector<KString> : public KotlinTypeConverter
        {
            using NativeType = std::vector<std::string>;
            using ManagedType = NativeType;
            using KotlinType = jobject;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, helper._KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, helper._KVector_get, i);
                    auto k_element = KString().toKotlinType(env, ko_element);
                    ret.push_back(KString().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(helper._KVector, helper._KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KString().toKotlinType(env, element);
                    auto ko_element = KString().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, helper._KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, helper._KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KString().toKotlinType(env, element);
                    auto ko_element = KString().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, helper._KVector_add, ko_element);
                }
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<KotlinWrapperType>(value);
            }
        };

        template<typename KDerivedEnum>
        struct KVector<KDerivedEnum, typename std::enable_if<std::is_base_of<KEnum, KDerivedEnum>::value>::type> : public KotlinTypeConverter
        {
            using ManagedType = std::vector<int32_t>;
            using KotlinType = jobject;
            using KotlinElementType = KDerivedEnum;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, helper._KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, helper._KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(helper._KVector, helper._KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, helper._KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, helper._KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, helper._KVector_add, ko_element);
                }
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<KotlinWrapperType>(value);
            }
        };

        template<typename KDerivedObject>
        struct KVector<KDerivedObject, typename std::enable_if<std::is_base_of<KObject, KDerivedObject>::value>::type> : public KotlinTypeConverter
        {
            using ManagedType = std::vector<void*>;
            using KotlinType = jobject;
            using KotlinElementType = KDerivedObject;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, helper._KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, helper._KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(helper._KVector, helper._KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, helper._KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, helper._KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, helper._KVector_add, ko_element);
                }
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<KotlinWrapperType>(value);
            }
        };

        template<class KotlinReturnType, class... KotlinArgumentType>
        struct KVector<KCallback<KotlinReturnType, KotlinArgumentType...>> : public KotlinTypeConverter
        {
            using ManagedType = std::vector<void*>;
            using KotlinType = jobject;

            using KotlinElementType = KCallback<KotlinReturnType, KotlinArgumentType...>;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, helper._KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, helper._KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(helper._KVector, helper._KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, helper._KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, helper._KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, helper._KVector_add, ko_element);
                }
            }

            using KotlinWrapperType = jobject;
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return static_cast<KotlinWrapperType>(value);
            }
        };

    }
}

#endif
