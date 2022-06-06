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
#include "../../Auxiliary/Definition.hpp"


namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SymbolClass : public HasResult, public SymbolObject
        {
        public:
            const std::vector<std::string>& parentModules;
            std::vector<std::string> parentObjects;

            bool isInterface;
            std::string name;
            std::string description;
            
            std::vector<std::weak_ptr<SymbolClass>> bases;

            std::vector<std::shared_ptr<SymbolEnum>> enums;
            std::vector<std::shared_ptr<SymbolMethod>> methods;
            std::vector<std::shared_ptr<SymbolProperty>> properties;

            std::vector<std::shared_ptr<SymbolMethod>> getBaseMethod();
            std::vector<std::shared_ptr<SymbolProperty>> getBaseProperties();

            explicit SymbolClass(const nlohmann::json& object,
                bool isInterface,
                const std::vector<std::string>& module_paths,
                std::vector<std::string>& object_paths,
                SymbolObjectTable& objectTable,
                SymbolEnumTable& enumTable,
                std::vector<std::weak_ptr<HasSymbolType>>& hasTypes);

            Tag getTag() override { return Tag::Class; }
            std::string getCppName() override;
            std::string getKotlinName() override;

            Result change(SymbolObjectTable& objectTable);
        private:
            std::vector<std::string> _bases;

            void addEnumTable(SymbolEnumTable& enumTable, std::shared_ptr<SymbolEnum>& value);
        };
    }
}

#endif