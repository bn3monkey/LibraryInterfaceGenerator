#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLENUM__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLENUM__

#include "../../../json/json.hpp"
#include "../../Result/Result.hpp"
#include "../../Auxiliary/Definition.hpp"
#include <string>
#include <unordered_map>
#include "SymbolObject.hpp"
#include "../../Auxiliary/Definition.hpp"
#include <sstream>
#include <iostream>
#include <utility>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SymbolEnum : public HasResult, public SymbolObject
        {
        public:
            const std::vector<std::string>& parentModules;
            std::vector<std::string> parentObjects;

            std::string name;
            std::string description;
            std::vector<std::pair<std::string, std::string>> values;

            explicit SymbolEnum(const nlohmann::json& object,
                const std::vector<std::string>& module_paths,
                std::vector<std::string>& object_paths
                );

            Tag getTag() const override { return Tag::Enum; }
            std::string getCppName() const override;
            std::string getKotlinName() const override;
        };

    }
}

#endif