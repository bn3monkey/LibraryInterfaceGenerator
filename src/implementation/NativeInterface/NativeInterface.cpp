#include "Nativeinterface.hpp"

#ifdef __linux__
static char* delimeter = "/";
#elif _WIN32
static char* delimeter = "\\";
#else
static char* delimeter = "/";
#endif

LibraryInterfaceGenerator::Implementation::NativeInterface::NativeInterface(Environment environment, const SymbolTable& symbolTable, std::string root_dir_path)
	: _symbolTable(symbolTable)
{
	_interface_dir_path = root_dir_path;
	_interface_dir_path += delimeter;
	_interface_dir_path += "interface";

}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::NativeInterface::make()
{
	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::NativeInterface::createInterfaceFile(const SymbolPackage& symbolObject, std::string& parent_include_path)
{
	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::NativeInterface::createInterfaceContent(const SymbolPackage& symbolObject, std::string& header_content, std::string& cpp_content)
{
	return Result();
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::wrapPackageScope(const SymbolPackage& symbolObject, std::stringstream& ss)
{
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::wrapModuleScope(const SymbolModule& symbolObject, std::stringstream& ss, std::string& indent)
{
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createClassDeclaration(const SymbolClass& clazz, const SymbolMethod& object)
{
	return std::vector<std::string>();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createConstructorDeclaration(const SymbolClass& clazz, const SymbolMethod& object)
{
	return std::string();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createConstructorDefinition(const SymbolClass& clazz, const SymbolMethod& object)
{
	return std::vector<std::string>();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::allocate(const SymbolClass& clazz, const SymbolMethod& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createDestructorDeclaration(const SymbolClass& clazz)
{
	return std::string();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createDestructorDefinition(const SymbolClass& clazz)
{
	return std::vector<std::string>();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::deallocate(const SymbolClass& clazz)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createClassMethodDeclaration(const SymbolMethod& object)
{
	return std::string();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createClassMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object)
{
	return std::vector<std::string>();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::callClassMethod(const SymbolClass& clazz, const SymbolMethod& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createStaticMethodDeclaration(const SymbolMethod& object)
{
	return std::string();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createStaticMethodDefinition(const SymbolMethod& object)
{
	return std::vector<std::string>();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::callStaticMethod(const SymbolMethod& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createParametersDefinition(const SymbolMethod& parameters)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createParameterDefinition(const SymbolParameter& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createInputParameterChanger(const SymbolParameter& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createOutputParameterChanger(const SymbolParameter& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertyName(const SymbolProperty& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertySetterDeclaration(const std::string& propertyName, const SymbolProperty& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertyGetterDeclaration(const std::string& propertyName, const SymbolProperty& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::callPropertySetter(const std::string& propertyName, const SymbolProperty& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::callPropertyGetter(const std::string& propertyName, const SymbolProperty& object)
{
	return std::string();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertyDeclaration(const SymbolProperty& object)
{
	return std::vector<std::string>();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertyDefinition(const SymbolClass& clazz, const SymbolProperty& object)
{
	return std::vector<std::string>();
}
