#include <memory>
#include <vector>
#include <functional>
#include <string>
#include <algorithm>
#include <iterator>

#ifdef __BN3MONKEY_MEMORY_POOL__
static Bn3Monkey::Bn3Container::vector<std::function<void()>> releasers { Bn3Monkey::Bn3Allocator<std::function<void()>>(Bn3Monkey::Bn3Tag("releasers")) };
static int shared_ptr_num = 1;
static int raw_ptr_num = 1;
#endif

void initializeNativeInterface()
{
#ifdef __BN3MONKEY_MEMORY_POOL__
	Bn3Monkey::Bn3MemoryPool::initialize({128, 128, 128, 128, 4, 4, 4, 4, 4});
	releasers.reserve(128);
#else

#endif
}

void releaseNativeInterface()
{
#ifdef __BN3MONKEY_MEMORY_POOL__
	Bn3Monkey::Bn3MemoryPool::release();
	for (auto& releaser : releasers)
		releaser();
	releasers.reset();
#else

#endif
}

template<class T>
class ReferenceConverter
{
public:

	using Instance = std::shared_ptr<T>;
	using LibraryReference = Instance*; 
	using Reference = void*;

	using ReferenceCast = reinterpret_cast<Reference>;
	using LibraryReferenceCast = reinterpret_cast<LibraryReference>;

	template<class ...Args>
	static Reference create(Args... args)
	{
		LibraryReference ret;
#ifdef __BN3MONKEY_MEMORY_POOL__
		char ptr_tag[256]{0};
		sprintf(ptr_tag, "ptr_%d", shared_ptr_num++);
		char raw_tag[256]{0};
		sprintf(raw_tag, "raw_%d", raw_ptr_num++);

    	ret = Bn3Monkey::Bn3MemoryPool::construct<Instance>(Bn3Monkey::Bn3Tag(ptr_tag));
    	*ret = Instance(Bn3Monkey::Bn3MemoryPool::construct<T>(Bn3Monkey::Bn3Tag(raw_tag), args...), [](T* p) {
        	Bn3Monkey::Bn3MemoryPool::destroy<T>(p);
    	});
#else
    	ret = new Instance(new T(args...));
#endif
    	return ReferenceCast(ret);
	}	

	static void release(Reference ref)
	{
#ifdef __BN3MONKEY_MEMORY_POOL__
    	Bn3Monkey::Bn3MemoryPool::destroy(LibraryReferenceCast(ref));
#else
    	delete LibraryReferenceCast(ref);
#endif
	}

	static Instance& get(Reference ref)
	{
		LibraryReference ret = LibraryReferenceCast(ref);
		return *ret;
	}

	static Reference clone(Reference ref)
	{
		LibraryReference src = LibraryReferenceCast(ref);
		LibraryReference dest;
		
#ifdef __LIBRARYINTERFACEGENERATOR_MEMORY_POOL__
		char ptr_tag[256]{0};
		sprintf(ptr_tag, "ptr_%d", shared_ptr_num++);
    	dest = Bn3Monkey::Bn3MemoryPool::construct<Instance>(Bn3Monkey::Bn3Tag(ptr_tag));
    	*dest = src;
#else
    	dest = new Instance(*src);
#endif
    	return ReferenceCast(dest);	
	}

	static void addReleaser(Reference releaser)
	{
#ifdef __LIBRARYINTERFACEGENERATOR_MEMORY_POOL__
		auto* param = reinterpret_cast<std::function<void()>*)(releaser);
		releasers.push_back(*param);
#endif
	}	
};

template<typename ENUM>
class EnumConverter
{
public:
	using NativeType = ENUM;
	using InterfaceType = int32_t;

	using NativeTypeArray = std::vector<ENUM>;
	using InterfaceTypeArray = std::vector<int32_t>;
	
	using NativeTypeVector = std::vector<ENUM>;
	using InterfaceTypeVector = std::vector<int32_t>;

	static NativeType toNativeType(InterfaceType value)
	{
		return static_cast<NativeType>(value);
	}
	static InterfaceType toInterfaceType(NativeType value)
	{
		return static_cast<InterfaceType>(value);
	}

	static NativeTypeArray toNativeTypeArray(InterfaceTypeArray values)
	{
		NativeTypeArray ret;
		std::transform(value.begin(), value.end(), std::back_inserter(ret), EnumConverter<ENUM>::toNativeType);
		return ret;
	}

	static InterfaceTypeArray toInterfaceTypeArray(NativeTypeArray values)
	{
		InterfaceTypeArray ret;
		std::transform(value.begin(), value.end(), std::back_inserter(ret), EnumConverter<ENUM>::toInterfaceType);
		return ret;	
	}

	static NativeTypeVector toNativeTypeVector(InterfaceTypeVector values)
	{
		NativeTypeVector ret;
		std::transform(value.begin(), value.end(), std::back_inserter(ret), EnumConverter<ENUM>::toNativeType);
		return ret;
	}

