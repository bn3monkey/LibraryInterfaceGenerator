#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLMODULE__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLMODULE__

#include "../../../json/json.hpp"
#include "../../Result/Result.hpp"
#include "../../Auxiliary/Definition.hpp"
#include "SymbolClass.hpp"
#include "SymbolInterface.hpp"
#include "SymbolEnum.hpp"
#include "SymbolMethod.hpp"
#include <vector>
#include <string>


namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SymbolModule : public HasResult
        {
        public:
            const std::vector<std::string> module_paths;
            const std::string name;
            const std::string description;

            const std::vector<std::shared_ptr<SymbolClass>> classes; 
            const std::vector<std::shared_ptr<SymbolInterface>> interfaces;
            const std::vector<std::shared_ptr<SymbolMethod>> globla_methods;
            const std::vector<std::shared_ptr<SymbolModule>> submodules;
            const std::vector<std::shared_ptr<SymbolEnum>> enums;

            explicit SymbolModule(const nlohmann::json& object,
                std::vector<std::string> module_paths,
                SymbolObjectTable& objectTable,
                SymbolEnumTable& enumTable);
        };
    }
}

#endif