#include <memory>
#include <vector>

template<class T, class ...Args>
inline void* createReference(Args... args)
{
#ifdef __LIBRARYINTERFACEGENERATOR_MEMORY_POOL__
    auto* ret = MemoryPool::allocate<std::shared_ptr<T>>();
    ret = std::shared_ptr<T>(MemoryPool::allocate<T>(args), [](T* p) {
        MemoryPool::deallocate<T>(p);
    });
#else
    auto* ret = new std::shared_ptr<T>(new T(args...));
#endif
    return ret;
}
template<class T>
inline void* createReference()
{
#ifdef __LIBRARYINTERFACEGENERATOR_MEMORY_POOL__
    auto* ret = MemoryPool::allocate<std::shared_ptr<T>>();
    ret = std::shared_ptr<T>(MemoryPool::allocate<T>(), [](T* p) {
        MemoryPool::deallocate<T>(p);
    });
#else
    auto* ret = new std::shared_ptr<T>();
#endif
    return ret;
}
template<class T>
inline void releaseReference(void* cptr)
{
#ifdef __LIBRARYINTERFACEGENERATOR_MEMORY_POOL__
    MemoryPool::deallocate(reinterpret_cast<std::shared_ptr<T>*>(cptr));
#else
    delete reinterpret_cast<std::shared_ptr<T>*>(cptr);
#endif
}
template<class T>
inline std::shared_ptr<T>& getReference(void* cptr)
{
    auto* ret = reinterpret_cast<std::shared_ptr<T>*>(cptr);
    return *ret;
}
template<class T>
inline std::shared_ptr<T>* cloneReference(std::shared_ptr<T>& cptr)
{
#ifdef __LIBRARYINTERFACEGENERATOR_MEMORY_POOL__
    auto* ret = MemoryPool::allocate<std::shared_ptr<T>>();
    *ret = value;
#else
    auto* ret = new shared_ptr<T>(cptr);
#endif
    return ret;
}


template<class ENUM>
inline int32_t createInterfaceEnum(ENUM value)
{
	return static_cast<int32_t>(value);
}
template<class CLASS>
inline void* createInterfaceObject(std::shared_ptr<CLASS>& value)
{
	return (void*)cloneReference<T>(value);
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
inline std::vector<void*> createInterfaceObjectArray(const std::vector<CLASS>& value)
{
	std::vector<void*> ret;
	for (auto& element : value)
	{
		auto* ielement = createInterfaceObject(element);
		ret.push_back(ielement);
	}
	return ret;
}
template<class CLASS>
inline std::vector<void*> createInterfaceObjectVector(const std::vector<CLASS>& value)
{
	std::vector<void*> ret;
	for (auto& element : value)
	{
		auto* ielement = createInterfaceObject(element);
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
}
template<class CLASS>
std::vector<std::shared_ptr<CLASS>> createNativeObjectVector(const std::vector<void*>& value)
{
	std::vector<CLASS> ret;
	for (auto& element : value)
	{
		auto* ielement = createNativeObject<CLASS>(element);
		ret.push_back(ielement);
	}
}

template<class ENUM>
void copyEnum(const ENUM src, int32_t& dest)
{
	value = static_cast<int32_t>(src);
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
inline std::vector<void*> copyObjectArray(const std::vector<CLASS>& src, std::vector<void*>& dest)
{
	dest.clear();
	for (auto& element : value)
	{
		auto* ielement = createInterfaceObject(element);
		ret.push_back(ielement);
	}
	return ret;
}
template<class CLASS>
inline std::vector<void*> copyObjectVector(const std::vector<CLASS>& src, std::vector<void*>& dest)
{
	dest.clear();
	for (auto& element : value)
	{
		auto* ielement = createInterfaceObject(element);
		ret.push_back(ielement);
	}
	return ret;
}