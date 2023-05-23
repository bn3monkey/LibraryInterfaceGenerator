#include <jni.h>
#include <string>
#include <memory>

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
        auto new_value = Converter().toManagedType(env, jvalue);

        for (size_t i=0;i<value.size();i++)
        {
            if (value[i] != new_value[i])
            {
                return env->NewStringUTF("Vector<KInt8> implementation fail");
            }
        }
        value.push_back('b');
        value.push_back('b');
        Converter().copy(env, value, origin_value);
    }
    {
        using Converter = KVector<KInt16>;
        auto origin_value = value2;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);
        auto new_value = Converter().toManagedType(env, jvalue);

        for (size_t i=0;i<value.size();i++)
        {
            if (value[i] != new_value[i])
            {
                return env->NewStringUTF("Vector<KInt16> implementation fail");
            }
        }
        value.push_back(2);
        value.push_back(2);
        Converter().copy(env, value, origin_value);
    }
    {
        using Converter = KVector<KInt32>;
        auto origin_value = value3;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);
        auto new_value = Converter().toManagedType(env, jvalue);

        for (size_t i=0;i<value.size();i++)
        {
            if (value[i] != new_value[i])
            {
                return env->NewStringUTF("Vector<KInt32> implementation fail");
            }
        }
        value.push_back(2);
        value.push_back(2);
        Converter().copy(env, value, origin_value);
    }
    {
        using Converter = KVector<KInt64>;
        auto origin_value = value4;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);
        auto new_value = Converter().toManagedType(env, jvalue);

        for (size_t i=0;i<value.size();i++)
        {
            if (value[i] != new_value[i])
            {
                return env->NewStringUTF("Vector<KInt64> implementation fail");
            }
        }
        value.push_back(2);
        value.push_back(2);
        Converter().copy(env, value, origin_value);
    }
    {
        using Converter = KVector<KFloat>;
        auto origin_value = value5;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);
        auto new_value = Converter().toManagedType(env, jvalue);

        for (size_t i=0;i<value.size();i++)
        {
            if (value[i] != new_value[i])
            {
                return env->NewStringUTF("Vector<KFloat> implementation fail");
            }
        }
        value.push_back(2.0f);
        value.push_back(2.0f);
        Converter().copy(env, value, origin_value);
    }
    {
        using Converter = KVector<KDouble>;
        auto origin_value = value6;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);
        auto new_value = Converter().toManagedType(env, jvalue);

        for (size_t i=0;i<value.size();i++)
        {
            if (value[i] != new_value[i])
            {
                return env->NewStringUTF("Vector<KDouble> implementation fail");
            }
        }
        value.push_back(2.0);
        value.push_back(2.0);
        Converter().copy(env, value, origin_value);
    }
    {
        using Converter = KVector<KBoolean>;
        auto origin_value = value7;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);
        auto new_value = Converter().toManagedType(env, jvalue);

        for (size_t i=0;i<value.size();i++)
        {
            if (value[i] != new_value[i])
            {
                return env->NewStringUTF("Vector<KBoolean> implementation fail");
            }
        }
        value.push_back(true);
        value.push_back(true);
        Converter().copy(env, value, origin_value);
    }
    {
        using Converter = KVector<KString>;
        auto origin_value = value8;
        auto value = Converter().toManagedType(env, origin_value);
        auto jvalue = Converter().toKotlinType(env, value);
        auto new_value = Converter().toManagedType(env, jvalue);

        for (size_t i=0;i<value.size();i++)
        {
            if (value[i] != new_value[i])
            {
                return env->NewStringUTF("Vector<KString> implementation fail");
            }
        }
        value.push_back("PAPYRUS");
        value.push_back("PAPYRUS");
        Converter().copy(env, value, origin_value);
    }
    Bn3Monkey::Kotlin::KotlinTypeConverter::release(env);
    return env->NewStringUTF("");
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_kotlintypeconverter_TestLibrary_enumTest(JNIEnv *env, jobject thiz,
                                                                  jobject value1,
                                                                  jobjectArray value2,
                                                                  jobject value3) {
    Bn3Monkey::Kotlin::KotlinTypeConverter::initialize(env);

    using namespace Bn3Monkey::Kotlin;

    class KTestEnum : public KEnum
    {
    public:
        const char* className() override { return "com/example/kotlintypeconverter/TestEnum";}
        const char* signature() override { return "Lcom/example/kotlintypeconverter/TestEnum;"; }
    };

    jobject ret;
    do {
        {
            using Converter = KTestEnum;
            auto mvalue = Converter().toManagedType(env, value1);
            auto kvalue = Converter().toKotlinType(env, mvalue);
            auto new_mvalue = Converter().toManagedType(env, kvalue);

            if (mvalue != new_mvalue) {
                ret = KTestEnum().toKotlinType(env, 8);
                break;
            }
            ret = kvalue;
        }
        {
            using Converter = KArray<KTestEnum, 3>;
            auto mvalue = Converter().toManagedType(env, value2);
            auto kvalue = Converter().toKotlinType(env, mvalue);
            auto new_mvalue = Converter().toManagedType(env, kvalue);

            bool isError = false;
            for (size_t i=0 ;i<3; i++) {
                if (mvalue[i] != new_mvalue[i]) {
                    ret = KTestEnum().toKotlinType(env, 8);
                    isError= true;
                    break;
                }
            }

            if (isError)
                break;

            for (size_t i=0;i<3;i++) {
                mvalue[i] = 6;
            }
            Converter().copy(env, mvalue, value2);
        }
        {
            using Converter = KVector<KTestEnum>;
            auto mvalue = Converter().toManagedType(env, value3);
            auto kvalue = Converter().toKotlinType(env, mvalue);
            auto new_mvalue = Converter().toManagedType(env, kvalue);

            bool isError = false;
            for (size_t i=0 ;i<3; i++) {
                if (mvalue[i] != new_mvalue[i]) {
                    ret = KTestEnum().toKotlinType(env, 8);
                    isError= true;
                    break;
                }
            }

            if (isError)
                break;

            mvalue.push_back(2);
            mvalue.push_back(2);
            Converter().copy(env, mvalue, value3);
        }
    }
    while(false);
    Bn3Monkey::Kotlin::KotlinTypeConverter::release(env);
    return ret;
}

