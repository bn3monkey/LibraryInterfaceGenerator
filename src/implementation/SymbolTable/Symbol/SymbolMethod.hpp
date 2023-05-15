#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLMETHOD__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLMETHOD__

#include "../../../json/json.hpp"
#include "../../Result/Result.hpp"
#include "../../Auxiliary/Definition.hpp"
#include "SymbolType.hpp"
#include "SymbolParameter.hpp"
#include <vector>
#include <memory>
#include <string>
#include "../../Auxiliary/Definition.hpp"


namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SymbolMethod : public HasResult, public HasSymbolType
        {
        public:
            const std::vector<std::string>& parentModules;
            // std::unique_ptr<SymbolType> type;
            std::string name;
            std::string description;
            std::string return_description;
            std::vector<std::shared_ptr<SymbolParameter>> parameters;


            explicit SymbolMethod(const nlohmann::json& object,
                const std::vector<std::string>& module_paths,
                std::vector<std::weak_ptr<HasSymbolType>>& hasTypes,
                ObjectReferenceSet& parentObjectReferenceSet,
                EnumReferenceSet& parentEnumReferenceSet,
                CallbackReferenceSet& parentCallbackReferenceSet
                );
        };
    }
}

#endif