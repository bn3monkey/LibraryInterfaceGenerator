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


        using SymbolObjectTable = std::unordered_map<std::string, std::weak_ptr<SymbolObject>>;
        using SymbolEnumTable = std::unordered_map<std::string, std::weak_ptr<SymbolObject>>;


        class ReferenceComparaer
        {
        public:
            bool operator()(const std::weak_ptr<SymbolObject>& lhs, const std::weak_ptr<SymbolObject>& rhs) const
            {
                auto& lObject = lhs.lock();
                auto& rObject = rhs.lock();
                if (lObject != nullptr && rObject != nullptr)
                {
                    auto* lptr = lObject.get();
                    auto* rptr = rObject.get();
                    return lptr < rptr;
                }
                return true;
            }
        };

        using ReferenceSet = std::set<std::weak_ptr<SymbolObject>, ReferenceComparaer>;
        using EnumReferenceSet = ReferenceSet;
        using ObjectReferenceSet = ReferenceSet;


        class HasReference // Global Method & Interface & Class & Callback
        {
        public:
            inline const EnumReferenceSet& getEnumReferences() { return _enum_references;  }
            inline const ObjectReferenceSet& getObjectReferences() { return _object_references; }

            virtual std::vector<std::weak_ptr<SymbolObject>> collectAllClassReference() const = 0;
            virtual std::vector<std::weak_ptr<SymbolObject>> collectAllEnumReference() const = 0;

        protected:
            EnumReferenceSet _enum_references;
            ObjectReferenceSet _object_references;
        };

        
    }
}

#endif