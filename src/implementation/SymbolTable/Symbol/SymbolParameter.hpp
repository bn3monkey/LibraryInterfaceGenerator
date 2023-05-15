#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLPARAMETER__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLPARAMETER__

#include "../../../json/json.hpp"
#include "../../Result/Result.hpp"
#include "../../Auxiliary/Definition.hpp"
#include "SymbolType.hpp"
#include <memory>
#include <string>
#include "../../Auxiliary/Definition.hpp"


namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SymbolParameter : public HasResult, public HasSymbolType
        {
        public:
            enum class IO
            {
                IN,
                OUT,
            };
            // std::unique_ptr<SymbolType> type;
            std::string name;
            std::string description;
            IO io;

            explicit SymbolParameter(const nlohmann::json& object,
                ObjectReferenceSet& parentobjectReferenceSet,
                EnumReferenceSet& parentEnumReferenceSet,
                CallbackReferenceSet& paraentCallbackReferenceSet);
        };
    }
}

#endif