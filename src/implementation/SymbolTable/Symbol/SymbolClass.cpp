#include "SymbolClass.hpp"

using namespace LibraryInterfaceGenerator::Implementation::Definition;

std::vector<std::shared_ptr<LibraryInterfaceGenerator::Implementation::SymbolMethod>> LibraryInterfaceGenerator::Implementation::SymbolClass::getBaseMethod()
{
	return std::vector<std::shared_ptr<SymbolMethod>>();
}

std::vector<std::shared_ptr<LibraryInterfaceGenerator::Implementation::SymbolProperty>> LibraryInterfaceGenerator::Implementation::SymbolClass::getBaseProperties()
{
	return std::vector<std::shared_ptr<SymbolProperty>>();
}

LibraryInterfaceGenerator::Implementation::SymbolClass::SymbolClass
(
	const nlohmann::json& object,
	bool isInterface,
	const std::vector<std::string>& module_paths, 
	std::vector<std::string>& object_paths, 
	SymbolObjectTable& objectTable, 
	SymbolEnumTable& enumTable, 
	std::vector<std::weak_ptr<HasSymbolType>>& hasTypes) : isInterface(isInterface), parentModules(module_paths), parentObjects(object_paths)
{
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
		auto iter = object.find(Field::Base);
		if (iter != object.end())
		{
			auto& base_list = *iter;
			for (auto& base_obj : base_list)
			{
				auto base_str = base_obj.get<std::string>();
				_bases.push_back(base_str);
			}
		}
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

			if (order == Order::Enum)
			{
				std::vector<std::string> objects{ parentObjects };
				objects.push_back(name);

				//SymbolEnum temp{ child, parentModules, objects };
				auto tempEnum = std::make_shared<SymbolEnum>(
					child,
					parentModules,
					objects
					);

				_result = tempEnum->toResult();
				if (!_result)
					return;

				enums.push_back(tempEnum);
				addEnumTable(enumTable, tempEnum);
			}
			else if (order == Order::Method)
			{
				/*
				SymbolMethod tempMethod{
					child,
					parentModules,
					hasTypes
				};
				*/
				auto tempMethod = std::make_shared<SymbolMethod>(
					child,
					parentModules,
					hasTypes
					);

				_result = tempMethod->toResult();
				if (!_result)
					return;

				methods.push_back(tempMethod);
				hasTypes.push_back(tempMethod);
			}
			else if (order == Order::Property)
			{
				/*
				SymbolProperty tempProperty{
					child,
					hasTypes
				};
				*/
				auto tempProperty = std::make_shared<SymbolProperty>(
					child					
					);

				_result = tempProperty->toResult();
				if (!_result)
					return;

				properties.push_back(tempProperty);
				hasTypes.push_back(tempProperty);
			}
			else
			{
				_result = Result(Result::Code::JSON_OBJECT_HAS_INVALID_VALUE);
				return;
			}
		}
	}
}

std::string LibraryInterfaceGenerator::Implementation::SymbolClass::getCppName()
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
	return name;
}

std::string LibraryInterfaceGenerator::Implementation::SymbolClass::getKotlinName()
{
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
	return name;
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SymbolClass::change(SymbolObjectTable& objectTable)
{
	for (auto& _base : _bases)
	{
		auto objectIter = objectTable.find(_base);
		if (objectIter != objectTable.end())
		{
			auto objectptr = objectIter->second;
			if (auto object = objectptr.lock())
			{
				auto clazz = std::dynamic_pointer_cast<SymbolClass>(object);
				if (clazz)
				{
					this->bases.push_back(clazz);
					continue;
				}
			}
		}
		return Result(Result::Code::CANNOT_FIND_CLASSNAME, "There is no class (%s)", _base.c_str());
	}
	return Result(Result::Code::SUCCESS);
}

void LibraryInterfaceGenerator::Implementation::SymbolClass::addEnumTable(SymbolEnumTable& enumTable, std::shared_ptr<SymbolEnum>& value)
{
	std::string key{ "" };
	for (auto& moduleName : value->parentModules)
	{
		key += moduleName;
		key += "/";
	}
	for (auto& objectName : value->parentObjects)
	{
		key += objectName;
		key += "/";
	}
	key += value->name;
	enumTable[key] = value;
}