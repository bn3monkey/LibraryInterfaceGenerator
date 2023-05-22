#include <jni.h>
#include <string>

#include <KotlinTypeConverter/KotlinTypeConverter.hpp>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_kotlintypeconverter_TestLibrary_primitiveTest(
        JNIEnv* env,
        jobject /*this */,
        jbyte value1,
        jshort value2,
        jint value3,
        jlong value4,
        jfloat value5,
        jdouble value6,
        jboolean value7,
        jstring value8
        )
{
    Bn3Monkey::Kotlin::KotlinTypeConverter::initialize(env);

    using namespace Bn3Monkey::Kotlin;

    {
        using Converter = KInt8;
        auto origin_value = value1;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);

        if (origin_value != jvalue)
        {
            return env->NewStringUTF("Kint8 implementation fail");
        }
    }
    {
        using Converter = KInt16;
        auto origin_value = value2;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);

        if (origin_value != jvalue)
        {
            return env->NewStringUTF("Kint16 implementation fail");
        }
    }
    {
        using Converter = KInt32;
        auto origin_value = value3;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);

        if (origin_value != jvalue)
        {
            return env->NewStringUTF("Kint32 implementation fail");
        }
    }
    {
        using Converter = KInt64;
        auto origin_value = value4;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);

        if (origin_value != jvalue)
        {
            return env->NewStringUTF("Kint64 implementation fail");
        }
    }
    {
        using Converter = KFloat;
        auto origin_value = value5;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);

        if (origin_value != jvalue)
        {
            return env->NewStringUTF("KFloat implementation fail");
        }
    }
    {
        using Converter = KDouble;
        auto origin_value = value6;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);

        if (origin_value != jvalue)
        {
            return env->NewStringUTF("KDouble implementation fail");
        }
    }
    {
        using Converter = KBoolean;
        auto origin_value = value7;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);

        if (origin_value != jvalue)
        {
            return env->NewStringUTF("KBoolean implementation fail");
        }
    }
    {
        using Converter = KString;
        auto origin_value = value8;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);

        const char* v1 = env->GetStringUTFChars(origin_value, nullptr);
        const char* v2 = env->GetStringUTFChars(jvalue, nullptr);
        const char* v3 = value.c_str();

        if (strcmp(v1, v2))
        {
            return env->NewStringUTF("KString implementation fail (v1, v2)");
        }
        if (strcmp(v2, v3))
        {
            return env->NewStringUTF("KString implementation fail (v2, v3)");
        }
        if (strcmp(v3, v1))
        {
            return env->NewStringUTF("KString implementation fail (v3, v1)");
        }
    }
    Bn3Monkey::Kotlin::KotlinTypeConverter::release(env);
    return env->NewStringUTF("");
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_kotlintypeconverter_TestLibrary_primitiveArrayTest(
        JNIEnv* env,
        jobject /*this */,
        jbyteArray value1,
        jshortArray value2,
        jintArray value3,
        jlongArray value4,
        jfloatArray value5,
        jdoubleArray value6,
        jbooleanArray value7,
        jobjectArray value8
) {
    Bn3Monkey::Kotlin::KotlinTypeConverter::initialize(env);

    using namespace Bn3Monkey::Kotlin;

    {
        using Converter = KArray<KInt8, 3>;
        auto origin_value = value1;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);

        auto elements1 = env->GetByteArrayElements(origin_value, nullptr);
        auto elements2 = env->GetByteArrayElements(jvalue, nullptr);
        auto elements3 = value.data();

        for (size_t i =0 ;i<3;i++)
        {
            if (elements1[i] != elements2[i])
            {
                return env->NewStringUTF("Array<Kint8> implementation fail");
            }
            else if (elements1[i] != elements3[i])
            {
                return env->NewStringUTF("Array<Kint8> implementation fail");
            }
            else if (elements2[i] != elements3[i])
            {
                return env->NewStringUTF("Array<Kint8> implementation fail");
            }
        }

        for (auto& element : value)
        {
            element = 'b';
        }
        Converter().copy(env, value, origin_value);
    }

    {
        using Converter = KArray<KInt16, 3>;
        auto origin_value = value2;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);

        auto elements1 = env->GetShortArrayElements(origin_value, nullptr);
        auto elements2 = env->GetShortArrayElements(jvalue, nullptr);
        auto elements3 = value.data();

        for (size_t i =0 ;i<3;i++)
        {
            if (elements1[i] != elements2[i])
            {
                return env->NewStringUTF("Array<Kint16> implementation fail");
            }
            else if (elements1[i] != elements3[i])
            {
                return env->NewStringUTF("Array<Kint16> implementation fail");
            }
            else if (elements2[i] != elements3[i])
            {
                return env->NewStringUTF("Array<Kint16> implementation fail");
            }
        }

        for (auto& element : value)
        {
            element = 2;
        }
        Converter().copy(env, value, origin_value);
    }

    {
        using Converter = KArray<KInt32, 3>;
        auto origin_value = value3;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);

        auto elements1 = env->GetIntArrayElements(origin_value, nullptr);
        auto elements2 = env->GetIntArrayElements(jvalue, nullptr);
        auto elements3 = value.data();

        for (size_t i =0 ;i<3;i++)
        {
            if (elements1[i] != elements2[i])
            {
                return env->NewStringUTF("Array<Kint32> implementation fail");
            }
            else if (elements1[i] != elements3[i])
            {
                return env->NewStringUTF("Array<Kint32> implementation fail");
            }
            else if (elements2[i] != elements3[i])
            {
                return env->NewStringUTF("Array<Kint32> implementation fail");
            }
        }

        for (auto& element : value)
        {
            element = 2;
        }
        Converter().copy(env, value, origin_value);
    }

    {
        using Converter = KArray<KInt64, 3>;
        auto origin_value = value4;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);

        auto elements1 = env->GetLongArrayElements(origin_value, nullptr);
        auto elements2 = env->GetLongArrayElements(jvalue, nullptr);
        auto elements3 = value.data();

        for (size_t i =0 ;i<3;i++)
        {
            if (elements1[i] != elements2[i])
            {
                return env->NewStringUTF("Array<Kint64> implementation fail");
            }
            else if (elements1[i] != elements3[i])
            {
                return env->NewStringUTF("Array<Kint64> implementation fail");
            }
            else if (elements2[i] != elements3[i])
            {
                return env->NewStringUTF("Array<Kint64> implementation fail");
            }
        }

        for (auto& element : value)
        {
            element = 2;
        }
        Converter().copy(env, value, origin_value);
    }

    {
        using Converter = KArray<KFloat, 3>;
        auto origin_value = value5;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);

        auto elements1 = env->GetFloatArrayElements(origin_value, nullptr);
        auto elements2 = env->GetFloatArrayElements(jvalue, nullptr);
        auto elements3 = value.data();

        for (size_t i =0 ;i<3;i++)
        {
            if (elements1[i] != elements2[i])
            {
                return env->NewStringUTF("Array<KFloat> implementation fail");
            }
            else if (elements1[i] != elements3[i])
            {
                return env->NewStringUTF("Array<KFloat> implementation fail");
            }
            else if (elements2[i] != elements3[i])
            {
                return env->NewStringUTF("Array<KFloat> implementation fail");
            }
        }

        for (auto& element : value)
        {
            element = 2.0f;
        }
        Converter().copy(env, value, origin_value);
    }

    {
        using Converter = KArray<KDouble, 3>;
        auto origin_value = value6;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);

        auto elements1 = env->GetDoubleArrayElements(origin_value, nullptr);
        auto elements2 = env->GetDoubleArrayElements(jvalue, nullptr);
        auto elements3 = value.data();

        for (size_t i =0 ;i<3;i++)
        {
            if (elements1[i] != elements2[i])
            {
                return env->NewStringUTF("Array<KDouble> implementation fail");
            }
            else if (elements1[i] != elements3[i])
            {
                return env->NewStringUTF("Array<KDouble> implementation fail");
            }
            else if (elements2[i] != elements3[i])
            {
                return env->NewStringUTF("Array<KDouble> implementation fail");
            }
        }

        for (auto& element : value)
        {
            element = 2.0;
        }
        Converter().copy(env, value, origin_value);
    }

    {
        using Converter = KArray<KBoolean, 3>;
        auto origin_value = value7;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);

        auto elements1 = env->GetBooleanArrayElements(origin_value, nullptr);
        auto elements2 = env->GetBooleanArrayElements(jvalue, nullptr);
        auto elements3 = value.data();

        for (size_t i =0 ;i<3;i++)
        {
            if (elements1[i] != elements2[i])
            {
                return env->NewStringUTF("Array<KBoolean> implementation fail");
            }
            else if (elements1[i] != elements3[i])
            {
                return env->NewStringUTF("Array<KBoolean> implementation fail");
            }
            else if (elements2[i] != elements3[i])
            {
                return env->NewStringUTF("Array<KBoolean> implementation fail");
            }
        }

        for (auto& element : value)
        {
            element = false;
        }
        Converter().copy(env, value, origin_value);
    }

    {
        using Converter = KArray<KString, 3>;
        auto origin_value = value8;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);


        for (size_t i =0 ;i<3;i++)
        {
            auto element1 = env->GetObjectArrayElement(origin_value, i);
            auto element2 = env->GetObjectArrayElement(jvalue, i);
            auto element3 = value[i];

            auto v1 = env->GetStringUTFChars((jstring)element1, nullptr);
            auto v2 = env->GetStringUTFChars((jstring)element2, nullptr);
            auto v3 = element3.c_str();

            if (strcmp(v1, v2))
            {
                return env->NewStringUTF("Array<KString> implementation fail");
            }
            if (strcmp(v1, v3))
            {
                return env->NewStringUTF("Array<KString> implementation fail");
            }
            if (strcmp(v2, v3))
            {
                return env->NewStringUTF("Array<KString> implementation fail");
            }
        }
        for (auto& element : value)
        {
            element = "PAPYRUS";
        }
        Converter().copy(env, value, origin_value);
    }

    Bn3Monkey::Kotlin::KotlinTypeConverter::release(env);
    return env->NewStringUTF("");
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_kotlintypeconverter_TestLibrary_primitiveVectorTest(JNIEnv *env, jobject thiz,
                                                                     jobject value1, jobject value2,
                                                                     jobject value3, jobject value4,
                                                                     jobject value5, jobject value6,
                                                                     jobject value7,
                                                                     jobject value8) {
    Bn3Monkey::Kotlin::KotlinTypeConverter::initialize(env);

    using namespace Bn3Monkey::Kotlin;

    {
        using Converter = KVector<KInt8>;
        auto origin_value = value1;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);

    }


    Bn3Monkey::Kotlin::KotlinTypeConverter::release(env);
    return env->NewStringUTF("");
}