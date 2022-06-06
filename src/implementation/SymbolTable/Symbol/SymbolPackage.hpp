#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLPACKAGE__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLPACKAGE__

#include "../../../json/json.hpp"
#include "../../Result/Result.hpp"
#include "../../Auxiliary/Definition.hpp"
#include "SymbolModule.hpp"
#include <vector>
#include <string>
#include "../../Auxiliary/Definition.hpp"


namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SymbolPackage : public HasResult
        {
        public:
            std::string name;
            std::string author;
            std::string description;
            std::vector<std::shared_ptr<SymbolModule>> modules;

            explicit SymbolPackage(const nlohmann::json& object,
                SymbolObjectTable& objectTable,
                SymbolEnumTable& enumTable,
                std::vector<std::weak_ptr<HasSymbolType>>& hasTypes);
        };
    }
}

#endif