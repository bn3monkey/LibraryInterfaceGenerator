#include "NativeInterfaceConverter.hpp"
#ifdef __BN3MONKEY_MEMORY_POOL__
using namespace Bn3Monkey;
std::vector<std::function<void()>> releasers;

void initializeNativeInterface()
{
#ifdef __BN3MONKEY_MEMORY_POOL__
	Bn3Monkey::Bn3MemoryPool::initialize({ 128, 128, 128, 128, 4, 4, 4, 4, 4 });
	// releasers = Bn3Monkey::Bn3MemoryPool::construct<Bn3Monkey::Bn3Container::vector<std::function<void()>> {}
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
	releasers.clear();
#else

#endif
}

void addReleaser(void* releaser)
{
#ifdef __BN3MONKEY_MEMORY_POOL__
	auto* ret = reinterpret_cast<std::function<void()>*>(releaser);
	releasers.push_back(*ret);
#else

#endif
}

#endif