#include "SymbolMethod.hpp"

using namespace LibraryInterfaceGenerator::Implementation::Definition;

LibraryInterfaceGenerator::Implementation::SymbolMethod::SymbolMethod(
	const nlohmann::json& object, 
	const std::vector<std::string>& module_paths, 
	std::vector<std::weak_ptr<HasSymbolType>>& hasTypes,
	ObjectReferenceSet& parentObjectReferenceSet,
	EnumReferenceSet& parentEnumReferenceSet) : parentModules(module_paths)
{
	_result = Result(Result::Code::SUCCESS);

	registerReferenceSet(&parentObjectReferenceSet, &parentEnumReferenceSet);
	

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
		auto iter = object.find(Field::Return);
		if (iter == object.end())
		{
			_result = Result(Result::Code::JSON_OBJECT_HAS_NO_RETURN);
			return;
		}

		return_description = iter->get<std::string>();
	}

	{
		auto iter = object.find(Field::Parameters);
		if (iter == object.end())
		{
			_result = Result(Result::Code::JSON_OBJECT_HAS_NO_PARAMETERS);
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

			if (order == Order::Parameter)
			{
				//std::vector<std::string> paths = { name };
				auto tempParameter = std::make_shared<SymbolParameter>(
					child,
					parentObjectReferenceSet,
					parentEnumReferenceSet
					);
				_result = tempParameter->toResult();
				if (!_result)
					return;

				parameters.push_back(tempParameter);
				hasTypes.push_back(tempParameter);
			}
			else
			{
				_result = Result(Result::Code::JSON_OBJECT_HAS_INVALID_VALUE);
				return;
			}
		}
	}
}


