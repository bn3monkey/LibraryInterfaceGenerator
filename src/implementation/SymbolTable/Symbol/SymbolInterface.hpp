#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLINTERFACE__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLINTERFACE__

#include "../../../json/json.hpp"
#include "../../Result/Result.hpp"
#include "../../Auxiliary/Definition.hpp"
#include "SymbolType.hpp"
#include "SymbolProperty.hpp"
#include "SymbolEnum.hpp"
#include "SymbolMethod.hpp"
#include <memory>
#include <vector>
#include <string>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SymbolInterface : public HasResult, SymbolObject
        {
        public:
            const std::vector<std::string> module_paths;
            const std::string name;
            const std::string description;

            const std::vector<std::shared_ptr<SymbolEnum>> enums;
            const std::vector<SymbolMethod> methods;
            const std::vector<SymbolProperty> properties;

            explicit SymbolInterface(const nlohmann::json& object,
                const std::vector<std::string> module_paths,
                SymbolObjectTable& objectTable,
                SymbolEnumTable& enumTable);

            Tag getTag() override { return Tag::Interface; }
            std::string getCppName() override;
            std::string getKotlinName() override;
        };
    }
}

#endif