#include "SymbolType.hpp"

bool hasPrefix(const std::string& type, std::string prefix)
{
	return type.compare(0, prefix.size(), prefix.data()) == 0;
}
bool hasPostfix(const std::string& type, std::string postfix)
{
	const size_t start = type.size() - postfix.size();
	return type.compare(start, type.size(), postfix.data()) == 0;
}

std::unique_ptr<LibraryInterfaceGenerator::Implementation::SymbolType> LibraryInterfaceGenerator::Implementation::makeType(
	const std::string& type,
	const SymbolObjectTable& objectTable,
	const SymbolEnumTable& enumTable)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	if (type == "void") {
		return std::make_unique<SymbolTypeVoid>();
	}
	else if (type == "bool")
	{
		return std::make_unique<SymbolTypeBool>();
	}
	else if (type == "int8")
	{
		return std::make_unique<SymbolTypeInt8>();
	}
	else if (type == "int16")
	{
		return std::make_unique<SymbolTypeInt16>();
	}
	else if (type == "int32")
	{
		return std::make_unique<SymbolTypeInt32>();
	}
	else if (type == "int64")
	{
		return std::make_unique<SymbolTypeInt64>();
	}
	else if (type == "float")
	{
		return std::make_unique<SymbolTypeFloat>();
	}
	else if (type == "double")
	{
		return std::make_unique<SymbolTypeDouble>();
	}
	else if (type == "string")
	{
		return std::make_unique<SymbolTypeString>();
	}
	else
	{
		if (hasPrefix(type, "array<") && hasPostfix(type, ">"))
		{
			auto inner_type = std::string(type.begin() + sizeof("array<") - 1, type.end() - 1);
			if (inner_type == "bool")
			{
				return std::make_unique< SymbolTypeArray<SymbolTypeBool>>();
			}
			else if (inner_type == "int8")
			{
				return std::make_unique<SymbolTypeArray<SymbolTypeInt8>>();
			}
			else if (inner_type == "int16")
			{
				return std::make_unique< SymbolTypeArray<SymbolTypeInt16>>();
			}
			else if (inner_type == "int32")
			{
				return std::make_unique< SymbolTypeArray<SymbolTypeInt32>>();
			}
			else if (inner_type == "int64")
			{
				return std::make_unique< SymbolTypeArray<SymbolTypeInt64>>();
			}
			else if (inner_type == "float")
			{
				return std::make_unique< SymbolTypeArray<SymbolTypeFloat>>();
			}
			else if (inner_type == "double")
			{
				return std::make_unique<SymbolTypeArray<SymbolTypeDouble>>();
			}
			else if (inner_type == "string")
			{
				return std::make_unique< SymbolTypeArray<SymbolTypeString>>();
			}
			else
			{
				auto object_iter = objectTable.find(inner_type);
				if (object_iter != objectTable.end())
				{
					auto object = object_iter->second;
					return std::make_unique<SymbolTypeArray<SymbolTypeObject>>(object);
				}
				auto enum_iter = enumTable.find(inner_type);
				if (enum_iter != enumTable.end())
				{
					auto object = enum_iter->second;
					return std::make_unique< SymbolTypeArray<SymbolTypeEnum>>(object);
				}
			}
		}
		else if (hasPrefix(type, "vector<") && hasPostfix(type, ">"))
		{
			auto inner_type = std::string(type.begin() + sizeof("vector<") - 1, type.end() - 1);
			if (inner_type == "bool")
			{
				return std::make_unique<SymbolTypeVector<SymbolTypeBool>>();
			}
			else if (inner_type == "int8")
			{
				return std::make_unique<SymbolTypeVector<SymbolTypeInt8>>();
			}
			else if (inner_type == "int16")
			{
				return std::make_unique<SymbolTypeVector<SymbolTypeInt16>>();
			}
			else if (inner_type == "int32")
			{
				return std::make_unique<SymbolTypeVector<SymbolTypeInt32>>();
			}
			else if (inner_type == "int64")
			{
				return std::make_unique<SymbolTypeVector<SymbolTypeInt64>>();
			}
			else if (inner_type == "float")
			{
				return std::make_unique<SymbolTypeVector<SymbolTypeFloat>>();
			}
			else if (inner_type == "double")
			{
				return std::make_unique<SymbolTypeVector<SymbolTypeDouble>>();
			}
			else if (inner_type == "string")
			{
				return std::make_unique< SymbolTypeVector<SymbolTypeString>>();
			}
			else
			{
				auto object_iter = objectTable.find(inner_type);
				if (object_iter != objectTable.end())
				{
					auto object = object_iter->second;
					return std::make_unique< SymbolTypeVector<SymbolTypeObject>>(object);
				}
				auto enum_iter = enumTable.find(inner_type);
				if (enum_iter != enumTable.end())
				{
					auto object = enum_iter->second;
					return std::make_unique< SymbolTypeVector<SymbolTypeEnum>>(object);
				}
			}
		}
		else
		{
			auto object_iter = objectTable.find(type);
			if (object_iter != objectTable.end())
			{
				auto object = object_iter->second;
				return std::make_unique<SymbolTypeObject>(object);
			}
			auto enum_iter = enumTable.find(type);
			if (enum_iter != enumTable.end())
			{
				auto object = enum_iter->second;
				return std::make_unique<SymbolTypeEnum>(object);
			}
		}
	}
	return std::make_unique<SymbolType>();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::HasSymbolType::change(SymbolObjectTable& objectTable, SymbolEnumTable& enumTable)
{
	type = LibraryInterfaceGenerator::Implementation::makeType(_type, objectTable, enumTable);
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