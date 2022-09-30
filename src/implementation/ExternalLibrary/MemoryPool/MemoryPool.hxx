#ifndef __LIBRARYINTERFACEGENERATOR_MEMORY_POOL__
#define __LIBRARYINTERFACEGENERATOR_MEMORY_POOL__

#include <cstdint>
#include <mutex>
#include "../Log/Log.hpp"
#include <memory>
#include <functional>
#include <vector>

class MemoryPool
{
public:
    static bool initialize(size_t _block_num);
    static void release();
    static bool isInitialized();
    static void* alloc(size_t object_size, size_t n);
    static void dealloc(void* cptr, size_t object_size, size_t n);
    static void addReleaser(const std::function<void()>& releaser);


    template<class T, class... Args>
    static T* allocate(size_t n, Args... args)
    {
       using value_type = T;
		if (!isInitialized())
			return nullptr;
       T* ret = reinterpret_cast<T*>(MemoryPool::alloc(sizeof(T), n));
       new(ret) T(args...);
       return ret;
    }
    template<class T>
    static T* allocate(size_t n = 1)
    {
        using value_type = T;
		if (!isInitialized())
			return nullptr;
        T* ret = reinterpret_cast<T*>(MemoryPool::alloc(sizeof(T), n));
        new(ret) T();
        return ret;
    }
    template<class T>
    static void deallocate(T* ptr, size_t n = 1)
    {
		if (!isInitialized())
			return;
        using value_type = T;
        ptr->~T();
        MemoryPool::dealloc(ptr, sizeof(T), n);
    }

#ifdef _DEBUG
    static int8_t* getPool();
    static int32_t* getDirty();
    static int8_t* getRearFreedMemoryPtr();
#endif
};

template<class T>
struct MemoryAllocator  /* : public std::allocator<T> */
{
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    MemoryAllocator() noexcept {};
    ~MemoryAllocator() noexcept {};
    template<class U> MemoryAllocator(const MemoryAllocator<U>& e) noexcept
    {
    }
    pointer allocate(size_type n, const void* hint = 0)
    {
        return MemoryPool::allocate<value_type>(n);
    }
    void deallocate(pointer ptr, size_type n)
    {
        MemoryPool::deallocate<value_type>(ptr, n);
    }
    template<typename U>
    struct rebind {
        using other =  MemoryAllocator<U>;
    };
};
template <class T, class U>
constexpr bool operator== (const MemoryAllocator<T>&, const MemoryAllocator<U>&) noexcept
{
    return true;
}
template <class T, class U>
constexpr bool operator!= (const MemoryAllocator<T>&, const MemoryAllocator<U>&) noexcept
{
    return false;
}


#endif