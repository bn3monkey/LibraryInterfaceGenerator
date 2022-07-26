#include "MemoryPool.hpp"

static int8_t* pool {nullptr};
static int32_t* dirty{nullptr};

static int8_t* rear_freed_memory_ptr{nullptr};

static std::mutex mtx;
size_t block_num;
size_t dirty_num;

static std::vector<std::function<void()>> releasers;

#ifdef _DEBUG
int8_t* MemoryPool::getPool()
{
    return pool;
}
int32_t* MemoryPool::getDirty()
{
    return dirty;
}

int8_t* MemoryPool::getRearFreedMemoryPtr()
{
    return rear_freed_memory_ptr;
}
#endif
bool MemoryPool::isInitialized()
{
    return pool && dirty;
}
bool MemoryPool::initialize(size_t _block_num)
{
    std::lock_guard<std::mutex> lock(mtx);

    if (MemoryPool::isInitialized())
        return true;

    block_num = _block_num;
    pool = new int8_t[block_num];
    if (!pool)
        return false;
    memset(pool, 0, block_num * sizeof(int8_t));

    dirty_num = block_num / 32 * 4;
    dirty = new int32_t[dirty_num];
    if (!dirty)
    {
        delete[] pool;
        pool = nullptr;
        return false;
    }
    memset(dirty, 0, dirty_num * sizeof(int32_t));

    rear_freed_memory_ptr = pool;

    return true;
}
void MemoryPool::addReleaser(const std::function<void()>& releaser)
{
    releasers.push_back(releaser);
}
void MemoryPool::release()
{
    std::lock_guard<std::mutex> lock(mtx);
    if (MemoryPool::isInitialized())
    {
        for(auto& releaser : releasers)
        {
            releaser();
        }
        releasers.clear();

        delete[] pool;
        pool = nullptr;
        delete[] dirty;
        dirty = nullptr;

        rear_freed_memory_ptr = nullptr;
    }
}

static inline size_t getRelativeAddress(void* ptr, void* source)
{
    return static_cast<size_t>(reinterpret_cast<const char*>(ptr) - reinterpret_cast<const char*>(source));
}

static void* allocStartedFromMostRearFreedPtr(size_t allocation_size)
{
    size_t begin_address = rear_freed_memory_ptr - pool;

    if (begin_address > block_num)
        return nullptr;
    
    size_t end_address = begin_address + allocation_size;

    for (size_t relative_address = begin_address; relative_address < end_address; relative_address++)
    {
        size_t dirty_relative_address = (relative_address >> 5); // begin_address /32
        size_t dirty_bit = relative_address - (dirty_relative_address << 5);  // begin_address %32

        bool is_dirty = *(dirty + dirty_relative_address) & (1 << dirty_bit);
        if (is_dirty)
            return nullptr;
    }

    for (size_t relative_address = begin_address; relative_address < end_address; relative_address++)
    {
        size_t dirty_relative_address = (relative_address >> 5); // begin_address /32
        size_t dirty_bit = relative_address - (dirty_relative_address << 5);  // begin_address %32

        *(dirty + dirty_relative_address) |= (1 << dirty_bit);
    }

    void* ret = reinterpret_cast<void *>(rear_freed_memory_ptr);
    rear_freed_memory_ptr = rear_freed_memory_ptr + allocation_size;
    Log::D(__FUNCTION__, "Rear address is %p, Relative address is %d", rear_freed_memory_ptr, getRelativeAddress(rear_freed_memory_ptr, pool));
    return ret;    
}

static void* allocStartedFromMostFrontFreedPtr(size_t allocation_size)
{
    size_t remained_size = allocation_size;

    size_t begin_address = 0;
    size_t end_address = rear_freed_memory_ptr - pool;
    size_t relative_address = 0;

    size_t allocated_start_address = begin_address;
    
    for (relative_address = begin_address; 
        relative_address < end_address; 
        relative_address++)
    {
        size_t dirty_relative_address = (relative_address >> 5); // relative_address /32
        size_t dirty_bit = relative_address - (dirty_relative_address << 5);  // relative_address %32

        bool is_dirty = *(dirty + dirty_relative_address) & (1 << dirty_bit);
        if (is_dirty)
        {
            remained_size = allocation_size;
            allocated_start_address = relative_address;
        }
        else
        {
            remained_size--;
            if (!remained_size)
                break;
        }
    }
    
    if (relative_address == end_address)
        return nullptr;

    size_t allocated_end_address = relative_address;

    for (relative_address = allocated_start_address; 
        relative_address < allocated_end_address;
        relative_address++)
    {
        size_t dirty_relative_address = (relative_address >> 5); // relative_address /32
        size_t dirty_bit = relative_address - (dirty_relative_address << 5);  // relative_address %32

        *(dirty + dirty_relative_address) |= (1 << dirty_bit);
    }

    void* ret = pool + allocated_start_address;
    return ret;
}


void* MemoryPool::alloc(size_t object_size, size_t n)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (!MemoryPool::isInitialized())
        return nullptr;

    size_t allocation_size = object_size * n;


    void* ret = allocStartedFromMostRearFreedPtr(allocation_size);
    if (!ret)
    {
        ret = allocStartedFromMostFrontFreedPtr(allocation_size);
    }
    Log::D(__FUNCTION__, "Allocated address is %p, Relative address is %d", ret, getRelativeAddress(ret, pool));
    return ret;
}
void MemoryPool::dealloc(void* cptr, size_t object_size, size_t n)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (!MemoryPool::isInitialized())
        return;


    if ((int8_t *)cptr < pool && pool + block_num <= (int8_t *)cptr)
    {
        return;
    }

    size_t allocation_size = object_size * n;
    memset(cptr, 0, object_size * n);

    size_t begin_address = (int8_t *)cptr - pool;
    size_t end_address = begin_address + object_size * n;
    size_t relative_address = 0;
    for (relative_address = begin_address;
        relative_address < end_address;
        relative_address++)
        {
            size_t dirty_relative_address = (relative_address >> 5); // relative_address /32
            size_t dirty_bit = relative_address - (dirty_relative_address << 5);  // relative_address %32

            *(dirty + dirty_relative_address) &= ~(1 << dirty_bit);
        }


   
    {
        size_t rear_address = rear_freed_memory_ptr - pool - 1;
        size_t relative_address = rear_address;

        for (;
            relative_address > 0u;
            relative_address--)
        {
            size_t dirty_relative_address = (relative_address >> 5); // relative_address /32
            size_t dirty_bit = relative_address - (dirty_relative_address << 5);  // relative_address %32

            bool is_dirty = *(dirty + dirty_relative_address) & (1 << dirty_bit);
            if (is_dirty)
                break;
        }

        if (relative_address == 0)
        {
            bool is_dirty = *(dirty) & 1;
            if (!is_dirty)
            {
                rear_freed_memory_ptr = pool;
                Log::D(__FUNCTION__, "Rear address is %p, Relative address is %d", cptr, getRelativeAddress(rear_freed_memory_ptr, pool));
            }
        }
        else
        {
            rear_freed_memory_ptr = pool + relative_address + 1;
            Log::D(__FUNCTION__, "Rear address is %p, Relative address is %d", cptr, getRelativeAddress(rear_freed_memory_ptr, pool));
        }
    }

    Log::D(__FUNCTION__, "Released address is %p, Relative address is %d", cptr, getRelativeAddress(cptr, pool));
    return;
}