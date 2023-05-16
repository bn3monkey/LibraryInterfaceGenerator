#ifndef __BN3MONKEY_NATIVE_INTERFACE_CONVERTER__
#define __BN3MONKEY_NATIVE_INTERFACE_CONVERTER__

#include <memory>
#include <vector>
#include <functional>
#include <string>
#include <algorithm>
#include <iterator>

#include "../MemoryPool/MemoryPool.hpp"


void initializeNativeInterface();
void releaseNativeInterface();
void addReleaser(void* releaser);

template<typename _NativeType, typename _ManagedType>
class TypeConverter
{
public:
	using NativeType = _NativeType;
	using ManagedType = _ManagedType;

	using NativeTypeArray = std::vector<NativeType>;
	using ManagedTypeArray = std::vector<ManagedType>;

	using NativeTypeVector = std::vector<NativeType>;
	using ManagedTypeVector = std::vector<ManagedType>;

	virtual NativeType toNativeType(const ManagedType& value) = 0;
	virtual ManagedType toManagedType(const NativeType& value) = 0;

	NativeTypeArray toNativeTypeArray(const ManagedTypeArray& values)
	{
		NativeTypeArray ret;
		std::transform(values.begin(), values.end(), std::back_inserter(ret), [&](const ManagedType& value) {
			return toNativeType(value);
			});
		return ret;
	}

	ManagedTypeArray toManagedTypeArray(const NativeTypeArray& values)
	{
		ManagedTypeArray ret;
		std::transform(values.begin(), values.end(), std::back_inserter(ret), [&](const NativeType& value) {
			return toManagedType(value);
			});
		return ret;
	}

	NativeTypeVector toNativeTypeVector(const ManagedTypeVector& values)
	{
		NativeTypeVector ret;
		std::transform(values.begin(), values.end(), std::back_inserter(ret), [&](const ManagedType& value) {
			return toNativeType(value);
			});
		return ret;
	}

	ManagedTypeVector toManagedTypeVector(const NativeTypeVector& values)
	{
		ManagedTypeVector ret;
		std::transform(values.begin(), values.end(), std::back_inserter(ret), [&](const NativeType& value) {
			return toManagedType(value);
			});
		return ret;
	}

	void copyArray(const NativeTypeArray& src, ManagedTypeArray& dest)
	{
		dest.clear();
		std::transform(src.begin(), src.end(), std::back_inserter(dest), [&](const NativeType& value) {
			return toManagedType(value);
			});
	}

	void copyArray(const ManagedTypeArray& src, NativeTypeArray& dest)
	{
		dest.clear();
		std::transform(src.begin(), src.end(), std::back_inserter(dest), [&](const ManagedType& value) {
			return toNativeType(value);
			});
	}

	void copyVector(const NativeTypeVector& src, ManagedTypeVector& dest)
	{
		dest.clear();
		std::transform(src.begin(), src.end(), std::back_inserter(dest), [&](const NativeType& value) {
			return toManagedType(value);
			});
	}

	void copyVector(const ManagedTypeVector& src, NativeTypeVector& dest)
	{
		dest.clear();
		std::transform(src.begin(), src.end(), std::back_inserter(dest), [&](const ManagedType& value) {
			return toNativeType(value);
			});
	}
};

template<typename NativeEnum, typename ManagedEnum = int32_t>
class EnumConverter : public TypeConverter<NativeEnum, ManagedEnum>
{
public:
	using BaseConverter = TypeConverter<NativeEnum, ManagedEnum>;

	BaseConverter::NativeType toNativeType(const BaseConverter::ManagedType& value) override
	{
		return static_cast<BaseConverter::NativeType>(value);
	}
	BaseConverter::ManagedType toManagedType(const BaseConverter::NativeType& value) override
	{
		return static_cast<BaseConverter::ManagedType>(value);
	}

};	

/*
template<typename Type, typename NativeObject = std::shared_ptr<Type>, typename ManagedObject = void*>
class ObjectConverter : public TypeConverter<NativeObject, ManagedObject>
{
public:
	using BaseConverter = TypeConverter<NativeObject, ManagedObject>;

private:
	static size_t shared_ptr_num{ 0 };
	static size_t raw_ptr_num{ 0 };

	BaseConverter::NativeType* createManagedHandle()
	{
#ifdef __BN3MONKEY_MEMORY_POOL
		char ptr_tag[32]{ 0 };
		sprintf(ptr_tag, "ptr_%d", shared_ptr_num++);
		return Bn3Monkey::Bn3MemoryPool::construct<BaseConverter::NativeType>(Bn3Monkey::Bn3Tag(ptr_tag));
#else
		return new BaseConverter::NativeType();
#endif
	}

public:

	template<class... Args>
	BaseConverter::ManagedType construct(Args... args)
	{
		BaseConverter::NativeType* ret = createManagedHandle();

#ifdef __BN3MONKEY_MEMORY_POOL__
		char raw_tag[32]{ 0 };
		sprintf(raw_tag, "raw_%d", raw_ptr_num++);
		*ret = BaseConverter::NativeType(Bn3Monkey::Bn3MemoryPool::construct<Type>(Bn3Monkey::Bn3Tag(raw_tag), args...), [](Type* p) {
			Bn3Monkey::Bn3MemoryPool::destroy<Type>(p);
			});
#else
		*ret = new Type(args...);
#endif
		return reinterpret_cast<ManagedType>(ret);
	}

	BaseConverter::ManagedType clone(const NativeType& src)
	{
		BaseConverter::NativeType* ret = createManagedHandle();
		*ret = src;
		return reinterpret_cast<BaseConverter::ManagedType>(ret);
	}

	void release(const BaseConverter::ManagedType& ref)
	{
		auto* ret = reinterpret_cast<BaseConverter::NativeType*>(ref);
#ifdef __BN3MONKEY_MEMORY_POOL__
		Bn3Monkey::Bn3MemoryPool::destroy(ret);
#else
		delete ret;
#endif
	}

	BaseConverter::NativeType toNativeType(const BaseConverter::ManagedType& value) override
	{
		BaseConverter::NativeType* ref = reinterpret_cast<BaseConverter::NativeType*>(value);
		return *ref;
	}
	BaseConverter::ManagedType toManagedType(const BaseConverter::NativeType& value) override
	{
		return clone(src);
	}
};


template<typename NativeCallback, typename ManagedCallback = void*>
class CallbackConverter : public TypeConverter<NativeCallback, ManagedCallback>
{
public:
	using BaseConverter = TypeConverter<NativeCallback, ManagedCallback>;

	BaseConverter::NativeType toNativeType(const BaseConverter::ManagedType& value) override
	{
		auto callback = reinterpret_cast<BaseConverter::NativeType*>(value);
		return *callback;
	}
	BaseConverter::ManagedType toManagedType(const BaseConverter::NativeType& value) override
	{
		// NOT USED
		return nullptr;
	}

};
*/

#endif