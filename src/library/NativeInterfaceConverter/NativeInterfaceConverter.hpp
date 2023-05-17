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
protected:
#ifdef __BN3MONKEY_MEMORY_POOL__
	Bn3Monkey::Bn3Tag handleTag() {
		static int num{ 0 };
		char tag[32]{ 0 };
		sprintf(tag, "managedhandle_%d", num++);
		return Bn3Monkey::Bn3Tag(tag);
	}
	Bn3Monkey::Bn3Tag objectTag() {
		static int num{ 0 };
		char tag[32]{ 0 };
		sprintf(tag, "managedobject_%d", num++);
		return Bn3Monkey::Bn3Tag(tag);

	}
#endif
};

template<typename NativeEnum, typename ManagedEnum = int32_t>
class EnumConverter : public TypeConverter<NativeEnum, ManagedEnum>
{
public:
	using BaseConverter = TypeConverter<NativeEnum, ManagedEnum>;
	using NativeType = typename BaseConverter::NativeType;
	using ManagedType = typename BaseConverter::ManagedType;

	NativeType toNativeType(const ManagedType& value) override
	{
		return static_cast<NativeType>(value);
	}
	ManagedType toManagedType(const NativeType& value) override
	{
		return static_cast<ManagedType>(value);
	}

};	

template<typename Type, typename NativeObject = std::shared_ptr<Type>, typename ManagedObject = void*>
class ObjectConverter : public TypeConverter<NativeObject, ManagedObject>
{
public:
	using BaseConverter = TypeConverter<NativeObject, ManagedObject>;
	using NativeType = typename BaseConverter::NativeType;
	using ManagedType = typename BaseConverter::ManagedType;

	
	NativeType toNativeType(const ManagedType& value) override
	{
		NativeType* ref = reinterpret_cast<NativeType*>(value);
		return *ref;
	}

	ManagedType toManagedType(const NativeType& value) override
	{
		return clone(value);
	}

	template<class... Args>
	ManagedObject construct(Args... args)
	{
		NativeObject* ret = createManagedHandle();

#ifdef __BN3MONKEY_MEMORY_POOL__
		*ret = NativeObject(Bn3Monkey::Bn3MemoryPool::construct<Type>(BaseConverter::objectTag(), std::forward<Args>(args)...), [](Type* p) {
			Bn3Monkey::Bn3MemoryPool::destroy<Type>(p);
			});
#else
		*ret = new Type(args...);
#endif
		return reinterpret_cast<ManagedObject>(ret);
	}

	ManagedObject clone(const NativeObject& src)
	{
		NativeObject* ret = createManagedHandle();
		*ret = src;
		return reinterpret_cast<ManagedObject>(ret);
	}

	void release(const ManagedObject& ref)
	{
		NativeObject* ret = reinterpret_cast<NativeObject*>(ref);
#ifdef __BN3MONKEY_MEMORY_POOL__
		Bn3Monkey::Bn3MemoryPool::destroy(ret);
#else
		delete ret;
#endif
	}
		

private:

	NativeObject* createManagedHandle()
	{
#ifdef __BN3MONKEY_MEMORY_POOL__		
		return Bn3Monkey::Bn3MemoryPool::construct<NativeObject>(BaseConverter::handleTag());
#else
		return new NativeObject();
#endif
	}

};


template<typename NativeCallback, typename ManagedCallback = void*>
class CallbackConverter : public TypeConverter<NativeCallback, ManagedCallback>
{
public:
	using BaseConverter = TypeConverter<NativeCallback, ManagedCallback>;
	using NativeType = typename BaseConverter::NativeType;
	using ManagedType = typename BaseConverter::ManagedType;

	NativeType toNativeType(const ManagedType& value) override
	{
		auto callback = reinterpret_cast<NativeType*>(value);
		return *callback;
	}
	ManagedType toManagedType(const NativeType& value) override
	{
		// NOT USED
		// return reinterpret_cast<BaseConverter::ManagedType>(&value);
		return nullptr;
	}

};

#endif