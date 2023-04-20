#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_KOTLINCONVERTER__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_KOTLINCONVERTER__

#include "../SymbolTable/SymbolTable.hpp"

namespace LibraryInterfaceGenerator
{
	namespace Implementation
	{
		inline std::string createKotlinWrapperScope(const SymbolClass& clazz)
		{
			std::string scope;
			auto& moduleNames = clazz.parentModules;
			auto& objectNames = clazz.parentObjects;
			for (auto& moduleName : moduleNames)
			{
				scope += moduleName;
				scope += "_";
			}
			for (auto& objectName : objectNames)
			{
				scope += objectName;
				scope += "_";
			}
			scope += clazz.name;
			scope += "_";
			return scope;
		}

		inline std::string createKotlinWrapperScope(const SymbolMethod& method)
		{
			std::string scope;
			auto& moduleNames = method.parentModules;
			for (auto& moduleName : moduleNames)
			{
				scope += moduleName;
				scope += "_";
			}
			return scope;
		}
	}
}
#endif