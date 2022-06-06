#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLCLASS__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLCLASS__

#include "../../../json/json.hpp"
#include "../../Result/Result.hpp"
#include "../../Auxiliary/Definition.hpp"
#include "SymbolType.hpp"
#include "SymbolProperty.hpp"
#include "SymbolEnum.hpp"
#include "SymbolMethod.hpp"
#include <memory>
#include <string>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SymbolClass : public HasResult, SymbolObject
        {
        public:
            const std::vector<std::string>& module_paths;
            const std::string name;
            const std::string description;
            const std::vector<std::string> base;

            const std::vector<SymbolMethod> methods;
            const std::vector<SymbolProperty> properties;

            const std::vector<SymbolMethod> getBaseMethod();
            const std::vector<SymbolProperty> getBaseProperties();

            explicit SymbolClass(const nlohmann::json& object,
                const std::vector<std::string>& module_paths,
                SymbolObjectTable& objectTable,
                SymbolEnumTable& enumTable);

            Tag getTag() override { return Tag::Class; }
            std::string getCppName() override;
            std::string getKotlinName() override;

        private:
            SymbolObjectTable& _objectTable;
        };
    }
}

#endif