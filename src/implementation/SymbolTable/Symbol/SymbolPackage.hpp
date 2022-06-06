#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLPACKAGE__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLPACKAGE__

#include "../../../json/json.hpp"
#include "../../Result/Result.hpp"
#include "../../Auxiliary/Definition.hpp"
#include "SymbolModule.hpp"
#include <vector>
#include <string>


namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SymbolPackage : public HasResult
        {
        public:
            const std::string name;
            const std::string author;
            const std::string description;
            const std::vector<SymbolModule> modules;

            explicit SymbolPackage(const nlohmann::json& object,
                SymbolObjectTable& objectTable,
                SymbolEnumTable& enumTable);
        };
    }
}

#endif