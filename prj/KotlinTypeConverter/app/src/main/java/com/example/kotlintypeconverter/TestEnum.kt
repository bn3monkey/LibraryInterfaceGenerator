package com.example.kotlintypeconverter
enum class TestEnum(val value : Int) : WrapperTypeEnum {
    A(2),
    B(4),
    C(6),
    Error(8);

    override fun toWrapperType() : Int{
        return value
    }

}
