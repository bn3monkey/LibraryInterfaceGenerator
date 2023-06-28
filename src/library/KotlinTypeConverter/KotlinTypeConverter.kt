interface WrapperTypeEnum
{
    fun toWrapperType() : Int
}

inline fun <reified T : WrapperTypeEnum> toKotlinWrapperType(value : T) : Int {
    return value.toWrapperType()
}

@JvmName("toKotlinWrapperEnumArray")
inline fun <reified T : WrapperTypeEnum> toKotlinWrapperType(value : Array<T>) : IntArray {
    return value.map { toKotlinWrapperType(it) }.toIntArray()
}
@JvmName("toKotlinWrapperEnumVector")
inline fun <reified  T : WrapperTypeEnum> toKotlinWrapperType(value : MutableList<T>) : MutableList<Int> {
    return value.map { toKotlinWrapperType(it)}.toMutableList()
}
inline fun <reified T> toKotlinType(value : Int) : T where  T: Enum<T>, T:  WrapperTypeEnum {
    return enumValues<T>().find {
        toKotlinWrapperType(it) == value
    }!!
}
@JvmName("toKotlinEnumArray")
inline fun <reified T> toKotlinType(value : IntArray) : Array<T>  where  T: Enum<T>, T:  WrapperTypeEnum{
    return value.map { toKotlinType<T>(it) }.toTypedArray()
}
@JvmName("toKotlinEnumVector")
inline fun <reified T> toKotlinType(value : MutableList<Int>) : MutableList<T>  where  T: Enum<T>, T: WrapperTypeEnum {
    return value.map { toKotlinType<T>(it) }.toMutableList()
}


abstract class WrapperTypeObject constructor(protected val nativeHandle : NativeHandle) : AutoCloseable {

    data class NativeHandle(var value : Long)
    init {
        require(nativeHandle.value != 0L)
    }

    fun toWrapperType() : Long = nativeHandle.value

    protected val releaser = { close() }
    override fun close() {
        if (0L != nativeHandle.value)
        {
            release(nativeHandle.value)
            nativeHandle.value = 0L
        }
    }

    abstract fun release(value : Long)
}

inline fun <reified T> toKotlinWrapperType(value : T) : Long where T : WrapperTypeObject {
    return value.toWrapperType()
}
@JvmName("toKotlinWrapperObjectArray")
inline fun <reified T> toKotlinWrapperType(value : Array<T>) : LongArray where T : WrapperTypeObject  {
    return value.map { toKotlinWrapperType(it) }.toLongArray()
}
@JvmName("toKotlinWrapperObjectVector")
inline fun <reified T> toKotlinWrapperType(value : MutableList<T>) : MutableList<Long> where T : WrapperTypeObject  {
    return value.map { toKotlinWrapperType(it) }.toMutableList()
}

inline fun <reified T> toKotlinType(value : Long) : T  where T : WrapperTypeObject {
    val constructor = T::class.java.getDeclaredConstructor(WrapperTypeObject.NativeHandle::class.java)
    return constructor.newInstance(WrapperTypeObject.NativeHandle(value))
}

@JvmName("toKotlinObjectArray")
inline fun <reified T> toKotlinType(value : LongArray) : Array<T>  where T : WrapperTypeObject {
    return value.map { toKotlinType<T>(it) }.toTypedArray()
}
@JvmName("toKotlinObjectVector")
inline fun <reified T> toKotlinType(value : MutableList<Long>) : MutableList<T>  where T : WrapperTypeObject {
    return value.map { toKotlinType<T>(it) }.toMutableList()
}