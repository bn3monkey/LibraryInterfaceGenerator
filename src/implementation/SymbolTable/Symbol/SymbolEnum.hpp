#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLENUM__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLENUM__

#include "../../../json/json.hpp"
#include "../../Result/Result.hpp"
#include "../../Auxiliary/Definition.hpp"
#include <string>
#include <unordered_map>
#include "SymbolObject.hpp"

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SymbolEnum : public HasResult, SymbolObject
        {
        public:
            const std::vector<std::string>& module_paths;
            std::string name;
            std::string description;
            std::unordered_map<std::string, int> values;

            explicit SymbolEnum(const nlohmann::json& object,
                const std::vector<std::string> module_paths,
                SymbolObjectTable& objectTable,
                SymbolEnumTable& enumTable);

            Tag getTag() override { return Tag::Enum; }
            std::string getCppName() override;
            std::string getKotlinName() override;
        };

    }
}

#endif