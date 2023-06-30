
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

interface WrapperTypeObject
{
    fun toWrapperType() : Long
}

abstract class WrapperTypeObjectImpl(private val nativeHandle : NativeHandle) : WrapperTypeObject, AutoCloseable {

    class NativeHandle(private var value : Long) {
        fun isValid() : Boolean = value != 0L
        fun invalidate() { value = 0L }
        fun get() : Long { return value }
    }
    init {
        require(nativeHandle.isValid())
    }
    abstract fun release(handle : NativeHandle)

    override fun toWrapperType() : Long = nativeHandle.get()
    protected var self = nativeHandle.get()

    protected val releaser = { close() }
    override fun close() {
        if (nativeHandle.isValid())
        {
            release(nativeHandle)
            nativeHandle.invalidate()
        }
    }

}

@JvmName("toKotlinWrapperObject")
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

@JvmName("toKotlinObject")
inline fun <reified T> toKotlinType(value : Long) : T  where T : WrapperTypeObject {
    val constructor = T::class.java.getDeclaredConstructor(WrapperTypeObjectImpl.NativeHandle::class.java)
    return constructor.newInstance(WrapperTypeObjectImpl.NativeHandle(value))
}

@JvmName("toKotlinObjectArray")
inline fun <reified T> toKotlinType(value : LongArray) : Array<T>  where T : WrapperTypeObject {
    return value.map { toKotlinType<T>(it) }.toTypedArray()
}
@JvmName("toKotlinObjectVector")
inline fun <reified T> toKotlinType(value : MutableList<Long>) : MutableList<T>  where T : WrapperTypeObject {
    return value.map { toKotlinType<T>(it) }.toMutableList()
}



inline fun <reified T> toKotlinWrapperType(value : T) : T {
    return value
}
inline fun <reified T> toKotlinType(value : T) : T {
    return value
}