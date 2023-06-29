#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_KOTLINEXTERNALLIBRARYDIRECTORY__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_KOTLINEXTERNALLIBRARYDIRECTORY__

#include <string>
#include "../Result/Result.hpp"


namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class KotlinExternalLibraryDirectory
        {
        public:
            enum class ExternalTool : size_t
            {
                KotlinTypeConverter,
                LENGTH
            };

            std::string createExternalToolCode(ExternalTool tool) const;
        };
    }
}

#endif