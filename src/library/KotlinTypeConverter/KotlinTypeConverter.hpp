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
        protected:
            static bool isInitialized;
            static JavaVM* jvm;
            static jclass _KBoolean;
            static jclass _KByte;
            static jclass _KShort;
            static jclass _KInt;
            static jclass _KLong;
            static jclass _KFloat;
            static jclass _KDouble;
            static jclass _KString;
            static jclass _KVector; // arraylist

            static jmethodID _KBoolean_init;
            static jmethodID _KBoolean_booleanValue;

            static jmethodID _KByte_init;
            static jmethodID _KByte_byteValue;

            static jmethodID _KShort_init;
            static jmethodID _KShort_shortValue;

            static jmethodID _KInt_init;
            static jmethodID _KInt_intValue;

            static jmethodID _KLong_init;
            static jmethodID _KLong_longValue;

            static jmethodID _KFloat_init;
            static jmethodID _KFloat_floatValue;

            static jmethodID _KDouble_init;
            static jmethodID _KDouble_doubleValue;

            static jmethodID _KVector_init;
            static jmethodID _KVector_size;
            static jmethodID _KVector_add;
            static jmethodID _KVector_get;
            static jmethodID _KVector_clear;
        };

        class KVoid : public KotlinTypeConverter
        {
        public:
            using NativeType = void;
            using ManagedType = void;
            using KotlinType = void;
            using KotlinWrapperType = void;
            const char* signature() override { return "V"; }
        };

        class KBoolean : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "Z"; }

            using NativeType = bool;
            using ManagedType = NativeType;
            using KotlinType = jboolean;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                return static_cast<ManagedType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return env->CallBooleanMethod(value, _KBoolean_booleanValue);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return env->NewObject(_KBoolean, _KBoolean_init, value);
            }
        private:    
        };
        class KInt8 : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "B"; }

            using NativeType = int8_t;
            using ManagedType = NativeType;
            using KotlinType = jbyte;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                return static_cast<ManagedType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return env->CallByteMethod(value, _KByte_byteValue);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return env->NewObject(_KByte, _KByte_init, value);
            }
        };
        class KInt16 : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "S"; }

            using NativeType = int16_t;
            using ManagedType = NativeType;
            using KotlinType = jshort;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                return static_cast<ManagedType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return env->CallShortMethod(value, _KShort_shortValue);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return env->NewObject(_KShort, _KShort_init, value);
            }
        };
        class KInt32 : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "I"; }

            using NativeType = int32_t;
            using ManagedType = NativeType;
            using KotlinType = jint;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                return static_cast<ManagedType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return env->CallIntMethod(value, _KInt_intValue);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return env->NewObject(_KInt, _KInt_init, value);
            }
        };
        class KInt64 : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "J"; }

            using NativeType = int64_t;
            using ManagedType = NativeType;
            using KotlinType = jlong;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                return static_cast<ManagedType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return env->CallLongMethod(value, _KLong_longValue);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return env->NewObject(_KLong, _KLong_init, value);
            }
        };
        class KFloat : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "F"; }

            using NativeType = float;
            using ManagedType = NativeType;
            using KotlinType = jfloat;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                return static_cast<ManagedType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return env->CallFloatMethod(value, _KFloat_floatValue);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return env->NewObject(_KFloat, _KFloat_init, value);
            }
        };
        class KDouble : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "D"; }

            using NativeType = double;
            using ManagedType = NativeType;
            using KotlinType = jdouble;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                return static_cast<ManagedType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                return static_cast<KotlinType>(value);
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return env->CallDoubleMethod(value, _KDouble_doubleValue);
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return env->NewObject(_KDouble, _KDouble_init, value);
            }
        };
        class KString : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "Ljava/lang/String;"; }

            using NativeType = std::string;
            using ManagedType = NativeType;
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
            enum Dummy
            {
                A
            };
            virtual const char* className() = 0;
            virtual const char* signature() = 0;

            using NativeType = Dummy;
            using ManagedType = int32_t;
            using KotlinType = jobject;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto _Enum = env->FindClass(className());
                auto _Enum_Value = env->GetFieldID(_Enum, "value", "I");
                return static_cast<ManagedType>(env->GetIntField(value, _Enum_Value));
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                 auto _Enum = env->FindClass(className());

                 char sig[256]{ 0 };
                 sprintf(sig, "(I)%s", signature());

                 auto _Enum_toEnum = env->GetStaticMethodID(_Enum, "toEnum", sig);
                 return env->CallStaticObjectMethod(_Enum, _Enum_toEnum, static_cast<jint>(value));
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return value;
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return value;
            }
        };


        // class Object; (생성 규칙에 의해 만들어짐)
        class KObject : public KotlinTypeConverter
        {
        public:
            virtual const char* className() = 0;
            virtual const char* signature() = 0;

            using NativeType = KObject;
            using ManagedType = void*;
            using KotlinType = jobject;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto _Object = env->FindClass(className());
                auto _Object_nativeHandle = env->GetMethodID(_Object, "nativeHandle", "()J");
                return reinterpret_cast<ManagedType>(env->CallLongMethod(value, _Object_nativeHandle));
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                auto _Object = env->FindClass(className());
                auto _Object_init = env->GetMethodID(_Object, "<init>", "(J)V");
                return env->NewObject(_Object, _Object_init, reinterpret_cast<jlong>(value));
            }
            KotlinType toKotlinType(JNIEnv* env, const KotlinWrapperType& value) {
                return value;
            }
            KotlinWrapperType toKotlinWrapperType(JNIEnv* env, const KotlinType& value) {
                return value;
            }
        };

        template<class KotlinReturnType, class... KotlinArgumentType>
        class KCallback : public KotlinTypeConverter
        {
            const char* signature() override
            {
                assert("Kotlin Callback cannot be used as a Callback, Array, or Vector argument.");
                return nullptr;
            }

            static_assert(std::is_base_of<KotlinTypeConverter, KotlinReturnType>::value, "KotlinReturnType must be derived from KotlinTypeConverter");
            static_assert(std::conjunction<std::is_base_of<KotlinTypeConverter, KotlinArgumentType>...>::value, "All KotlinArgumentType must be derived from KotlinTypeConverter");

            using NativeType = std::function<typename KotlinReturnType::NativeType(typename KotlinArgumentType::NativeType...)>;
            using ManagedType = std::function<typename KotlinReturnType::ManagdType(typename KotlinArgumentType::ManagedType...)>;
            using KotlinType = jobject;
            using KotlinWrapperType = jobject;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value)
            {
                auto jcallback = env->NewGlobalRef(value);

                return [&, jcallback](typename KotlinArgumentType::ManagedType... args) -> typename KotlinReturnType::ManagedType
                {
                    char invoke_signature[256]{ 0 };
                    // getKotlinSignature<KotlinReturnType, KotlinParamTypes...>(invoke_signature);

                    JNIEnv* env {nullptr};
                    int getEnvStat = jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
                    if (getEnvStat == JNI_EDETACHED) {
                        if (jvm->AttachCurrentThread(&env, NULL) != 0) {
                            return;
                        }
                    }
                    else if (getEnvStat == JNI_EVERSION) {
                        return;
                    }
                    else if (getEnvStat == JNI_OK) {
                    }

                    jclass clazz = env->GetObjectClass(jcallback);
                    jmethodID callback = env->GetMethodID(clazz, "invoke", invoke_signature);

                    if constexpr(std::is_same_v<typename KotlinReturnType::NativeType, typename KVoid::NativeType>)
                    {
                        env->CallVoidMethod(clazz, callback);
                    }

                    if (getEnvStat == JNI_EDETACHED)
                        jvm->DetachCurrentThread();
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
            using NativeType = std::array<bool, size>;
            using ManagedType = NativeType;
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
        };

        template<size_t size>
        class KArray<KInt8, size> : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "[B"; };

            using NativeType = std::array<int8_t, size>;
            using ManagedType = NativeType;
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
        };

        template<size_t size>
        class KArray<KInt16, size> : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "[S"; };

            using NativeType = std::array<int16_t, size>;
            using ManagedType = NativeType;
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
        };

        template<size_t size>
        class KArray<KInt32, size> : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "[I"; };

            using NativeType = std::array<int32_t, size>;
            using ManagedType = NativeType;
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
        };

        template<size_t size>
        class KArray<KInt64, size> : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "[J"; };

            using NativeType = std::array<int64_t, size>;
            using ManagedType = NativeType;
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
        };

        template<size_t size>
        class KArray<KFloat, size> : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "[F"; };

            using NativeType = std::array<float, size>;
            using ManagedType = NativeType;
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
        };

        template<size_t size>
        class KArray<KDouble, size> : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "[D"; };

            using NativeType = std::array<double, size>;
            using ManagedType = NativeType;
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
        };

        template<size_t size>
        class KArray<KString, size> : public KotlinTypeConverter
        {
        public:
            const char* signature() override { return "[Ljava/lang/String;"; };

            using NativeType = std::array<std::string, size>;
            using ManagedType = NativeType;
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
                KotlinType ret = env->NewObjectArray(value.size(), _KString, nullptr);
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
        };

        template<typename KDerivedEnum, size_t size>
        class KArray<KDerivedEnum, size, typename std::enable_if<std::is_base_of<KEnum, KDerivedEnum>::value>::type> : public KotlinTypeConverter
        {
        public:
            const char* signature() override {
                static char sig[256];
                sprintf(sig, "[%s", KDerivedEnum().signature());
                return sig;
            }

            using NativeType = std::array<typename KDerivedEnum::NativeType, size>;
            using ManagedType = std::array<int32_t, size>;
            using KotlinType = jobjectArray;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                size_t length = env->GetArrayLength(value);
                
                ManagedType ret;
                for (size_t i = 0; i < length; i++)
                {
                    auto element = env->GetObjectArrayElement(value, i);
                    auto new_element = KDerivedEnum().toManagedType(env, element);
                    ret[i] = new_element;
                }
                
                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                auto clazz = env->FindClass(KDerivedEnum().className());
                KotlinType ret = env->NewObjectArray(value.size(), clazz, nullptr);
                for (size_t i =0;i<value.size();i++)
                {
                    auto element = value[i];
                    auto new_element = KDerivedEnum().toKotlinType(env, element);
                    env->SetObjectArrayElement(ret, i, new_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                for (size_t i =0;i<src.size();i++) {
                    auto element = src[i];
                    auto new_element = KDerivedEnum().toKotlinType(env, element);
                    env->SetObjectArrayElement(dest, i, new_element);
                }
            }
        };
        
        template<typename KDerivedObject, size_t size>
        class KArray<KDerivedObject, size, typename std::enable_if<std::is_base_of<KObject, KDerivedObject>::value>::type> : public KotlinTypeConverter
        {
        public:
            const char* signature() override {
                static char sig[256];
                sprintf(sig, "[%s", KDerivedObject().signature());
                return sig;
            }

            using NativeType = std::array<typename KDerivedObject::NativeType, size>;
            using ManagedType = std::array<void*, size>;
            using KotlinType = jobjectArray;

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                size_t length = env->GetArrayLength(value);

                ManagedType ret;
                for (size_t i = 0; i < length; i++)
                {
                    auto element = env->GetObjectArrayElement(value, i);
                    auto new_element = KDerivedObject().toManagedType(env, element);
                    ret[i] = new_element;
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                auto clazz = env->FindClass(KDerivedObject().className());
                KotlinType ret = env->NewObjectArray(value.size(), clazz, nullptr);
                for (size_t i =0;i<value.size();i++)
                {
                    auto element = value[i];
                    auto new_element = KDerivedObject().toKotlinType(env, element);
                    env->SetObjectArrayElement(ret, i, new_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                for (size_t i =0;i<src.size();i++)
                {
                    auto element = src[i];
                    auto new_element = KDerivedObject().toKotlinType(env, element);
                    env->SetObjectArrayElement(dest, i, new_element);
                }
            }
        };

        template<size_t size, class KotlinReturnType, class... KotlinArgumentType>
        class KArray<KCallback<KotlinReturnType, KotlinArgumentType...>, size> : public KotlinTypeConverter
        {
        public:

            using Callback = KCallback<KotlinReturnType, KotlinArgumentType...>;
            using NativeType = std::vector<typename Callback::NativeType>;
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
            using NativeType = std::vector<bool>;
            using ManagedType = NativeType;
            using KotlinType = jobject;
            using KotlinElementType = KBoolean;
            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }
            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, _KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, _KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(_KVector, _KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, _KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, _KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, _KVector_add, ko_element);
                }
            }
        };

        template<>
        struct KVector<KInt8> : public KotlinTypeConverter
        {
            using NativeType = std::vector<int8_t>;
            using ManagedType = NativeType;
            using KotlinType = jobject;
            using KotlinElementType = KInt8;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, _KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, _KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(_KVector, _KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, _KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, _KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, _KVector_add, ko_element);
                }
            }
        };

        template<>
        struct KVector<KInt16> : public KotlinTypeConverter
        {
            using NativeType = std::vector<int16_t>;
            using ManagedType = NativeType;
            using KotlinType = jobject;
            using KotlinElementType = KInt16;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, _KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, _KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(_KVector, _KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, _KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, _KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, _KVector_add, ko_element);
                }
            }
        };

        template<>
        struct KVector<KInt32> : public KotlinTypeConverter
        {
            using NativeType = std::vector<int32_t>;
            using ManagedType = NativeType;
            using KotlinType = jobject;
            using KotlinElementType = KInt32;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, _KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, _KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(_KVector, _KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, _KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, _KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, _KVector_add, ko_element);
                }
            }
        };

        template<>
        struct KVector<KInt64> : public KotlinTypeConverter
        {
            using NativeType = std::vector<int64_t>;
            using ManagedType = NativeType;
            using KotlinType = jobject;
            using KotlinElementType = KInt64;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, _KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, _KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(_KVector, _KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, _KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, _KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, _KVector_add, ko_element);
                }
            }
        };

        template<>
        struct KVector<KFloat> : public KotlinTypeConverter
        {
            using NativeType = std::vector<float>;
            using ManagedType = NativeType;
            using KotlinType = jobject;
            using KotlinElementType = KFloat;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, _KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, _KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(_KVector, _KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, _KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, _KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, _KVector_add, ko_element);
                }
            }
        };

        template<>
        struct KVector<KDouble> : public KotlinTypeConverter
        {
            using NativeType = std::vector<double>;
            using ManagedType = NativeType;
            using KotlinType = jobject;
            using KotlinElementType = KDouble;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, _KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, _KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(_KVector, _KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, _KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, _KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, _KVector_add, ko_element);
                }
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
                auto size = env->CallIntMethod(value, _KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, _KVector_get, i);
                    auto k_element = KString().toKotlinType(env, ko_element);
                    ret.push_back(KString().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(_KVector, _KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KString().toKotlinType(env, element);
                    auto ko_element = KString().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, _KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, _KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KString().toKotlinType(env, element);
                    auto ko_element = KString().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, _KVector_add, ko_element);
                }
            }
        };

        template<typename KDerivedEnum>
        struct KVector<KDerivedEnum, typename std::enable_if<std::is_base_of<KEnum, KDerivedEnum>::value>::type> : public KotlinTypeConverter
        {
            using NativeType = std::vector<typename KDerivedEnum::NativeType>;
            using ManagedType = std::vector<int32_t>;
            using KotlinType = jobject;
            using KotlinElementType = KDerivedEnum;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, _KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, _KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(_KVector, _KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, _KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, _KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, _KVector_add, ko_element);
                }
            }
        };

        template<typename KDerivedObject>
        struct KVector<KDerivedObject, typename std::enable_if<std::is_base_of<KObject, KDerivedObject>::value>::type> : public KotlinTypeConverter
        {
            using NativeType = std::vector<typename KDerivedObject::NativeType>;
            using ManagedType = std::vector<void*>;
            using KotlinType = jobject;
            using KotlinElementType = KDerivedObject;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, _KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, _KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(_KVector, _KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, _KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, _KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, _KVector_add, ko_element);
                }
            }
        };

        template<class KotlinReturnType, class... KotlinArgumentType>
        struct KVector<KCallback<KotlinReturnType, KotlinArgumentType...>> : public KotlinTypeConverter
        {
            using Callback = KCallback<KotlinReturnType, KotlinArgumentType...>;
            using NativeType = std::vector<Callback>;
            using ManagedType = std::vector<void*>;
            using KotlinType = jobject;

            using KotlinElementType = KCallback<KotlinReturnType, KotlinArgumentType...>;

            const char* signature() override {
                return "Ljava/lang/ArrayList;";
            }

            ManagedType toManagedType(JNIEnv* env, const KotlinType& value) {
                auto size = env->CallIntMethod(value, _KVector_size);
                ManagedType ret;

                for (size_t i = 0; i < size; i++)
                {
                    auto ko_element = env->CallObjectMethod(value, _KVector_get, i);
                    auto k_element = KotlinElementType().toKotlinType(env, ko_element);
                    ret.push_back(KotlinElementType().toManagedType(env, k_element));
                }

                return ret;
            }
            KotlinType toKotlinType(JNIEnv* env, const ManagedType& value) {
                KotlinType ret = env->NewObject(_KVector, _KVector_init);
                for (auto element : value)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(ret, _KVector_add, ko_element);
                }
                return ret;
            }
            void copy(JNIEnv* env, const ManagedType& src, KotlinType& dest)
            {
                env->CallVoidMethod(dest, _KVector_clear);
                for (auto element : src)
                {
                    auto kelemnent = KotlinElementType().toKotlinType(env, element);
                    auto ko_element = KotlinElementType().toKotlinWrapperType(env, kelemnent);
                    env->CallBooleanMethod(dest, _KVector_add, ko_element);
                }
            }
        };

    }
}

#endif
