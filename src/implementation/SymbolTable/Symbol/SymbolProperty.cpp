#include "SymbolProperty.hpp"

using namespace LibraryInterfaceGenerator::Implementation::Definition;

LibraryInterfaceGenerator::Implementation::SymbolProperty::SymbolProperty(
	const nlohmann::json& object, 
	ObjectReferenceSet& parentObjectReferenceSet,
	EnumReferenceSet& parentEnumReferenceSet,
	CallbackReferenceSet& parentCallbackReferenceSet)
{
	_result = Result(Result::Code::SUCCESS);

	registerReferenceSet(&parentObjectReferenceSet, &parentEnumReferenceSet, &parentCallbackReferenceSet);

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
		auto iter = object.find(Field::Type);
		if (iter == object.end())
		{
			_result = Result(Result::Code::JSON_OBJECT_HAS_NO_TYPE);
			return;
		}

		_type = iter->get<std::string>();
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
		auto iter = object.find(Field::ReadOnly);
		if (iter == object.end())
		{
			_result = Result(Result::Code::JSON_OBJECT_HAS_NO_IO);
			return;
		}

		readonly = iter->get<bool>();
	}

}
