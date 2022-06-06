#include "SymbolPackage.hpp"

using namespace LibraryInterfaceGenerator::Implementation::Definition;

LibraryInterfaceGenerator::Implementation::SymbolPackage::SymbolPackage(
	const nlohmann::json& object,
	SymbolObjectTable& objectTable, 
	SymbolEnumTable& enumTable, 
	std::vector<std::weak_ptr<HasSymbolType>>& hasTypes)
{
	_result = Result(Result::Code::SUCCESS);

	{
		auto iter = object.find(Field::Name);
		if (iter == object.end())
		{
			_result = Result(Result::Code::JSON_OBJECT_HAS_NO_NAME);
			return;
		}
		name = iter->get<std::string>();
	}
	{
		auto iter = object.find(Field::Author);
		if (iter == object.end())
		{
			_result = Result(Result::Code::JSON_OBJECT_HAS_NO_AUTHOR);
			return;
		}

		author = iter->get<std::string>();
	}
	{
		auto iter = object.find(Field::Description);
		if (iter == object.end())
		{
			_result = Result(Result::Code::JSON_OBJECT_HAS_NO_DESCRIPTION);
			return;
		}

		description = iter->get<std::string>();
	}

	{
		auto iter = object.find(Field::Childs);
		if (iter == object.end())
		{
			_result = Result(Result::Code::JSON_OBJECT_HAS_NO_CHILDS);
			return;
		}

		auto& childs = *iter;
		for (const auto& child : childs)
		{
			auto iter = child.find(Field::Order);
			if (iter == child.end())
			{
				_result = Result(Result::Code::JSON_OBJECT_HAS_NO_ORDER);
				return;
			}

			auto order = iter->get<std::string>();

			if (order == Order::Module)
			{
				std::vector<std::string> paths = { name };

				auto tempModule = std::make_shared<SymbolModule>( child, paths, objectTable, enumTable, hasTypes );
				_result = tempModule->toResult();
				if (!_result)
					return;

				modules.push_back(tempModule);
			}
			else
			{
				_result = Result(Result::Code::JSON_OBJECT_HAS_INVALID_VALUE);
				return;
			}
		}
	}
}
