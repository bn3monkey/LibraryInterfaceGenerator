package com.example.kotlintypeconverter

enum class TestEnum(val value : Int) {
    A(2),
    B(4),
    C(6),
    Error(8);

    companion object
    {
        @JvmStatic
        fun toEnum(value : Int) : TestEnum
        {
            return enumValues<TestEnum>().find { it.value == value }!!
        }
    }
}