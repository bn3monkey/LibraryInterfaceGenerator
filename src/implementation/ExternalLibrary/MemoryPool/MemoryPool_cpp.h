#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_MEMORYPOOL_CPP__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_MEMORYPOOL_CPP__
static constexpr char* MEMORYPOOL_CPP = 
"#include \"MemoryPool.hpp\"\n"
"\n"
"using namespace Bn3Monkey;\n"
"Bn3MemoryBlockPools<BLOCK_SIZE_POOL_LENGTH> Bn3MemoryPool::_impl{};\n";
#endif //__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_MEMORYPOOL_CPP__
