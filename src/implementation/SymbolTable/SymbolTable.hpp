#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLTABLE__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLTABLE__

#include "../json/json.hpp"
#include "../Result/Result.hpp"
#include "Symbol/SymbolPackage.hpp"
#include "Symbol/SymbolModule.hpp"
#include "Symbol/SymbolType.hpp"
#include "Symbol/SymbolInterface.hpp"
#include "Symbol/SymbolObject.hpp"
#include "Symbol/SymbolClass.hpp"
#include "Symbol/SymbolEnum.hpp"
#include "Symbol/SymbolParameter.hpp"
#include "Symbol/SymbolProperty.hpp"
#include "Symbol/SymbolMethod.hpp"

#include "../Auxiliary/Definition.hpp"

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SymbolTable : public HasResult
        {
        public:
            explicit SymbolTable(const nlohmann::json& object);
            const SymbolPackage& getPackage();
        private:
            std::shared_ptr<SymbolPackage> _package;
            SymbolObjectTable _objectTable;
            SymbolEnumTable _enumTable;
        }
    }
}

#endif