#include "../include/LibraryInterfaceGenerator.hpp"
#include "SymbolTable/SymbolTable.hpp"

LibraryInterfaceGenerator::Error LibraryInterfaceGenerator::createNativeSourceDirectory(const std::string& json_content)
{
	using namespace LibraryInterfaceGenerator::Implementation;

	nlohmann::json object;
	try
	{
		object = nlohmann::json::parse(json_content);
	}
	catch (std::exception& e)
	{
		return Error(Error::Code::FAIL, e.what());
	}
	SymbolTable symbolTable{ object };
	if (!symbolTable)
	{
		auto result = symbolTable.toResult();
		return Error(Error::Code::FAIL, result.toString());
	}

	auto& package = symbolTable.getPackage();

	return Error(Error::Code::SUCCESS);
}
LibraryInterfaceGenerator::Error LibraryInterfaceGenerator::createAPIDocumentation(const std::string& json_content)
{
	return Error(Error::Code::SUCCESS);
}
LibraryInterfaceGenerator::Error LibraryInterfaceGenerator::createNativeInterface(const std::string& json_content)
{
	return Error(Error::Code::SUCCESS);
}
LibraryInterfaceGenerator::Error LibraryInterfaceGenerator::createWrapper(const std::string& json_content, Framework env)
{
	return Error(Error::Code::SUCCESS);
}
LibraryInterfaceGenerator::Error LibraryInterfaceGenerator::createSourceDirectory(const std::string& json_content, Framework env)
{
	return Error(Error::Code::SUCCESS);
}