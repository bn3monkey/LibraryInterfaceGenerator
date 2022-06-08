#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLMODULE__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLMODULE__

#include "../../../json/json.hpp"
#include "../../Result/Result.hpp"
#include "../../Auxiliary/Definition.hpp"
#include "SymbolClass.hpp"
#include "SymbolEnum.hpp"
#include "SymbolMethod.hpp"
#include <vector>
#include <string>
#include "../../Auxiliary/Definition.hpp"


namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SymbolModule : public HasResult
        {
        public:
            std::vector<std::string> moduleNames; // include itself
            std::string name;
            std::string description;

            std::vector<std::shared_ptr<SymbolClass>> classes;
            std::vector<std::shared_ptr<SymbolClass>> interfaces;
            std::vector<std::shared_ptr<SymbolMethod>> globla_methods;
            std::vector<std::shared_ptr<SymbolModule>> submodules;
            std::vector<std::shared_ptr<SymbolEnum>> enums;

            explicit SymbolModule(const nlohmann::json& object,
                std::vector<std::string>& parentModules,
                SymbolObjectTable& objectTable,
                SymbolEnumTable& enumTable,
                std::vector<std::weak_ptr<HasSymbolType>>& hasTypes);

        private:
            void addEnumTable(SymbolEnumTable& enumTable, std::shared_ptr<SymbolEnum>& value);
            void addObjectTable(SymbolObjectTable& objectTable, std::shared_ptr<SymbolClass>& value);
        };
    }
}

#endif