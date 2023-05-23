package com.example.kotlintypeconverter

import android.util.Log

class TestLibrary {
    companion object {
        // Used to load the 'kotlintypeconverter' library on application startup.
        init {
            try {
                // it only be used in unit test not insturmnetal test
                /*
                val cpuArchitecture = System.getProperty("os.arch")
                val currentDirectory = System.getProperty("user.dir")
                val cpuDirectory =
                    when(cpuArchitecture)
                    {
                        "amd64" -> "arm64-v8a"
                        else -> "x86"
                    }

                val currentLibraryDirectory = "${currentDirectory}\\libs\\${cpuDirectory}\\libKotlinTypeConverter.so"
                System.load(currentLibraryDirectory)
                */
                System.loadLibrary("KotlinTypeConverter")
            }
            catch (e : Exception)
            {
                val value = e.stackTraceToString()
                Log.e("TestLibrary", value)
            }
        }
    }

    external fun primitiveTest(value1 : Byte, value2 : Short, value3 : Int, value4 : Long, value5 : Float, value6: Double, value7 : Boolean, value8 : String) : String
    external fun primitiveArrayTest(value1 : ByteArray, value2 : ShortArray, value3 : IntArray, value4 : LongArray, value5 : FloatArray, value6 : DoubleArray, value7 : BooleanArray, value8 : Array<String>) : String
    external fun primitiveVectorTest(
        value1 : MutableList<Byte>,
        value2 : MutableList<Short>,
        value3 : MutableList<Int>,
        value4 : MutableList<Long>,
        value5 : MutableList<Float>,
        value6 : MutableList<Double>,
        value7 : MutableList<Boolean>,
        value8 : MutableList<String>) : String

    external fun enumTest(
        value1 : TestEnum,
        value2 : Array<TestEnum>,
        value3 : MutableList<TestEnum>
    ) : TestEnum

    external fun createNativeHandle(value : Int) : Long
    external fun releaseNativeHandle(handle : Long)

    external fun TestObject_getValue(handle : Long) : Int
    external fun TestObject_sum(handle : Long, value1 : TestObject, value2 : TestObject) : TestObject

    external fun objectTest(
        value1 : TestObject,
        value2 : Array<TestObject>,
        value3 : MutableList<TestObject>
    ) : TestObject

    external fun callbackTest(
        voidCallback : (Int, String, TestObject, MutableList<String>) -> Unit,
        booleanCallback : (Int, Float) -> Boolean,
        byteCallback : (Int, Float) -> Byte,
        shortCallback : (Int, Float) -> Short,
        IntCallback : (Int, Float) -> Int,
        LongCallback : (Int, Float) -> Long,
        floatCallback : (Int, Float) -> Float,
        doubleCallback : (Int, Float) -> Double,
        stringCallback : (Int, Float) -> String,
        enumCallback : (Int, Float) -> TestEnum,
        objectCallback : (Int, Float) -> TestObject
        ) : Boolean
}