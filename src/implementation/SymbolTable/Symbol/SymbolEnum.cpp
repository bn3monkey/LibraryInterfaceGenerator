#include "SymbolEnum.hpp"

using namespace LibraryInterfaceGenerator::Implementation::Definition;

LibraryInterfaceGenerator::Implementation::SymbolEnum::SymbolEnum(
	const nlohmann::json& object, 
	const std::vector<std::string>& module_paths,
	std::vector<std::string>& object_paths) : parentModules(module_paths), parentObjects(object_paths)
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
		auto iter = object.find(Field::Description);
		if (iter == object.end())
		{
			_result = Result(Result::Code::JSON_OBJECT_HAS_NO_DESCRIPTION);
			return;
		}

		description = iter->get<std::string>();
	}
	{
		auto iter = object.find(Field::Values);
		if (iter == object.end())
		{
			_result = Result(Result::Code::JSON_OBJECT_HAS_NO_VALUES);
			return;
		}

		auto pairs = *iter;
		for (auto& pair_iter : pairs)
		{
			std::string pair = pair_iter.get<std::string>();
			std::istringstream ss{ pair };

			std::string key;
			std::getline(ss, key, '/');

			std::string value;
			std::getline(ss, value, '/');

			values.push_back({ key, value });
		}
	}

}

std::string LibraryInterfaceGenerator::Implementation::SymbolEnum::getCppName() const
{
	std::string value{ "" };
	for (auto& moduleName : parentModules)
	{
		value += moduleName;
		value += "::";
	}
	for (auto& objectName : parentObjects)
	{
		value += objectName;
		value += "::";
	}
	value += name;
	return value;
}

std::string LibraryInterfaceGenerator::Implementation::SymbolEnum::getKotlinName() const
{
	/*
	std::string value{ "" };
	for (auto& moduleName : parentModules)
	{
		value += moduleName;
		value += ".";
	}
	for (auto& objectName : parentObjects)
	{
		value += objectName;
		value += ".";
	}
	value += name;
	*/
	return name;
}
