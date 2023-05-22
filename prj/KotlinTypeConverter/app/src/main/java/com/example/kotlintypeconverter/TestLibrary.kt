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

}