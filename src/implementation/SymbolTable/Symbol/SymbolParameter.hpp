#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLPARAMETER__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLPARAMETER__

#include "../../../json/json.hpp"
#include "../../Result/Result.hpp"
#include "../../Auxiliary/Definition.hpp"
#include "SymbolType.hpp"
#include <memory>
#include <string>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SymbolParameter : public HasResult
        {
        public:
            enum IO
            {
                IN,
                OUT,
            };
            const std::unique_ptr<SymbolType> type;
            const std::string name;
            const std::string description;
            const IO io;

            explicit SymbolParameter(const nlohmann::json& object,
                SymbolObjectTable& objectTable,
                SymbolEnumTable& enumTable);
        };
    }
}

#endif