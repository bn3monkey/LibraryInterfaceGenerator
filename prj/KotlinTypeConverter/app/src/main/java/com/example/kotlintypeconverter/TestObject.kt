package com.example.kotlintypeconverter

class TestObject {
    constructor()
    {

    }
    constructor(handle : Long)
    {
       _nativeHandle = handle
    }

    private var _nativeHandle : Long = 0

    fun nativeHandle() : Long {
        return _nativeHandle
    }
}