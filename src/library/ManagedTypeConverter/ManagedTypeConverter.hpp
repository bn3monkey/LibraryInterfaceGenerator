#ifndef __BN3MONKEY_MANAGED_TYPE_CONVERTER__
#define __BN3MONKEY_MANAGED_TYPE_CONVERTER__

#include <memory>
#include <vector>
#include <functional>
#include <string>
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <array>

#include "../MemoryPool/MemoryPool.hpp"

namespace Bn3Monkey
{
	class ManagedTypeConverter
	{
	public:
		static bool initialize();
		static void release();
		static bool isInitialized();
		static void addReleaser(std::function<void()> releaser);

		inline static Bn3Monkey::Bn3Tag handleTag() {
			static int num{ 0 };
			char tag[32]{ 0 };
			sprintf(tag, "managedhandle_%d", num++);
			return Bn3Monkey::Bn3Tag(tag);
		}
		inline static Bn3Monkey::Bn3Tag objectTag() {
			static int num{ 0 };
			char tag[32]{ 0 };
			sprintf(tag, "managedobject_%d", num++);
			return Bn3Monkey::Bn3Tag(tag);

		}
	};

	struct MVoid : ManagedTypeConverter
	{
		using NativeType = void;
		using ManagedType = void;
	};

	struct MBool : ManagedTypeConverter
	{
		using NativeType = bool;
		using ManagedType = bool;

		NativeType toNativeType(const ManagedType& value) {
			return static_cast<NativeType>(value);
		}
		ManagedType toManagedType(const NativeType& value) {
			return static_cast<ManagedType>(value);
		}
	};

	struct MInt8 : ManagedTypeConverter
	{
		using NativeType = int8_t;
		using ManagedType = int8_t;

		NativeType toNativeType(const ManagedType& value) {
			return static_cast<NativeType>(value);
		}
		ManagedType toManagedType(const NativeType& value) {
			return static_cast<ManagedType>(value);
		}
	};

	struct MInt16 : ManagedTypeConverter
	{
		using NativeType = int16_t;
		using ManagedType = int16_t;

		NativeType toNativeType(const ManagedType& value) {
			return static_cast<NativeType>(value);
		}
		ManagedType toManagedType(const NativeType& value) {
			return static_cast<ManagedType>(value);
		}
	};

	struct MInt32 : ManagedTypeConverter
	{
		using NativeType = int32_t;
		using ManagedType = int32_t;

		NativeType toNativeType(const ManagedType& value) {
			return static_cast<NativeType>(value);
		}
		ManagedType toManagedType(const NativeType& value) {
			return static_cast<ManagedType>(value);
		}
	};

	struct MInt64 : ManagedTypeConverter
	{
		using NativeType = int64_t;
		using ManagedType = int64_t;

		NativeType toNativeType(const ManagedType& value) {
			return static_cast<NativeType>(value);
		}
		ManagedType toManagedType(const NativeType& value) {
			return static_cast<ManagedType>(value);
		}
	};

	struct MFloat : ManagedTypeConverter
	{
		using NativeType = float;
		using ManagedType = float;

		NativeType toNativeType(const ManagedType& value) {
			return static_cast<NativeType>(value);
		}
		ManagedType toManagedType(const NativeType& value) {
			return static_cast<ManagedType>(value);
		}
	};

	struct MDouble : ManagedTypeConverter
	{
		using NativeType = double;
		using ManagedType = double;

		NativeType toNativeType(const ManagedType& value) {
			return static_cast<NativeType>(value);
		}
		ManagedType toManagedType(const NativeType& value) {
			return static_cast<ManagedType>(value);
		}
	};

	struct MString : ManagedTypeConverter
	{
		using NativeType = std::string;
		using ManagedType = std::string;

		NativeType toNativeType(const ManagedType& value) {
			return static_cast<NativeType>(value);
		}
		ManagedType toManagedType(const NativeType& value) {
			return static_cast<ManagedType>(value);
		}
	};

	// 
	template<typename _NativeEnum>
	struct MEnum : ManagedTypeConverter
	{
		static_assert(std::is_enum_v<_NativeEnum>, "MEnum can receive only Enum as a template argument.");

		using NativeType = _NativeEnum;
		using ManagedType = int32_t;

		NativeType toNativeType(const ManagedType& value) {
			return static_cast<NativeType>(value);
		}
		ManagedType toManagedType(const NativeType& value) {
			return static_cast<int32_t>(value);
		}
	};

	template<typename _NativeObject>
	struct MObject : ManagedTypeConverter
	{
		static_assert(std::is_class_v<_NativeObject>, "MObject can receive only object as a template argument.");

		using NativeObject = _NativeObject;
		using NativeType = std::shared_ptr<NativeObject>;
		using ManagedType = void*;

		NativeType toNativeType(const ManagedType& value) {
			NativeType* ref = reinterpret_cast<NativeType*>(value);
			return *ref;
		}
		ManagedType toManagedType(const NativeType& value) {
			return clone(value);
		}

