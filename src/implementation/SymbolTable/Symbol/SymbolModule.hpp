#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLMODULE__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLMODULE__

#include "../../../json/json.hpp"
#include "../../Result/Result.hpp"
#include "../../Auxiliary/Definition.hpp"
#include "SymbolClass.hpp"
#include "SymbolEnum.hpp"
#include "SymbolMethod.hpp"
#include "SymbolCallback.hpp"
#include <vector>
#include <string>
#include "../../Auxiliary/Definition.hpp"


namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SymbolModule : public HasResult, public HasReference
        {
        public:
            std::vector<std::string> moduleNames; // include itself
            std::string name;
            std::string description;

            std::vector<std::shared_ptr<SymbolClass>> classes;
            std::vector<std::shared_ptr<SymbolClass>> interfaces;
            std::vector<std::pair<std::shared_ptr<SymbolMethod>, int>> global_methods;
            std::vector<std::shared_ptr<SymbolModule>> submodules;
            std::vector<std::shared_ptr<SymbolEnum>> enums;

            std::vector<std::shared_ptr<SymbolCallback>> callbacks;

            explicit SymbolModule(const nlohmann::json& object,
                std::vector<std::string>& parentModules,
                SymbolObjectTable& objectTable,
                SymbolEnumTable& enumTable,
                SymbolCallbackTable& callbackTable,
                std::vector<std::weak_ptr<HasSymbolType>>& hasTypes);

            std::vector<std::weak_ptr<SymbolObject>> collectAllClassReference() const override;
            std::vector<std::weak_ptr<SymbolObject>> collectAllEnumReference() const override;
            std::vector<std::weak_ptr<SymbolObject>> collectAllCallbackReference() const override;

        private:
            void addEnumTable(SymbolEnumTable& enumTable, std::shared_ptr<SymbolEnum>& value);
            void addObjectTable(SymbolObjectTable& objectTable, std::shared_ptr<SymbolClass>& value);
            void addCallbackTable(SymbolCallbackTable& callbackTable, std::shared_ptr<SymbolCallback>& value);
        };
    }
}

#endif