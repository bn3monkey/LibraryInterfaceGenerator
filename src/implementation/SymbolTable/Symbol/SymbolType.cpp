#include "SymbolType.hpp"
#include <sstream>

std::shared_ptr<LibraryInterfaceGenerator::Implementation::SymbolType> LibraryInterfaceGenerator::Implementation::makeType(
	const std::string& type,
	const SymbolObjectTable& objectTable,
	const SymbolEnumTable& enumTable,
	const SymbolCallbackTable& callbackTable,
	ObjectReferenceSet* objectReferenceSet,
	EnumReferenceSet* enumReferenceSet,
	CallbackReferenceSet* callbackReferenceSet)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	if (type == "void") {
		return std::make_shared<SymbolTypeVoid>();
	}
	else if (type == "bool")
	{
		return std::make_shared<SymbolTypeBool>();
	}
	else if (type == "int8")
	{
		return std::make_shared<SymbolTypeInt8>();
	}
	else if (type == "int16")
	{
		return std::make_shared<SymbolTypeInt16>();
	}
	else if (type == "int32")
	{
		return std::make_shared<SymbolTypeInt32>();
	}
	else if (type == "int64")
	{
		return std::make_shared<SymbolTypeInt64>();
	}
	else if (type == "float")
	{
		return std::make_shared<SymbolTypeFloat>();
	}
	else if (type == "double")
	{
		return std::make_shared<SymbolTypeDouble>();
	}
	else if (type == "string")
	{
		return std::make_shared<SymbolTypeString>();
	}
	else
	{
		if (hasPrefix(type, "array<") && hasPostfix(type, ">"))
		{
			auto array_info = std::string(type.begin() + sizeof("array<") - 1, type.end() - 1);
			std::stringstream ss {array_info};
			std::string inner_type;
			std::getline(ss, inner_type, ',');
			std::string size_str;
			std::getline(ss, size_str, ',');
			size_t size = std::atoi(size_str.c_str());

			if (inner_type == "bool")
			{
				return std::make_shared< SymbolTypeArray<SymbolTypeBool>>(size);
			}
			else if (inner_type == "int8")
			{
				return std::make_shared<SymbolTypeArray<SymbolTypeInt8>>(size);
			}
			else if (inner_type == "int16")
			{
				return std::make_shared< SymbolTypeArray<SymbolTypeInt16>>(size);
			}
			else if (inner_type == "int32")
			{
				return std::make_shared< SymbolTypeArray<SymbolTypeInt32>>(size);
			}
			else if (inner_type == "int64")
			{
				return std::make_shared< SymbolTypeArray<SymbolTypeInt64>>(size);
			}
			else if (inner_type == "float")
			{
				return std::make_shared< SymbolTypeArray<SymbolTypeFloat>>(size);
			}
			else if (inner_type == "double")
			{
				return std::make_shared<SymbolTypeArray<SymbolTypeDouble>>(size);
			}
			else if (inner_type == "string")
			{
				return std::make_shared< SymbolTypeArray<SymbolTypeString>>(size);
			}
			else
			{
				auto object_iter = objectTable.find(inner_type);
				if (object_iter != objectTable.end())
				{
					auto object = object_iter->second;
					if (objectReferenceSet != nullptr)
					{
						objectReferenceSet->insert(object);
					}

					return std::make_shared<SymbolTypeArray<SymbolTypeObject>>(object, size);
				}
				auto enum_iter = enumTable.find(inner_type);
				if (enum_iter != enumTable.end())
				{
					auto object = enum_iter->second;
					if (enumReferenceSet != nullptr)
					{
						enumReferenceSet->insert(object);
					}

					return std::make_shared< SymbolTypeArray<SymbolTypeEnum>>(object, size);
				}
				auto callback_iter = callbackTable.find(inner_type);
				if (callback_iter != callbackTable.end())
				{
					auto callback = callback_iter->second;
					if (callbackReferenceSet != nullptr)
					{
						callbackReferenceSet->insert(callback);
					}

					return std::make_shared< SymbolTypeArray<SymbolTypeCallback>>(callback, size);
				}
			}
		}
		else if (hasPrefix(type, "vector<") && hasPostfix(type, ">"))
		{
			auto inner_type = std::string(type.begin() + sizeof("vector<") - 1, type.end() - 1);
			if (inner_type == "bool")
			{
				return std::make_shared<SymbolTypeVector<SymbolTypeBool>>();
			}
			else if (inner_type == "int8")
			{
				return std::make_shared<SymbolTypeVector<SymbolTypeInt8>>();
			}
			else if (inner_type == "int16")
			{
				return std::make_shared<SymbolTypeVector<SymbolTypeInt16>>();
			}
			else if (inner_type == "int32")
			{
				return std::make_shared<SymbolTypeVector<SymbolTypeInt32>>();
			}
			else if (inner_type == "int64")
			{
				return std::make_shared<SymbolTypeVector<SymbolTypeInt64>>();
			}
			else if (inner_type == "float")
			{
				return std::make_shared<SymbolTypeVector<SymbolTypeFloat>>();
			}
			else if (inner_type == "double")
			{
				return std::make_shared<SymbolTypeVector<SymbolTypeDouble>>();
			}
			else if (inner_type == "string")
			{
				return std::make_shared< SymbolTypeVector<SymbolTypeString>>();
			}
			else
			{
				auto object_iter = objectTable.find(inner_type);
				if (object_iter != objectTable.end())
				{
					auto object = object_iter->second;
					if (objectReferenceSet != nullptr)
					{
						objectReferenceSet->insert(object);
					}
					return std::make_shared< SymbolTypeVector<SymbolTypeObject>>(object);
				}
				auto enum_iter = enumTable.find(inner_type);
				if (enum_iter != enumTable.end())
				{
					auto object = enum_iter->second;
					if (enumReferenceSet != nullptr)
					{
						enumReferenceSet->insert(object);
					}
 					return std::make_shared< SymbolTypeVector<SymbolTypeEnum>>(object);
				}

				auto callback_iter = callbackTable.find(inner_type);
				if (callback_iter != callbackTable.end())
				{
					auto callback = callback_iter->second;
					if (callbackReferenceSet != nullptr)
					{
						callbackReferenceSet->insert(callback);
					}

					return std::make_shared< SymbolTypeVector<SymbolTypeCallback>>(callback);
				}
			}
		}
		else
		{
			auto object_iter = objectTable.find(type);
			if (object_iter != objectTable.end())
			{
				auto object = object_iter->second;
				if (objectReferenceSet != nullptr)
				{
					objectReferenceSet->insert(object);
				}
				return std::make_shared<SymbolTypeObject>(object);
			}
			auto enum_iter = enumTable.find(type);
			if (enum_iter != enumTable.end())
			{
				auto object = enum_iter->second;
				if (enumReferenceSet != nullptr)
				{
					enumReferenceSet->insert(object);
				}
				return std::make_shared<SymbolTypeEnum>(object);
			}

			auto callback_iter = callbackTable.find(type);
			if (callback_iter != callbackTable.end())
			{
				auto callback = callback_iter->second;
				if (callbackReferenceSet != nullptr)
				{
					callbackReferenceSet->insert(callback);
				}

				return std::make_shared< SymbolTypeCallback>(callback);
			}
		}
	}
	return std::make_shared<SymbolType>();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::HasSymbolType::change(SymbolObjectTable& objectTable, SymbolEnumTable& enumTable, SymbolCallbackTable& callbackTable)
{
	type = LibraryInterfaceGenerator::Implementation::makeType(_type, objectTable, enumTable, callbackTable, _objectReferenceSet, _enumReferenceSet, _callbackReferenceSet);
	if (type->valid())
	{
		_type.clear();
		return Result(Result::Code::SUCCESS);
	}
	else
	{
		return Result(Result::Code::SYMBOL_TYPE_CAST_FAIL, "Symbol not exists (%s)", _type.c_str());
	}
}

void LibraryInterfaceGenerator::Implementation::HasSymbolType::registerReferenceSet(ObjectReferenceSet* objectReferenceSet, EnumReferenceSet* enumReferenceSet, CallbackReferenceSet* callbackReferenceSet)
{
	_objectReferenceSet = objectReferenceSet;
	_enumReferenceSet = enumReferenceSet;
	_callbackReferenceSet = callbackReferenceSet;
}
