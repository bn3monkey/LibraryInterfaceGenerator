#include "SymbolTable.hpp"

using namespace LibraryInterfaceGenerator::Implementation::Definition;
LibraryInterfaceGenerator::Implementation::SymbolTable::SymbolTable(const nlohmann::json& object)
{
	_result = Result(Result::Code::SUCCESS);
	
	auto order_iter = object.find(Field::Order);
	if (order_iter == object.end())
	{
		_result = Result(Result::Code::JSON_OBJECT_HAS_NO_ORDER);
		return;
	}

	auto order = order_iter->get<std::string>();

	if (order != Order::Package)
	{
		_result = Result(Result::Code::JSON_OBJECT_HAS_INVALID_VALUE);
		return;
	}

	_package = std::make_shared<LibraryInterfaceGenerator::Implementation::SymbolPackage>(
		object,
		_objectTable,
		_enumTable
		);

}

const LibraryInterfaceGenerator::Implementation::SymbolPackage& LibraryInterfaceGenerator::Implementation::SymbolTable::getPackage()
{
	return *(_package.get());
}