		template<class... Args>
		ManagedType construct(Args... args)
		{
			NativeType* ret = createManagedHandle();

#ifdef __BN3MONKEY_MEMORY_POOL__
			* ret = NativeType(Bn3Monkey::Bn3MemoryPool::construct<NativeObject>(ManagedTypeConverter::objectTag(), std::forward<Args>(args)...), [](NativeObject* p) {
				Bn3Monkey::Bn3MemoryPool::destroy<NativeObject>(p);
				});
#else
			*ret = new NativeObject(args...);
#endif
			return reinterpret_cast<ManagedType>(ret);
		}

		ManagedType clone(const NativeType& src)
		{
			NativeType* ret = createManagedHandle();
			*ret = src;
			return reinterpret_cast<ManagedType>(ret);
		}

		void release(const ManagedType& ref)
		{
			NativeType* ret = reinterpret_cast<NativeType*>(ref);
#ifdef __BN3MONKEY_MEMORY_POOL__
			Bn3Monkey::Bn3MemoryPool::destroy(ret);
#else
			delete ret;
#endif
		}


	private:

		NativeType* createManagedHandle()
		{
#ifdef __BN3MONKEY_MEMORY_POOL__
			return Bn3Monkey::Bn3MemoryPool::construct<NativeType>(ManagedTypeConverter::handleTag());
#else
			return new NativeObject();
#endif
		}
	};

	template<typename ManagedReturnTypeConverter, typename... ManagedArgTypeConverter>
	struct MCallback : ManagedTypeConverter
	{
		using NativeType = std::function<typename ManagedReturnTypeConverter::NativeType(typename ManagedArgTypeConverter::NativeType...)>;
		using ManagedType = std::function<typename ManagedReturnTypeConverter::ManagedType(typename ManagedArgTypeConverter::ManagedType...)>;

		NativeType toNativeType(const ManagedType& value)
		{
			return [value](typename ManagedArgTypeConverter::NativeType... args)
			{
				auto params = createArgTypes<ManagedArgTypeConverter...>(args...);
				
				if constexpr (std::is_same_v<MVoid, ManagedReturnTypeConverter>)
				{
					apply(value, params);
				}
				else
				{
					auto ret = apply(value, params);
					return ManagedReturnTypeConverter().toNativeType(ret);
				}
			};
		}

	private:
		template <class... ManagedTypeConverters>
		static inline std::tuple<typename ManagedTypeConverters::ManagedType...> createArgTypes(typename ManagedTypeConverters::NativeType... args)
		{
			return std::make_tuple(ManagedTypeConverters().toManagedType(args)...);
		}

		template <typename Function, typename Tuple, size_t... Indices>
		static inline constexpr decltype(auto) apply_impl(Function&& func, Tuple&& args, std::index_sequence<Indices...>)
		{
			return std::forward<Function>(func)(std::get<Indices>(std::forward<Tuple>(args))...);
		}

		// Function to apply a function to the elements of a tuple
		template <typename Function, typename Tuple>
		static inline auto apply(Function&& func, Tuple&& args)
		{
			constexpr size_t Size = std::tuple_size<std::decay_t<Tuple>>::value;
			return apply_impl(std::forward<Function>(func), std::forward<Tuple>(args), std::make_index_sequence<Size>());
		}

	};

	template<class TypeConverter, size_t size, typename std::enable_if_t<std::is_base_of_v<ManagedTypeConverter, TypeConverter>>* = nullptr>
	struct MArray
	{
		using NativeType = std::array<typename TypeConverter::NativeType, size>;
		using ManagedType = std::array<typename TypeConverter::ManagedType, size>;

		NativeType toNativeType(const ManagedType& values)
		{
			NativeType ret;
			for (size_t i = 0; i < size; i++)
			{
				auto& value = values[i];
				auto element = TypeConverter().toNativeType(value);
				ret[i] = element;
			}
			return ret;
		}

		ManagedType toManagedType(const NativeType& values)
		{
			ManagedType ret;
			for (size_t i = 0; i < size; i++)
			{
				auto& value = values[i];
				auto element = TypeConverter().toManagedType(value);
				ret[i] = element;
			}
			return ret;
		}

		void copy(const NativeType& src, ManagedType& dest)
		{
			for (size_t i = 0; i < size; i++)
			{
				auto& value = src[i];
				auto element = TypeConverter().toManagedType(value);
				dest[i] = element;
			}
		}
	};

	template<class TypeConverter, typename std::enable_if_t<std::is_base_of_v<ManagedTypeConverter, TypeConverter>>* = nullptr>
	struct MVector
	{
		using NativeType = std::vector<typename TypeConverter::NativeType>;
		using ManagedType = std::vector<typename TypeConverter::ManagedType>;

		NativeType toNativeType(const ManagedType& values)
		{
			NativeType ret;
			ret.reserve(values.size());
			for (auto& value : values)
			{
				auto element = TypeConverter().toNativeType(value);
				ret.push_back(element);
			}
			return ret;
		}

		ManagedType toManagedType(const NativeType& values)
		{
			ManagedType ret;
			ret.reserve(values.size());
			for (auto& value : values)
			{
				auto element = TypeConverter().toManagedType(value);
				ret.push_back(element);
			}
			return ret;
		}

		void copy(const NativeType& src, ManagedType& dest)
		{
			dest.clear();
			dest.reserve(src.size());
			for (auto& value : src)
			{
				auto element = TypeConverter().toManagedType(value);
				dest.push_back(element);
			}
		}
	};
}

#endif