#include <jni.h>
#include <string>
#include <memory>
#include <mutex>
#include <atomic>

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
    return env->NewStringUTF("");
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_kotlintypeconverter_TestLibrary_enumTest(JNIEnv *env, jobject thiz,
                                                                  jint value1,
                                                                  jintArray value2,
                                                                  jobject value3) {

    Bn3Monkey::Kotlin::KotlinTypeConverter::initialize(env);

    using namespace Bn3Monkey::Kotlin;



    jint ret;
    do {
        {
            using Converter = KEnum;
            auto mvalue = Converter().toManagedType(env, value1);
            auto kvalue = Converter().toKotlinType(env, mvalue);
            auto new_mvalue = Converter().toManagedType(env, kvalue);

            if (mvalue != new_mvalue) {
                ret = Converter().toKotlinType(env, 8);
                break;
            }
            ret = kvalue;
        }
        {
            using Converter = KArray<KEnum, 3>;
            auto mvalue = Converter().toManagedType(env, value2);
            auto kvalue = Converter().toKotlinType(env, mvalue);
            auto new_mvalue = Converter().toManagedType(env, kvalue);

            bool isError = false;
            for (size_t i=0 ;i<3; i++) {
                if (mvalue[i] != new_mvalue[i]) {
                    ret = KEnum().toKotlinType(env, 8);
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
            using Converter = KVector<KEnum>;
            auto mvalue = Converter().toManagedType(env, value3);
            auto kvalue = Converter().toKotlinType(env, mvalue);
            auto new_mvalue = Converter().toManagedType(env, kvalue);

            bool isError = false;
            for (size_t i=0 ;i<3; i++) {
                if (mvalue[i] != new_mvalue[i]) {
                    ret = KEnum().toKotlinType(env, 8);
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

static std::vector<std::function<void()>> releasers;


inline int Managed_getValue(void* handle)
{
    auto self = *(reinterpret_cast<std::shared_ptr<TestObject>*>(handle));
    auto ret = self->getValue();
    return ret;
}

inline void* Managed_sum(void* handle, void* v1, void* v2)
{
    auto self = *(reinterpret_cast<std::shared_ptr<TestObject>*>(handle));
    auto _v1 = *(reinterpret_cast<std::shared_ptr<TestObject>*>(v1));
    auto _v2 = *(reinterpret_cast<std::shared_ptr<TestObject>*>(v2));
    auto* ret = new std::shared_ptr<TestObject>();
    *ret = self->sum(_v1, _v2);
    return reinterpret_cast<void*>(ret);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_kotlintypeconverter_TestLibrary_TestObject_1construct(JNIEnv *env, jobject thiz, int value) {
    return reinterpret_cast<jlong>(new std::shared_ptr<TestObject>(new TestObject(value)));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_kotlintypeconverter_TestLibrary_TestObject_1release(JNIEnv *env, jobject thiz, jlong ptr) {
    delete reinterpret_cast<std::shared_ptr<TestObject>*>(ptr);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_kotlintypeconverter_TestLibrary_TestObject_1addReleaser(JNIEnv *env, jobject thiz,
                                                             jobject releaser) {
    using namespace Bn3Monkey::Kotlin;
    auto _releaser = KCallback<KVoid>().toManagedType(env, releaser);
    releasers.push_back(_releaser);

}


extern "C"
JNIEXPORT jint JNICALL
Java_com_example_kotlintypeconverter_TestLibrary_TestObject_1getValue(JNIEnv *env, jobject thiz,
                                                                      jlong handle) {

    using namespace Bn3Monkey::Kotlin;

    auto __ret = Managed_getValue((void*)handle);
    auto ret = KInt32().toKotlinType(env, __ret);

    return ret;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_kotlintypeconverter_TestLibrary_TestObject_1sum(JNIEnv *env, jobject thiz,
                                                                 jlong handle, jlong value1, jlong value2) {

    using namespace Bn3Monkey::Kotlin;

    auto _value1 = KObject().toManagedType(env, value1);
    auto _value2 = KObject().toManagedType(env, value2);

    auto _ret = Managed_sum((void *)handle, _value1, _value2);
    auto ret = KObject().toKotlinType(env, _ret);
    return ret;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_kotlintypeconverter_TestLibrary_objectTest(JNIEnv *env, jobject thiz,
                                                            jlong value1, jlongArray value2,
                                                            jobject value3) {

    Bn3Monkey::Kotlin::KotlinTypeConverter::initialize(env);

    auto ret = value1;

    using namespace Bn3Monkey::Kotlin;
    {
        using Converter = KArray<KObject, 3>;
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
        using Converter = KVector<KObject>;
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

#include <limits>

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_kotlintypeconverter_TestLibrary_callbackTest(JNIEnv *env, jobject thiz,
                                                              jobject void_callback,
                                                              jobject boolean_callback,
                                                              jobject byte_callback,
                                                              jobject short_callback,
                                                              jobject int_callback,
                                                              jobject long_callback,
                                                              jobject float_callback,
                                                              jobject double_callback,
                                                              jobject string_callback,
                                                              jobject enum_callback,
                                                              jobject object_callback) {
    Bn3Monkey::Kotlin::KotlinTypeConverter::initialize(env);
    using namespace Bn3Monkey::Kotlin;

    /*
    {
        auto clazz = env->GetObjectClass(void_callback);
        auto methodID = env->GetMethodID(clazz, "invoke",
                                         "(ILjava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V");

        auto v1 = new std::shared_ptr<TestObject>(
                new TestObject(2));
        auto v2 = std::vector<std::string> {"PAPYRUS", "PAPYRUS"};
        auto k1 = KTestObject().toKotlinType(env, v1);
        auto k2 = KVector<KString>().toKotlinType(env, v2);
        auto k3 = env->NewStringUTF("SANS");

        env->CallVoidMethod(void_callback, methodID, 1, k3, k1, k2);
        printf("s\n");
    }
    */

    {
        auto mvoid_callback = KCallback<KVoid, KInt32, KString, KObject, KVector<KString>>().toManagedType(env, void_callback);

        auto new_object = new std::shared_ptr<TestObject>(
                new TestObject(2));
        auto vector = std::vector<std::string> {"PAPYRUS", "PAPYRUS"};
        mvoid_callback(1, "SANS", new_object, vector);
    }
    {
        auto mcallback = KCallback<KBoolean, KInt32, KFloat>().toManagedType(env, boolean_callback);
        auto ret = mcallback(1, 1.0f);
        if (!ret)
            return false;
    }
    {
        auto mcallback = KCallback<KInt8, KInt32, KFloat>().toManagedType(env, byte_callback);
        auto ret = mcallback(1, 1.0f);
        if (ret != 'a')
            return false;
    }
    {
        auto mcallback = KCallback<KInt16, KInt32, KFloat>().toManagedType(env, short_callback);
        auto ret = mcallback(1, 1.0f);
        if (ret != std::numeric_limits<int16_t>::max())
            return false;
    }
    {
        auto mcallback = KCallback<KInt32, KInt32, KFloat>().toManagedType(env, int_callback);
        auto ret = mcallback(1, 1.0f);
        if (ret != std::numeric_limits<int32_t>::max())
            return false;
    }
    {
        auto mcallback = KCallback<KInt64, KInt32, KFloat>().toManagedType(env, long_callback);
        auto ret = mcallback(1, 1.0f);
        if (ret != std::numeric_limits<int64_t>::max())
            return false;
    }
    {
        auto mcallback = KCallback<KFloat, KInt32, KFloat>().toManagedType(env, float_callback);
        auto ret = mcallback(1, 1.0f);
        if (ret !=  std::numeric_limits<float>::max())
            return false;
    }
    {
        auto mcallback = KCallback<KDouble, KInt32, KFloat>().toManagedType(env, double_callback);
        auto ret = mcallback(1, 1.0f);
        auto mret = KDouble().toManagedType(env, ret);
        if (ret !=  std::numeric_limits<double>::max())
            return false;
    }
    {
        auto mcallback = KCallback<KString, KInt32, KFloat>().toManagedType(env, string_callback);
        auto ret = mcallback(1, 1.0f);
        if (ret !=  "SANS")
            return false;
    }
    {
        auto mcallback = KCallback<KEnum, KInt32, KFloat>().toManagedType(env, enum_callback);
        auto ret = mcallback(1, 1.0f);
        if (ret != 4)
            return false;
    }
    {
        auto mcallback = KCallback<KObject, KInt32, KFloat>().toManagedType(env, object_callback);
        auto ret = mcallback(1, 1.0f);
        auto ref = reinterpret_cast<std::shared_ptr<TestObject>*>(ret);
        auto value = (*ref)->getValue();
        if (value != 1)
            return false;
    }

    Bn3Monkey::Kotlin::KotlinTypeConverter::release(env);
    return JNI_TRUE;
}
