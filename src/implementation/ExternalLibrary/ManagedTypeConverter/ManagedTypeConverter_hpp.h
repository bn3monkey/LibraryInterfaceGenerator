#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_MANAGEDTYPECONVERTER_HPP__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_MANAGEDTYPECONVERTER_HPP__
static constexpr char* MANAGEDTYPECONVERTER_HPP = 
"#ifndef __BN3MONKEY_MANAGED_TYPE_CONVERTER__\n"
"#define __BN3MONKEY_MANAGED_TYPE_CONVERTER__\n"
"\n"
"#include <memory>\n"
"#include <vector>\n"
"#include <functional>\n"
"#include <string>\n"
"#include <algorithm>\n"
"#include <iterator>\n"
"#include <type_traits>\n"
"#include <array>\n"
"\n"
"#include \"../MemoryPool/MemoryPool.hpp\"\n"
"\n"
"namespace Bn3Monkey\n"
"{\n"
"	class ManagedTypeConverter\n"
"	{\n"
"	public:\n"
"		static bool initialize();\n"
"		static void release();\n"
"		static bool isInitialized();\n"
"		static void addReleaser(std::function<void()> releaser);\n"
"\n"
"		inline static Bn3Monkey::Bn3Tag handleTag() {\n"
"			static int num{ 0 };\n"
"			char tag[32]{ 0 };\n"
"			sprintf(tag, \"managedhandle_%d\", num++);\n"
"			return Bn3Monkey::Bn3Tag(tag);\n"
"		}\n"
"		inline static Bn3Monkey::Bn3Tag objectTag() {\n"
"			static int num{ 0 };\n"
"			char tag[32]{ 0 };\n"
"			sprintf(tag, \"managedobject_%d\", num++);\n"
"			return Bn3Monkey::Bn3Tag(tag);\n"
"\n"
"		}\n"
"	};\n"
"\n"
"	struct MVoid : ManagedTypeConverter\n"
"	{\n"
"		using NativeType = void;\n"
"		using ManagedType = void;\n"
"	};\n"
"\n"
"	struct MBool : ManagedTypeConverter\n"
"	{\n"
"		using NativeType = bool;\n"
"		using ManagedType = bool;\n"
"\n"
"		NativeType toNativeType(const ManagedType& value) {\n"
"			return static_cast<NativeType>(value);\n"
"		}\n"
"		ManagedType toManagedType(const NativeType& value) {\n"
"			return static_cast<ManagedType>(value);\n"
"		}\n"
"	};\n"
"\n"
"	struct MInt8 : ManagedTypeConverter\n"
"	{\n"
"		using NativeType = int8_t;\n"
"		using ManagedType = int8_t;\n"
"\n"
"		NativeType toNativeType(const ManagedType& value) {\n"
"			return static_cast<NativeType>(value);\n"
"		}\n"
"		ManagedType toManagedType(const NativeType& value) {\n"
"			return static_cast<ManagedType>(value);\n"
"		}\n"
"	};\n"
"\n"
"	struct MInt16 : ManagedTypeConverter\n"
"	{\n"
"		using NativeType = int16_t;\n"
"		using ManagedType = int16_t;\n"
"\n"
"		NativeType toNativeType(const ManagedType& value) {\n"
"			return static_cast<NativeType>(value);\n"
"		}\n"
"		ManagedType toManagedType(const NativeType& value) {\n"
"			return static_cast<ManagedType>(value);\n"
"		}\n"
"	};\n"
"\n"
"	struct MInt32 : ManagedTypeConverter\n"
"	{\n"
"		using NativeType = int32_t;\n"
"		using ManagedType = int32_t;\n"
"\n"
"		NativeType toNativeType(const ManagedType& value) {\n"
"			return static_cast<NativeType>(value);\n"
"		}\n"
"		ManagedType toManagedType(const NativeType& value) {\n"
"			return static_cast<ManagedType>(value);\n"
"		}\n"
"	};\n"
"\n"
"	struct MInt64 : ManagedTypeConverter\n"
"	{\n"
"		using NativeType = int64_t;\n"
"		using ManagedType = int64_t;\n"
"\n"
"		NativeType toNativeType(const ManagedType& value) {\n"
"			return static_cast<NativeType>(value);\n"
"		}\n"
"		ManagedType toManagedType(const NativeType& value) {\n"
"			return static_cast<ManagedType>(value);\n"
"		}\n"
"	};\n"
"\n"
"	struct MFloat : ManagedTypeConverter\n"
"	{\n"
"		using NativeType = float;\n"
"		using ManagedType = float;\n"
"\n"
"		NativeType toNativeType(const ManagedType& value) {\n"
"			return static_cast<NativeType>(value);\n"
"		}\n"
"		ManagedType toManagedType(const NativeType& value) {\n"
"			return static_cast<ManagedType>(value);\n"
"		}\n"
"	};\n"
"\n"
"	struct MDouble : ManagedTypeConverter\n"
"	{\n"
"		using NativeType = double;\n"
"		using ManagedType = double;\n"
"\n"
"		NativeType toNativeType(const ManagedType& value) {\n"
"			return static_cast<NativeType>(value);\n"
"		}\n"
"		ManagedType toManagedType(const NativeType& value) {\n"
"			return static_cast<ManagedType>(value);\n"
"		}\n"
"	};\n"
"\n"
"	struct MString : ManagedTypeConverter\n"
"	{\n"
"		using NativeType = std::string;\n"
"		using ManagedType = std::string;\n"
"\n"
"		NativeType toNativeType(const ManagedType& value) {\n"
"			return static_cast<NativeType>(value);\n"
"		}\n"
"		ManagedType toManagedType(const NativeType& value) {\n"
"			return static_cast<ManagedType>(value);\n"
"		}\n"
"	};\n"
"\n"
"	// \n"
"	template<typename _NativeEnum>\n"
"	struct MEnum : ManagedTypeConverter\n"
"	{\n"
"		static_assert(std::is_enum_v<_NativeEnum>, \"MEnum can receive only Enum as a template argument.\");\n"
"\n"
"		using NativeType = _NativeEnum;\n"
"		using ManagedType = int32_t;\n"
"\n"
"		NativeType toNativeType(const ManagedType& value) {\n"
"			return static_cast<NativeType>(value);\n"
"		}\n"
"		ManagedType toManagedType(const NativeType& value) {\n"
"			return static_cast<int32_t>(value);\n"
"		}\n"
"	};\n"
"\n"
"	template<typename _NativeObject>\n"
"	struct MObject : ManagedTypeConverter\n"
"	{\n"
"		static_assert(std::is_class_v<_NativeObject>, \"MObject can receive only object as a template argument.\");\n"
"\n"
"		using NativeObject = _NativeObject;\n"
"		using NativeType = std::shared_ptr<NativeObject>;\n"
"		using ManagedType = void*;\n"
"\n"
"		NativeType toNativeType(const ManagedType& value) {\n"
"			NativeType* ref = reinterpret_cast<NativeType*>(value);\n"
"			return *ref;\n"
"		}\n"
"		ManagedType toManagedType(const NativeType& value) {\n"
"			return clone(value);\n"
"		}\n"
"\n"
"		template<class... Args>\n"
"		ManagedType construct(Args... args)\n"
"		{\n"
"			NativeType* ret = createManagedHandle();\n"
"\n"
"#ifdef __BN3MONKEY_MEMORY_POOL__\n"
"			* ret = NativeType(Bn3Monkey::Bn3MemoryPool::construct<NativeObject>(ManagedTypeConverter::objectTag(), std::forward<Args>(args)...), [](NativeObject* p) {\n"
"				Bn3Monkey::Bn3MemoryPool::destroy<NativeObject>(p);\n"
"				});\n"
"#else\n"
"			*ret = new NativeObject(args...);\n"
"#endif\n"
"			return reinterpret_cast<ManagedType>(ret);\n"
"		}\n"
"\n"
"		ManagedType clone(const NativeType& src)\n"
"		{\n"
"			NativeType* ret = createManagedHandle();\n"
"			*ret = src;\n"
"			return reinterpret_cast<ManagedType>(ret);\n"
"		}\n"
"\n"
"		void release(const ManagedType& ref)\n"
"		{\n"
"			NativeType* ret = reinterpret_cast<NativeType*>(ref);\n"
"#ifdef __BN3MONKEY_MEMORY_POOL__\n"
"			Bn3Monkey::Bn3MemoryPool::destroy(ret);\n"
"#else\n"
"			delete ret;\n"
"#endif\n"
"		}\n"
"\n"
"\n"
"	private:\n"
"\n"
"		NativeType* createManagedHandle()\n"
"		{\n"
"#ifdef __BN3MONKEY_MEMORY_POOL__\n"
"			return Bn3Monkey::Bn3MemoryPool::construct<NativeType>(ManagedTypeConverter::handleTag());\n"
"#else\n"
"			return new NativeObject();\n"
"#endif\n"
"		}\n"
"	};\n"
"\n"
"	template<typename ManagedReturnTypeConverter, typename... ManagedArgTypeConverter>\n"
"	struct MCallback : ManagedTypeConverter\n"
"	{\n"
"		using NativeType = std::function<typename ManagedReturnTypeConverter::NativeType(typename ManagedArgTypeConverter::NativeType...)>;\n"
"		using ManagedType = std::function<typename ManagedReturnTypeConverter::ManagedType(typename ManagedArgTypeConverter::ManagedType...)>;\n"
"\n"
"		NativeType toNativeType(const ManagedType& value)\n"
"		{\n"
"			return [value](typename ManagedArgTypeConverter::NativeType... args)\n"
"			{\n"
"				auto params = createArgTypes<ManagedArgTypeConverter...>(args...);\n"
"				\n"
"				if constexpr (std::is_same_v<MVoid, ManagedReturnTypeConverter>)\n"
"				{\n"
"					apply(value, params);\n"
"				}\n"
"				else\n"
"				{\n"
"					auto ret = apply(value, params);\n"
"					return ManagedReturnTypeConverter().toNativeType(ret);\n"
"				}\n"
"			};\n"
"		}\n"
"\n"
"	private:\n"
"		template <class... ManagedTypeConverters>\n"
"		static inline std::tuple<typename ManagedTypeConverters::ManagedType...> createArgTypes(typename ManagedTypeConverters::NativeType... args)\n"
"		{\n"
"			return std::make_tuple(ManagedTypeConverters().toManagedType(args)...);\n"
"		}\n"
"\n"
"		template <typename Function, typename Tuple, size_t... Indices>\n"
"		static inline constexpr decltype(auto) apply_impl(Function&& func, Tuple&& args, std::index_sequence<Indices...>)\n"
"		{\n"
"			return std::forward<Function>(func)(std::get<Indices>(std::forward<Tuple>(args))...);\n"
"		}\n"
"\n"
"		// Function to apply a function to the elements of a tuple\n"
"		template <typename Function, typename Tuple>\n"
"		static inline auto apply(Function&& func, Tuple&& args)\n"
"		{\n"
"			constexpr size_t Size = std::tuple_size<std::decay_t<Tuple>>::value;\n"
"			return apply_impl(std::forward<Function>(func), std::forward<Tuple>(args), std::make_index_sequence<Size>());\n"
"		}\n"
"\n"
"	};\n"
"\n"
"	template<class TypeConverter, size_t size, typename std::enable_if_t<std::is_base_of_v<ManagedTypeConverter, TypeConverter>>* = nullptr>\n"
"	struct MArray\n"
"	{\n"
"		using NativeType = std::array<typename TypeConverter::NativeType, size>;\n"
"		using ManagedType = std::array<typename TypeConverter::ManagedType, size>;\n"
"\n"
"		NativeType toNativeType(const ManagedType& values)\n"
"		{\n"
"			NativeType ret;\n"
"			for (size_t i = 0; i < size; i++)\n"
"			{\n"
"				auto& value = values[i];\n"
"				auto element = TypeConverter().toNativeType(value);\n"
"				ret[i] = element;\n"
"			}\n"
"			return ret;\n"
"		}\n"
"\n"
"		ManagedType toManagedType(const NativeType& values)\n"
"		{\n"
"			ManagedType ret;\n"
"			for (size_t i = 0; i < size; i++)\n"
"			{\n"
"				auto& value = values[i];\n"
"				auto element = TypeConverter().toManagedType(value);\n"
"				ret[i] = element;\n"
"			}\n"
"			return ret;\n"
"		}\n"
"\n"
"		void copy(const NativeType& src, ManagedType& dest)\n"
"		{\n"
"			for (size_t i = 0; i < size; i++)\n"
"			{\n"
"				auto& value = src[i];\n"
"				auto element = TypeConverter().toManagedType(value);\n"
"				dest[i] = element;\n"
"			}\n"
"		}\n"
"	};\n"
"\n"
"	template<class TypeConverter, typename std::enable_if_t<std::is_base_of_v<ManagedTypeConverter, TypeConverter>>* = nullptr>\n"
"	struct MVector\n"
"	{\n"
"		using NativeType = std::vector<typename TypeConverter::NativeType>;\n"
"		using ManagedType = std::vector<typename TypeConverter::ManagedType>;\n"
"\n"
"		NativeType toNativeType(const ManagedType& values)\n"
"		{\n"
"			NativeType ret;\n"
"			ret.reserve(values.size());\n"
"			for (auto& value : values)\n"
"			{\n"
"				auto element = TypeConverter().toNativeType(value);\n"
"				ret.push_back(element);\n"
"			}\n"
"			return ret;\n"
"		}\n"
"\n"
"		ManagedType toManagedType(const NativeType& values)\n"
"		{\n"
"			ManagedType ret;\n"
"			ret.reserve(values.size());\n"
"			for (auto& value : values)\n"
"			{\n"
"				auto element = TypeConverter().toManagedType(value);\n"
"				ret.push_back(element);\n"
"			}\n"
"			return ret;\n"
"		}\n"
"\n"
"		void copy(const NativeType& src, ManagedType& dest)\n"
"		{\n"
"			dest.clear();\n"
"			dest.reserve(src.size());\n"
"			for (auto& value : src)\n"
"			{\n"
"				auto element = TypeConverter().toManagedType(value);\n"
"				dest.push_back(element);\n"
"			}\n"
"		}\n"
"	};\n"
"}\n"
"\n"
"#endif\n";
#endif //__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_MANAGEDTYPECONVERTER_HPP__
