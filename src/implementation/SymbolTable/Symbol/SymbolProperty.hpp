#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLPROPERTY__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLPROPERTY__

#include "../../../json/json.hpp"
#include "../../Result/Result.hpp"
#include "SymbolObject.hpp"
#include "SymbolType.hpp"
#include "../../Auxiliary/Definition.hpp"

#include <memory>
#include <string>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SymbolProperty : public HasResult
        {
        public:
            
            const std::unique_ptr<SymbolType> type;
            const std::string name;
            const bool readonly;
            const std::string description;

            explicit SymbolProperty(const nlohmann::json& object,
                SymbolObjectTable& objectTable,
                SymbolEnumTable& enumTable);
        };
    }
}

#endif