	static InterfaceTypeVector toInterfaceTypeArray(NativeTypeVector values)
	{
		InterfaceTypeVector ret;
		std::transform(value.begin(), value.end(), std::back_inserter(ret), EnumConverter<ENUM>::toInterfaceType);
		return ret;	
	}
};

template<typename CLASS>
class ObjectConverter
{

};

template<typename CALLBACK>
class CallbackConverter
{

};




template<class ENUM>
inline int32_t createInterfaceEnum(ENUM value)
{
	return static_cast<int32_t>(value);
}
template<class CLASS>
inline void* createInterfaceObject(const std::shared_ptr<CLASS>& value)
{
	return (void*)cloneReference<CLASS>(value);
}
template<class ENUM>
inline std::vector<int32_t> createInterfaceEnumArray(const std::vector<ENUM>& value)
{
	std::vector<int32_t> ret;
	for (auto& element : value)
	{
		auto ielement = createInterfaceEnum(element);
		ret.push_back(ielement);
	}
	return ret;
}
template<class ENUM>
inline std::vector<int32_t> createInterfaceEnumVector(const std::vector<ENUM>& value)
{
	std::vector<int32_t> ret;
	for (auto& element : value)
	{
		auto ielement = createInterfaceEnum(element);
		ret.push_back(ielement);
	}
	return ret;
}
template<class CLASS>
inline std::vector<void*> createInterfaceObjectArray(const std::vector<std::shared_ptr<CLASS>>& value)
{
	std::vector<void*> ret;
	for (auto& element : value)
	{
		auto ielement = createInterfaceObject(element);
		ret.push_back(ielement);
	}
	return ret;
}
template<class CLASS>
inline std::vector<void*> createInterfaceObjectVector(const std::vector<std::shared_ptr<CLASS>>& value)
{
	std::vector<void*> ret;
	for (auto& element : value)
	{
		auto ielement = createInterfaceObject(element);
		ret.push_back(ielement);
	}
	return ret;
}

template<class ENUM>
inline ENUM createNativeEnum(int32_t value)
{
	return static_cast<ENUM>(value);
}

template<class CLASS>
inline std::shared_ptr<CLASS> createNativeObject(const void*& value)
{
	return getReference<CLASS>((void *)value);
}

template<class ENUM>
inline std::vector<ENUM> createNativeEnumArray(const std::vector<int32_t>& value)
{
	std::vector<ENUM> ret;
	for (auto& element : value)
	{
		auto ielement = convertNativeEnum<ENUM>(element);
		ret.push_back(ielement);
	}
	return ret;
}
template<class ENUM>
std::vector<ENUM> createNativeEnumVector(const std::vector<int32_t>& value)
{
	std::vector<ENUM> ret;
	for (auto& element : value)
	{
		auto ielement = convertNativeEnum<ENUM>(element);
		ret.push_back(ielement);
	}
	return ret;
}
template<class CLASS>
std::vector<CLASS> createNativeObjectArray(const std::vector<void *>& value)
{
	std::vector<CLASS> ret;
	for (auto& element : value)
	{
		auto* ielement = createNativeObject<CLASS>(element);
		ret.push_back(ielement);
	}
	return ret;
}
template<class CLASS>
std::vector<std::shared_ptr<CLASS>> createNativeObjectVector(const std::vector<void*>& value)
{
	std::vector<std::shared_ptr<CLASS>> ret;
	for (auto& element : value)
	{
		auto ielement = createNativeObject<CLASS>((const void*&)element);
		ret.push_back(ielement);
	}
	return ret;
}

template<class ENUM>
void copyEnum(const ENUM src, int32_t& dest)
{
	dest = static_cast<int32_t>(src);
}
template<class CLASS>
void copyObject(const std::shared_ptr<CLASS>& src, void*& dest)
{
	dest = (void*)cloneReference<CLASS>(src);
}

template<class ENUM>
void copyEnumArray(const std::vector<ENUM>& src, std::vector<int32_t>& dest)
{
	dest.clear();
	for (auto& element : src)
	{
		auto ielement = createInterfaceEnum(element);
		dest.push_back(ielement);
	}
}

template<class ENUM>
void copyEnumVector(const std::vector<ENUM>& src, std::vector<int32_t>& dest)
{
	dest.clear();
	for (auto& element : src)
	{
		auto ielement = createInterfaceEnum(element);
		dest.push_back(ielement);
	}
}

template<class CLASS>
inline void copyObjectArray(const std::vector<CLASS>& src, std::vector<void*>& dest)
{
	dest.clear();
	for (auto& element : src)
	{
		auto* ielement = createInterfaceObject(element);
		dest.push_back(ielement);
	}
}
template<class CLASS>
inline std::vector<void*> copyObjectVector(const std::vector<CLASS>& src, std::vector<void*>& dest)
{
	dest.clear();
	for (auto& element : src)
	{
		auto* ielement = createInterfaceObject(element);
		dest.push_back(ielement);
	}
}