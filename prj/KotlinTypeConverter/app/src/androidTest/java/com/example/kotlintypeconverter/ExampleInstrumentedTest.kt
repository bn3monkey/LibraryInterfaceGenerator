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
    @Test
    fun primiteveVectorTest()
    {
        val value1 = MutableList<Byte>(3) { _ -> 'a'.toByte() }
        val value2 = MutableList<Short>(3) {1}
        val value3 = MutableList<Int>(3) {1}
        val value4 = MutableList<Long>(3) {1}
        val value5 = MutableList<Float>(3) {1.0f}
        val value6 = MutableList<Double>(3) {1.0}
        val value7 = MutableList<Boolean>(3) {false}
        val value8 = MutableList<String>(3) {"SANS"}
        val result = TestLibrary().primitiveVectorTest(
            value1,
            value2,
            value3,
            value4, value5, value6, value7, value8
        )
        assertEquals("", result)

        assertEquals(
            value1, mutableListOf<Byte>(
                'a'.toByte(),
                'a'.toByte(),
                'a'.toByte(),
                'b'.toByte(),
                'b'.toByte()
            ))
        assertEquals(value2, mutableListOf<Short>(1, 1, 1, 2, 2))
        assertEquals(value3, mutableListOf<Int>(1, 1, 1, 2, 2))
        assertEquals(value4, mutableListOf<Long>(1, 1, 1, 2, 2))
        assertEquals(value5, mutableListOf<Float>(1.0f, 1.0f, 1.0f, 2.0f, 2.0f))
        assertEquals(value6, mutableListOf<Double>(1.0, 1.0, 1.0, 2.0, 2.0))
        assertEquals(value7, mutableListOf<Boolean>(false, false, false, true, true))
        assertEquals(value8, mutableListOf<String>("SANS", "SANS", "SANS", "PAYPRUS", "PAPYRUS"))

    }

    @Test
    fun enumTest()
    {
        val value1 = TestEnum.A;
        val value2 = arrayOf(TestEnum.A, TestEnum.B, TestEnum.C);
        val value3= mutableListOf(TestEnum.A, TestEnum.B, TestEnum.C);

        val ret = TestLibrary().enumTest(
            value1,
            value2,
            value3
        )

        assertEquals(TestEnum.A, ret)
        assertArrayEquals(arrayOf(TestEnum.C, TestEnum.C, TestEnum.C), value2)
        assertEquals(mutableListOf(TestEnum.A, TestEnum.B, TestEnum.C, TestEnum.A, TestEnum.A), value3)
    }

    @Test
    fun ObjectLifeCycleTest() {
        TestObject(2).use {
            obj ->
            val ret = obj.getValue()
            assertEquals(ret, 2)

            TestObject(3).use {
                obj1 ->
                assertEquals(obj1.getValue(), 3)

                TestObject(4).use {
                    obj2 ->
                    assertEquals(obj2.getValue(), 4)

                    val obj3 = obj.sum(obj1, obj2)
                    assertEquals(obj3.getValue(), 7)

                }
            }
        }
    }

    @Test
    fun ObjectTest()
    {
        val value1 = TestObject(1)
        val value2 = arrayOf(TestObject(2), TestObject(3), TestObject(4))
        val value3 = mutableListOf(TestObject(5), TestObject(6), TestObject(7))

        val ret = TestLibrary().objectTest(value1, value2, value3)

        assertEquals(value2[0].getValue(), 10)
        assertEquals(value2[1].getValue(), 20)
        assertEquals(value2[2].getValue(), 30)

        assertEquals(value3[3].getValue(), 10)
        assertEquals(value3[4].getValue(), 10)
    }

    @Test
    fun callbackTest()
    {
        val voidCallback = {
            value1 : Int, value2 : String, value3 : TestObject, value4 : MutableList<String> ->

            assertEquals(value1, 1)
            assertEquals(value2, "SANS")
            assertEquals(value3.getValue(), 2)
            assertEquals(value4, mutableListOf("PAPYRUS", "PAPYRUS"))
        }

        val booleanCallback = {
            value1 : Int, value2 : Float ->

            assertEquals(value1, 1)
            assertEquals(value2, 1.0f)
            true
        }

        val byteCallback = {
                value1 : Int, value2 : Float ->

            assertEquals(value1, 1)
            assertEquals(value2, 1.0f)
            'a'.toByte()
        }

        val shortCallback = {
            value1 : Int, value2 : Float ->

            assertEquals(value1, 1)
            assertEquals(value2, 1.0f)
            Short.MAX_VALUE
        }

        val intCallback = {
            value1 : Int, value2 : Float ->

            assertEquals(value1, 1)
            assertEquals(value2, 1.0f)
            Int.MAX_VALUE
        }

        val longCallback = {
                value1 : Int, value2 : Float ->

            assertEquals(value1, 1)
            assertEquals(value2, 1.0f)
            Long.MAX_VALUE
        }

        val floatCallback = {
                value1 : Int, value2 : Float ->

            assertEquals(value1, 1)
            assertEquals(value2, 1.0f)
            Float.MAX_VALUE
        }

        val doubleCallback = {
                value1 : Int, value2 : Float ->

            assertEquals(value1, 1)
            assertEquals(value2, 1.0f)
            Double.MAX_VALUE
        }

        val stringCallback = {
                value1 : Int, value2 : Float ->

            assertEquals(value1, 1)
            assertEquals(value2, 1.0f)
            "SANS"
        }

        val enumCallback = {
                value1 : Int, value2 : Float ->

            assertEquals(value1, 1)
            assertEquals(value2, 1.0f)
            TestEnum.B
        }


        val objectCallback = {
                value1 : Int, value2 : Float ->

            assertEquals(value1, 1)
            assertEquals(value2, 1.0f)
            TestObject(1)
        }

        val ret = TestLibrary().callbackTest(voidCallback, booleanCallback, byteCallback, shortCallback, intCallback, longCallback, floatCallback, doubleCallback, stringCallback, enumCallback, objectCallback)
        assertEquals(ret, true)
    }
}