class TestObject
{
public:
    TestObject(int value) : _value(value) {}

    std::shared_ptr<TestObject> sum(const std::shared_ptr<TestObject>& v1, const std::shared_ptr<TestObject>& v2) {
        return std::shared_ptr<TestObject>(new TestObject(v1->_value + v2->_value));
    }

    int getValue() {return _value;}

private:
    int _value;
};



extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_kotlintypeconverter_TestLibrary_createNativeHandle(JNIEnv *env, jobject thiz, int value) {
    return reinterpret_cast<jlong>(new std::shared_ptr<TestObject>(new TestObject(value)));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_kotlintypeconverter_TestLibrary_releaseNativeHandle(JNIEnv *env, jobject thiz, jlong ptr) {
    delete reinterpret_cast<std::shared_ptr<TestObject>*>(ptr);
}

class KTestObject : public Bn3Monkey::Kotlin::KObject
{
public:
    using NativeType = std::shared_ptr<TestObject>*;
    const char* className() override { return "com/example/kotlintypeconverter/TestObject"; }
    const char* signature() override { return "Lcom/example/kotlintypeconverter/TestObject;"; }
};

inline int Managed_getValue(void* handle)
{
    auto self = reinterpret_cast<KTestObject::NativeType>(handle);
    auto ret = (*self)->getValue();
    return ret;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_kotlintypeconverter_TestLibrary_TestObject_1getValue(JNIEnv *env, jobject thiz,
                                                                      jlong handle) {
    Bn3Monkey::Kotlin::KotlinTypeConverter::initialize(env);

    using namespace Bn3Monkey::Kotlin;

    auto __ret = Managed_getValue((void*)handle);
    auto ret = KInt32().toKotlinType(env, __ret);
    Bn3Monkey::Kotlin::KotlinTypeConverter::release(env);

    return ret;
}

inline void* Managed_sum(void* handle, void* value1, void* value2)
{
    auto _handle = reinterpret_cast<KTestObject::NativeType>(handle);
    auto _value1 = reinterpret_cast<KTestObject::NativeType>(value1);
    auto _value2 = reinterpret_cast<KTestObject::NativeType>(value2);

    auto ret = (*_handle)->sum(*_value1, *_value2);
    auto new_handle = new std::shared_ptr<TestObject>();
    *new_handle = ret;
    return reinterpret_cast<void*>(new_handle);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_kotlintypeconverter_TestLibrary_TestObject_1sum(JNIEnv *env, jobject thiz,
                                                                 jlong handle, jobject value1, jobject value2) {
    Bn3Monkey::Kotlin::KotlinTypeConverter::initialize(env);

    using namespace Bn3Monkey::Kotlin;

    auto _value1 = KTestObject().toManagedType(env, value1);
    auto _value2 = KTestObject().toManagedType(env, value2);

    auto _ret = Managed_sum((void *)handle, _value1, _value2);
    auto ret = KTestObject().toKotlinType(env, _ret);

    Bn3Monkey::Kotlin::KotlinTypeConverter::release(env);
    return ret;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_kotlintypeconverter_TestLibrary_objectTest(JNIEnv *env, jobject thiz,
                                                            jobject value1, jobjectArray value2,
                                                            jobject value3) {

    Bn3Monkey::Kotlin::KotlinTypeConverter::initialize(env);

    auto ret = value1;

    using namespace Bn3Monkey::Kotlin;
    {
        using Converter = KArray<KTestObject, 3>;
        auto mvalue = Converter().toManagedType(env, value2);
        auto jvalue = Converter().toKotlinType(env, mvalue);
        auto new_value = Converter().toManagedType(env, jvalue);

        for (size_t i =0;i<3;i++)
        {
            if (mvalue[i] != new_value[i])
            {
                return value1;
            }
        }

        for (size_t i=0;i<3;i++)
        {
            mvalue[i] = new std::shared_ptr<TestObject>(new TestObject(10*(i+1)));
        }

        Converter().copy(env, mvalue, value2);
    }

    {
        using Converter = KVector<KTestObject>;
        auto mvalue = Converter().toManagedType(env, value3);
        auto jvalue = Converter().toKotlinType(env, mvalue);
        auto new_value = Converter().toManagedType(env, jvalue);

        for (size_t i =0;i<3;i++)
        {
            if (mvalue[i] != new_value[i])
            {
                return value1;
            }
        }

        mvalue.push_back(new std::shared_ptr<TestObject>(new TestObject(20)));
        mvalue.push_back(new std::shared_ptr<TestObject>(new TestObject(20)));

        Converter().copy(env, mvalue, value3);
    }

    Bn3Monkey::Kotlin::KotlinTypeConverter::release(env);
    return ret;
}
