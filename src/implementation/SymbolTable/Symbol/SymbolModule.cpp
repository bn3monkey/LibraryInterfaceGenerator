#include "SymbolModule.hpp"

using namespace LibraryInterfaceGenerator::Implementation::Definition;

LibraryInterfaceGenerator::Implementation::SymbolModule::SymbolModule(
	const nlohmann::json& object, 
	std::vector<std::string>& parentModules,
	SymbolObjectTable& objectTable, 
	SymbolEnumTable& enumTable,
	SymbolCallbackTable& callbackTable,
	std::vector<std::weak_ptr<HasSymbolType>>& hasTypes)
{
	std::unordered_map<std::string, int> method_name_map;

	{
		auto iter = object.find(Field::Name);
		if (iter == object.end())
		{
			_result = Result(Result::Code::JSON_OBJECT_HAS_NO_NAME);
			return;
		}
		name = iter->get<std::string>();
	}

	moduleNames = parentModules;
	moduleNames.push_back(name);

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

			if (order == Order::Enum)
			{
				std::vector<std::string> objects{  };

				//SymbolEnum temp{ child, parentModules, objects };
				auto tempEnum = std::make_shared<SymbolEnum>(
					child,
					moduleNames,
					objects
					);

				_result = tempEnum->toResult();
				if (!_result)
					return;

				enums.push_back(tempEnum);
				addEnumTable(enumTable, tempEnum);
			}
			else if (order == Order::Interface)
			{
				std::vector<std::string> objects{  };

				/*
				SymbolClass temp{ 
					child, 
					true, 
					parentModules, 
					objects,
					objectTable,
					enumTable,
					hasTypes
				};
				*/
				
				auto tempInterface = std::make_shared<SymbolClass>(
					child,
					true,
					moduleNames,
					objects,
					objectTable,
					enumTable,
					hasTypes
					);

				_result = tempInterface->toResult();
				if (!_result)
					return;

				interfaces.push_back(tempInterface);
				addObjectTable(objectTable, tempInterface);
			}
			else if (order == Order::Class)
			{
				std::vector<std::string> objects{  };

				/*
				SymbolClass temp{
					child,
					true,
					parentModules,
					objects,
					objectTable,
					enumTable,
					hasTypes
				};
				*/
				auto tempClass = std::make_shared<SymbolClass>(
					child,
					false,
					moduleNames,
					objects,
					objectTable,
					enumTable,
					hasTypes
					);

				_result = tempClass->toResult();
				if (!_result)
					return;

				classes.push_back(tempClass);
				addObjectTable(objectTable, tempClass);
			}
			else if (order == Order::Method)
			{
				auto tempMethod = std::make_shared<SymbolMethod>(
					child,
					moduleNames,
					hasTypes,
					_object_references,
					_enum_references,
					_callback_references
				);

				_result = tempMethod->toResult();
				if (!_result)
					return;


				std::string method_name = tempMethod->name;
				auto name_iter = method_name_map.find(method_name);
				int name_count = 0;
				if (name_iter != method_name_map.end())
				{
					name_count = ++(name_iter->second);
				}
				else
				{
					method_name_map[method_name] = 0;
				}

				global_methods.push_back(std::make_pair(tempMethod, name_count));
				hasTypes.push_back(tempMethod);
			}
			else if (order == Order::Module)
			{

				auto tempModule = std::make_shared<SymbolModule>(
					child,
					moduleNames,
					objectTable,
					enumTable,
					callbackTable,
					hasTypes
					);

				_result = tempModule->toResult();
				if (!_result)
					return;

				submodules.push_back(tempModule);
			}
			else if (order == Order::Callback)
			{
				auto tempCallback = std::make_shared<SymbolCallback>(
					child,
					moduleNames,
					hasTypes
					);
				_result = tempCallback->toResult();
				if (!_result)
					return;

				callbacks.push_back(tempCallback);
				hasTypes.push_back(tempCallback);
				addCallbackTable(callbackTable, tempCallback);
			}
			else
			{
				_result = Result(Result::Code::JSON_OBJECT_HAS_INVALID_VALUE);
				return;
			}
		}
	}
}

std::vector<std::weak_ptr<LibraryInterfaceGenerator::Implementation::SymbolObject>> LibraryInterfaceGenerator::Implementation::SymbolModule::collectAllClassReference() const
{
	std::vector<std::weak_ptr<LibraryInterfaceGenerator::Implementation::SymbolObject>> ret;
	for (auto& reference : _object_references)
	{
		ret.push_back(reference);
	}
	return ret;
}

std::vector<std::weak_ptr<LibraryInterfaceGenerator::Implementation::SymbolObject>> LibraryInterfaceGenerator::Implementation::SymbolModule::collectAllEnumReference() const
{
	std::vector<std::weak_ptr<LibraryInterfaceGenerator::Implementation::SymbolObject>> ret;
	for (auto& reference : _enum_references)
	{
		ret.push_back(reference);
	}
	return ret;
}

std::vector<std::weak_ptr<LibraryInterfaceGenerator::Implementation::SymbolObject>> LibraryInterfaceGenerator::Implementation::SymbolModule::collectAllCallbackReference() const
{
	std::vector<std::weak_ptr<LibraryInterfaceGenerator::Implementation::SymbolObject>> ret;
	for (auto& reference : _callback_references)
	{
		ret.push_back(reference);
	}
	return ret;
}

void LibraryInterfaceGenerator::Implementation::SymbolModule::addEnumTable(SymbolEnumTable& enumTable, std::shared_ptr<SymbolEnum>& value)
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

void LibraryInterfaceGenerator::Implementation::SymbolModule::addObjectTable(SymbolObjectTable& objectTable, std::shared_ptr<SymbolClass>& value)
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
	objectTable[key] = value;
}

void LibraryInterfaceGenerator::Implementation::SymbolModule::addCallbackTable(SymbolCallbackTable& callbackTable, std::shared_ptr<SymbolCallback>& value)
{
	std::string key{ "" };
	for (auto& moduleName : value->parentModules)
	{
		key += moduleName;
		key += "/";
	}
	key += value->name;
	callbackTable[key] = value;
}
