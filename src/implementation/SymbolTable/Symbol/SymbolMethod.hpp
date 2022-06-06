#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLMETHOD__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLMETHOD__

#include "../../../json/json.hpp"
#include "../../Result/Result.hpp"
#include "../../Auxiliary/Definition.hpp"
#include "SymbolType.hpp"
#include "SymbolParameter.hpp"
#include <vector>
#include <memory>
#include <string>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SymbolMethod : public HasResult
        {
        public:
            const std::vector<std::string> module_paths;
            std::unique_ptr<SymbolType> type;
            std::string name;
            std::string description;
            std::string return_description;
            std::vector<SymbolParameter> parameters;

            explicit SymbolMethod(const nlohmann::json& object,
                std::vector<std::string>& module_paths,
                SymbolObjectTable& objectTable,
                SymbolEnumTable& enumTable);
        };
    }
}

#endif