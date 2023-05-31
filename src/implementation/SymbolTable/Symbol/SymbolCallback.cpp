#include "SymbolCallback.hpp"

using namespace LibraryInterfaceGenerator::Implementation::Definition;

LibraryInterfaceGenerator::Implementation::SymbolCallback::SymbolCallback(const nlohmann::json& object,
                const std::vector<std::string>& module_paths,
                std::vector<std::weak_ptr<HasSymbolType>>& hasTypes
                ) : parentModules(module_paths)
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
					_object_references,
					_enum_references,
					_callback_references
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

std::string LibraryInterfaceGenerator::Implementation::SymbolCallback::getCppName() const
{
	std::string value{ "" };
	for (auto& moduleName : parentModules)
	{
		value += moduleName;
		value += "::";
	}
	value += name;
	return value;
}

std::string LibraryInterfaceGenerator::Implementation::SymbolCallback::getManagedName() const
{
	return name;
}

std::string LibraryInterfaceGenerator::Implementation::SymbolCallback::getKotlinName() const
{
	return name;
}

std::vector<std::weak_ptr<LibraryInterfaceGenerator::Implementation::SymbolObject>> LibraryInterfaceGenerator::Implementation::SymbolCallback::collectAllClassReference() const
{
	std::vector<std::weak_ptr<LibraryInterfaceGenerator::Implementation::SymbolObject>> ret;
	for (auto& reference : _object_references)
	{
		ret.push_back(reference);
	}
	return ret;
}

std::vector<std::weak_ptr<LibraryInterfaceGenerator::Implementation::SymbolObject>> LibraryInterfaceGenerator::Implementation::SymbolCallback::collectAllEnumReference() const
{
	std::vector<std::weak_ptr<LibraryInterfaceGenerator::Implementation::SymbolObject>> ret;
	for (auto& reference : _enum_references)
	{
		ret.push_back(reference);
	}
	return ret;
}

std::vector<std::weak_ptr<LibraryInterfaceGenerator::Implementation::SymbolObject>> LibraryInterfaceGenerator::Implementation::SymbolCallback::collectAllCallbackReference() const
{
	std::vector<std::weak_ptr<LibraryInterfaceGenerator::Implementation::SymbolObject>> ret;
	for (auto& reference : _callback_references)
	{
		ret.push_back(reference);
	}
	return ret;
}

std::weak_ptr<LibraryInterfaceGenerator::Implementation::SymbolType> LibraryInterfaceGenerator::Implementation::SymbolCallback::returnType()
{
	return type;
}

std::vector<std::weak_ptr<LibraryInterfaceGenerator::Implementation::SymbolType>> LibraryInterfaceGenerator::Implementation::SymbolCallback::parameterTypes()
{
	std::vector<std::weak_ptr<SymbolType>> ret{};
	for (auto& param : parameters)
	{
		ret.push_back(param->type);
	}
	return ret;
}
