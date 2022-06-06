#include "SymbolParameter.hpp"


using namespace LibraryInterfaceGenerator::Implementation::Definition;

LibraryInterfaceGenerator::Implementation::SymbolParameter::SymbolParameter(const nlohmann::json& object, SymbolObjectTable& objectTable, SymbolEnumTable& enumTable)
{
	auto type_iter = object.find(Field::Type);
	if (type_iter == object.end())
	{
		_result =  Result(Result::Code::)
	}
}
