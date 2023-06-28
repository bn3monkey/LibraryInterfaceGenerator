package com.example.kotlintypeconverter

class TestObject : WrapperTypeObject {
    constructor(handle : NativeHandle) : super(handle)
    {
        TestLibrary().TestObject_addReleaser(releaser)
    }
    constructor(value : Int) : this(NativeHandle(TestLibrary().TestObject_construct(value)))
    {
        TestLibrary().TestObject_addReleaser(releaser)
    }

    override fun release(value: Long) {
        TestLibrary().TestObject_release(value)
    }


    fun sum(value1 : TestObject, value2 : TestObject) : TestObject
    {
        val __value1 = toKotlinWrapperType(value1)
        val __value2 = toKotlinWrapperType(value2)
        val __temp_ret = TestLibrary().TestObject_sum(toWrapperType(), __value1, __value2)
        val __ret = toKotlinType<TestObject>(__temp_ret)
        return __ret
    }

    fun getValue() : Int {
        return TestLibrary().TestObject_getValue(toWrapperType())
    }


}