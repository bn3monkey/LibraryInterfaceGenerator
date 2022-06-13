#include "Wrapper.hpp"

LibraryInterfaceGenerator::Implementation::Wrapper::Wrapper(Environment& environment, const NativeInterface& interfaceDirectory, const SymbolTable& symbolTable, std::string root_dir_path)
{
}

LibraryInterfaceGenerator::Implementation::Wrapper::~Wrapper()
{
}

Result LibraryInterfaceGenerator::Implementation::Wrapper::make()
{
	return Result();
}

Result LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperFile(const SymbolPackage& symbolObject, std::string& parent_include_path)
{
	return Result();
}

Result LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperContent(const SymbolPackage& symbolObject, std::string& header_content, std::string& cpp_content)
{
	return Result();
}

Result LibraryInterfaceGenerator::Implementation::Wrapper::createPackageDefinition(const SymbolPackage& symbolObject, std::stringstream& ss)
{
	return Result();
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createModuleDefinition(const SymbolModule& mod, std::stringstream& ss)
{
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createClassDefinition(const SymbolClass& clazz, std::stringstream& ss)
{
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::createConstructorDefinition(const SymbolClass& clazz, const SymbolMethod& constructor)
{
	return std::vector<std::string>();
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::callConstructor(const SymbolClass& clazz, const SymbolMethod& constructor)
{
	return std::string();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::createDestructorDefinition(const SymbolClass& clazz)
{
	return std::vector<std::string>();
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::callDestructor(const SymbolClass& clazz)
{
	return std::string();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::createClassMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object)
{
	return std::vector<std::string>();
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::callClassMethod(const SymbolMethod& object)
{
	return std::string();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::createStaticMethodDefinition(const SymbolMethod& object)
{
	return std::vector<std::string>();
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::callStaticMethod(const SymbolMethod& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createParametersDefinition(const SymbolMethod& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createParameterDefinition(const SymbolParameter& object)
{
	return std::string();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::createReturnValueChanger(const SymbolMethod& object)
{
	return std::vector<std::string>();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::createInputParameterChanger(const SymbolParameter& object)
{
	return std::vector<std::string>();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::createOutputParameterChanger(const SymbolParameter& object)
{
	return std::vector<std::string>();
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createPropertyName(const SymbolProperty& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createPropertySetterDeclaration(const std::string& scope, const std::string& propertyName, const SymbolProperty& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createPropertyGetterDeclaration(const std::string& scope, const std::string& propertyName, const SymbolProperty& object)
{
	return std::string();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::callPropertySetter(const SymbolClass& clazz, const SymbolProperty& object)
{
	return std::vector<std::string>();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::callPropertyGetter(const SymbolClass& clazz, const SymbolProperty& object)
{
	return std::vector<std::string>();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::createPropertyDefinition(const SymbolClass& clazz, const SymbolProperty& object)
{
	return std::vector<std::string>();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::createInputPropertyChanger(const SymbolProperty& object)
{
	return std::vector<std::string>();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::createOutputPropertyChanger(const SymbolProperty& object)
{
	return std::vector<std::string>();
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createScope(const SymbolClass& clazz)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createScope(const SymbolMethod& method)
{
	return std::string();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::createChangerFunction()
{
	return std::vector<std::string>();
}
