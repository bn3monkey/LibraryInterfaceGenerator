package com.example.kotlintypeconverter

class TestObject : WrapperTypeObject {
    constructor(handle : NativeHandle) : super(handle)
    {
        TestLibrary().TestObject_addReleaser(releaser)
    }
    constructor(value : Int) : this(construct(value))
    constructor(value1 : Int, value2 : Int) : this(construct(value1, value2))


    companion object {
        fun construct (value : Int) : NativeHandle {
            val i_value = toKotlinWrapperType(value)
            val temp_ret = TestLibrary().TestObject_construct(i_value)
            return NativeHandle(temp_ret)
        }
        fun construct (value1 : Int, value2 : Int) : NativeHandle {
            val i_value1 = toKotlinWrapperType(value1)
            val i_value2 = toKotlinWrapperType(value2)
            val temp_ret = TestLibrary().TestObject_construct(i_value1 + i_value2)
            return NativeHandle(temp_ret)
        }
    }

    override fun release(handle : NativeHandle) {
        TestLibrary().TestObject_release(handle.get())
    }


    fun sum(value1 : TestObject, value2 : TestObject) : TestObject
    {
        val __value1 = toKotlinWrapperType(value1)
        val __value2 = toKotlinWrapperType(value2)
        val __temp_ret = TestLibrary().TestObject_sum(self, __value1, __value2)
        val __ret = toKotlinType<TestObject>(__temp_ret)
        return __ret
    }

    fun getValue() : Int {
        return TestLibrary().TestObject_getValue(self)
    }


}