#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLOBJECT__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLOBJECT__

#include "../../../json/json.hpp"
#include "../../Result/Result.hpp"
#include <string>
#include <memory>
#include <unordered_map>
#include <set>
#include "../../Auxiliary/Definition.hpp"


namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SymbolObject
        {
        public:
            enum class Tag
            {
                Interface,
                Class,
                Enum
            };

            virtual Tag getTag() const = 0;
            virtual std::string getCppName() const = 0;
            virtual std::string getKotlinName() const = 0;
        };

        class HasReference // Global Method & Interface & Class
        {
        public:
            std::vector<std::weak_ptr<SymbolObject>> enum_references;
            std::vector<std::weak_ptr<SymbolObject>> object_references;
            void collectReference();
        private:
            std::set<std::string> _reference_names;
        };

        using SymbolObjectTable = std::unordered_map<std::string, std::weak_ptr<SymbolObject>>;
        using SymbolEnumTable = std::unordered_map<std::string, std::weak_ptr<SymbolObject>>;
        
        using HasReference
    }
}

#endif