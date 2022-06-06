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

	SymbolObjectTable objectTable;
	SymbolEnumTable enumTable;
	std::vector<std::weak_ptr<HasSymbolType>> hasTypes;

	_package = std::make_shared<LibraryInterfaceGenerator::Implementation::SymbolPackage>(
		object,
		objectTable,
		enumTable,
		hasTypes
		);

	_result = _package->toResult();
	if (!_result)
	{
		return;
	}

	for (auto& hasTypePtr : hasTypes)
	{
		if (auto hasType = hasTypePtr.lock())
		{
			_result = hasType->change(objectTable, enumTable);
			if (!_result)
			{
				return;
			}
		}
	}

	for (auto& objectptr : objectTable)
	{
		if (auto object = objectptr.second.lock())
		{
			auto clazz = std::dynamic_pointer_cast<SymbolClass>(object);
			if (clazz)
			{
				_result = clazz->change(objectTable);
				if (!_result)
				{
					return;
				}
			}
		}
	}
}

const LibraryInterfaceGenerator::Implementation::SymbolPackage& LibraryInterfaceGenerator::Implementation::SymbolTable::getPackage()
{
	return *(_package.get());
}

