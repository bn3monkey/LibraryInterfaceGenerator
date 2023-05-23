package com.example.kotlintypeconverter

class TestObject : AutoCloseable {
    constructor(value : Int)
    {
        _nativeHandle = TestLibrary().createNativeHandle(value)
    }
    internal constructor(handle : Long)
    {
       _nativeHandle = handle
    }

    private var _nativeHandle : Long = 0

    fun nativeHandle() : Long {
        return _nativeHandle
    }

    fun sum(value1 : TestObject, value2 : TestObject) : TestObject = TestLibrary().TestObject_sum(_nativeHandle, value1, value2)
    fun getValue() : Int = TestLibrary().TestObject_getValue(_nativeHandle)

    override fun close() {
        if (_nativeHandle != 0L)
        {
            TestLibrary().releaseNativeHandle(_nativeHandle)
            _nativeHandle = 0L
        }
    }
}