#include "ManagedTypeConverter.hpp"
#ifdef __BN3MONKEY_MEMORY_POOL__
using namespace Bn3Monkey;
std::vector<std::function<void()>> releasers;

static bool is_initialized;
bool Bn3Monkey::ManagedTypeConverter::initialize()
{
#ifdef __BN3MONKEY_MEMORY_POOL__
	Bn3Monkey::Bn3MemoryPool::initialize({ 128, 128, 128, 128, 4, 4, 4, 4, 4 });
	// releasers = Bn3Monkey::Bn3MemoryPool::construct<Bn3Monkey::Bn3Container::vector<std::function<void()>> {}
	releasers.reserve(128);
#else

#endif
	is_initialized = true;
	return is_initialized;
}
void Bn3Monkey::ManagedTypeConverter::release()
{
#ifdef __BN3MONKEY_MEMORY_POOL__
	Bn3Monkey::Bn3MemoryPool::release();
	for (auto& releaser : releasers)
		releaser();
	releasers.clear();
#else

#endif
	is_initialized = false;
}
void Bn3Monkey::ManagedTypeConverter::addReleaser(std::function<void()> releaser)
{
#ifdef __BN3MONKEY_MEMORY_POOL__
	releasers.push_back(releaser);
#else

#endif

}
bool Bn3Monkey::ManagedTypeConverter::isInitialized()
{
	return is_initialized;
}


#endif