#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLPROPERTY__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLPROPERTY__

#include "../../../json/json.hpp"
#include "../../Result/Result.hpp"
#include "SymbolObject.hpp"
#include "SymbolType.hpp"
#include "SymbolObject.hpp"
#include "../../Auxiliary/Definition.hpp"
#include <memory>
#include <string>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SymbolProperty : public HasResult, public HasSymbolType
        {
        public:
            
            // std::unique_ptr<SymbolType> type;
            std::string name;
            bool readonly;
            std::string description;

            explicit SymbolProperty(const nlohmann::json& object, 
                ObjectReferenceSet& parentobjectReferenceSet,
                EnumReferenceSet& parentEnumReferenceSet,
                CallbackReferenceSet& parentCallbackReferenceSet
                );

        };
    }
}

#endif