package com.example.kotlintypeconverter

import androidx.test.platform.app.InstrumentationRegistry
import androidx.test.ext.junit.runners.AndroidJUnit4

import org.junit.Test
import org.junit.runner.RunWith

import org.junit.Assert.*

/**
 * Instrumented test, which will execute on an Android device.
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
@RunWith(AndroidJUnit4::class)
class ExampleInstrumentedTest {
    @Test
    fun useAppContext() {
        // Context of the app under test.
        val appContext = InstrumentationRegistry.getInstrumentation().targetContext
        assertEquals("com.example.kotlintypeconverter", appContext.packageName)
    }
    @Test
    fun primitiveTest()
    {
        val result = TestLibrary().primitiveTest('a'.toByte(), Short.MAX_VALUE, Int.MAX_VALUE, Long.MAX_VALUE, Float.MAX_VALUE, Double.MAX_VALUE, true,"SANS")
        assertEquals("", result)
    }
    @Test
    fun primiteveArrayTest()
    {
        val value1 = ByteArray(3) { _ -> 'a'.toByte() }
        val value2 = ShortArray(3) {1}
        val value3 = IntArray(3) {1}
        val value4 = LongArray(3) {1}
        val value5 = FloatArray(3) {1.0f}
        val value6 = DoubleArray(3) {1.0}
        val value7 = BooleanArray(3) {true}
        val value8 = Array<String>(3) {"SANS"}
        val result = TestLibrary().primitiveArrayTest(
            value1,
            value2,
            value3,
            value4, value5, value6, value7, value8
        )
        assertEquals("", result)

        assertArrayEquals(ByteArray(3){'b'.toByte()}, value1)
        assertArrayEquals(ShortArray(3){2}, value2)
        assertArrayEquals(IntArray(3){2}, value3)
        assertArrayEquals(LongArray(3){2}, value4)

        assertArrayEquals(FloatArray(3){2.0f}, value5, 0.05f)
        assertArrayEquals(DoubleArray(3){2.0}, value6, 0.05)
        assertArrayEquals(BooleanArray(3){false}, value7)
        assertArrayEquals(Array<String>(3){"PAPYRUS"}, value8)
    }
}