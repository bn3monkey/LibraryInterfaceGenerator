#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_KOTLINTYPECONVERTER_KT__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_KOTLINTYPECONVERTER_KT__
static constexpr char* KOTLINTYPECONVERTER_KT = 
"\n"
"interface WrapperTypeEnum\n"
"{\n"
"    fun toWrapperType() : Int\n"
"}\n"
"\n"
"inline fun <reified T : WrapperTypeEnum> toKotlinWrapperType(value : T) : Int {\n"
"    return value.toWrapperType()\n"
"}\n"
"\n"
"@JvmName(\"toKotlinWrapperEnumArray\")\n"
"inline fun <reified T : WrapperTypeEnum> toKotlinWrapperType(value : Array<T>) : IntArray {\n"
"    return value.map { toKotlinWrapperType(it) }.toIntArray()\n"
"}\n"
"@JvmName(\"toKotlinWrapperEnumVector\")\n"
"inline fun <reified  T : WrapperTypeEnum> toKotlinWrapperType(value : MutableList<T>) : MutableList<Int> {\n"
"    return value.map { toKotlinWrapperType(it)}.toMutableList()\n"
"}\n"
"inline fun <reified T> toKotlinType(value : Int) : T where  T: Enum<T>, T:  WrapperTypeEnum {\n"
"    return enumValues<T>().find {\n"
"        toKotlinWrapperType(it) == value\n"
"    }!!\n"
"}\n"
"@JvmName(\"toKotlinEnumArray\")\n"
"inline fun <reified T> toKotlinType(value : IntArray) : Array<T>  where  T: Enum<T>, T:  WrapperTypeEnum{\n"
"    return value.map { toKotlinType<T>(it) }.toTypedArray()\n"
"}\n"
"@JvmName(\"toKotlinEnumVector\")\n"
"inline fun <reified T> toKotlinType(value : MutableList<Int>) : MutableList<T>  where  T: Enum<T>, T: WrapperTypeEnum {\n"
"    return value.map { toKotlinType<T>(it) }.toMutableList()\n"
"}\n"
"\n"
"interface WrapperTypeObject\n"
"{\n"
"    fun toWrapperType() : Long\n"
"}\n"
"\n"
"abstract class WrapperTypeObjectImpl(private val nativeHandle : NativeHandle) : WrapperTypeObject, AutoCloseable {\n"
"\n"
"    class NativeHandle(private var value : Long) {\n"
"        fun isValid() : Boolean = value != 0L\n"
"        fun invalidate() { value = 0L }\n"
"        fun get() : Long { return value }\n"
"    }\n"
"    init {\n"
"        require(nativeHandle.isValid())\n"
"    }\n"
"    abstract fun release(handle : NativeHandle)\n"
"\n"
"    override fun toWrapperType() : Long = nativeHandle.get()\n"
"    protected var self = nativeHandle.get()\n"
"\n"
"    protected val releaser = { close() }\n"
"    override fun close() {\n"
"        if (nativeHandle.isValid())\n"
"        {\n"
"            release(nativeHandle)\n"
"            nativeHandle.invalidate()\n"
"        }\n"
"    }\n"
"\n"
"}\n"
"\n"
"@JvmName(\"toKotlinWrapperObject\")\n"
"inline fun <reified T> toKotlinWrapperType(value : T) : Long where T : WrapperTypeObject {\n"
"    return value.toWrapperType()\n"
"}\n"
"@JvmName(\"toKotlinWrapperObjectArray\")\n"
"inline fun <reified T> toKotlinWrapperType(value : Array<T>) : LongArray where T : WrapperTypeObject  {\n"
"    return value.map { toKotlinWrapperType(it) }.toLongArray()\n"
"}\n"
"@JvmName(\"toKotlinWrapperObjectVector\")\n"
"inline fun <reified T> toKotlinWrapperType(value : MutableList<T>) : MutableList<Long> where T : WrapperTypeObject  {\n"
"    return value.map { toKotlinWrapperType(it) }.toMutableList()\n"
"}\n"
"\n"
"@JvmName(\"toKotlinObject\")\n"
"inline fun <reified T> toKotlinType(value : Long) : T  where T : WrapperTypeObject {\n"
"    val constructor = T::class.java.getDeclaredConstructor(WrapperTypeObjectImpl.NativeHandle::class.java)\n"
"    return constructor.newInstance(WrapperTypeObjectImpl.NativeHandle(value))\n"
"}\n"
"\n"
"@JvmName(\"toKotlinObjectArray\")\n"
"inline fun <reified T> toKotlinType(value : LongArray) : Array<T>  where T : WrapperTypeObject {\n"
"    return value.map { toKotlinType<T>(it) }.toTypedArray()\n"
"}\n"
"@JvmName(\"toKotlinObjectVector\")\n"
"inline fun <reified T> toKotlinType(value : MutableList<Long>) : MutableList<T>  where T : WrapperTypeObject {\n"
"    return value.map { toKotlinType<T>(it) }.toMutableList()\n"
"}\n"
"\n"
"\n"
"\n"
"inline fun <reified T> toKotlinWrapperType(value : T) : T {\n"
"    return value\n"
"}\n"
"inline fun <reified T> toKotlinType(value : T) : T {\n"
"    return value\n"
"}\n"
"inline fun <reified T> toKotlinType(value : Array<T>) : Array<T>{\n"
"    return value\n"
"}\n"
"inline fun <reified T> toKotlinType(value : MutableList<T>) : MutableList<T>{\n"
"    return value\n"
"}\n";

#endif //__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_KOTLINTYPECONVERTER_KT__
