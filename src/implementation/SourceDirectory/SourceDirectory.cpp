#include "SourceDirectory.hpp"

LibraryInterfaceGenerator::Implementation::SourceDirectory::SourceDirectory(Environment environment, const Wrapper& wrapperDirectory, const SymbolTable& symbolTable, std::string root_dir_path, const char* dir_name)
{
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::make()
{
	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createModule(const SymbolModule& object, std::string& parent_include_path, std::string& parent_src_path)
{
	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createInterfaceFile(const SymbolClass& object, std::string& parent_include_path)
{
	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createClassFile(const SymbolClass& object, std::string& parent_include_path, std::string& parent_src_path)
{
	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createEnumFile(const SymbolEnum& object, std::string& parent_include_path)
{
	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createMethodFile(const SymbolModule& object, std::string& parent_include_path, std::string& parent_src_path)
{
	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createInterfaceFileContent(const SymbolClass& object, std::string& header_content)
{
	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createClassFileContent(const SymbolClass& object, std::string& header_content, std::string& cpp_content)
{
	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createEnumFileContent(const SymbolEnum& object, std::string& header_content)
{
	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createMethodFileContent(const SymbolModule& object, std::string& header_content, std::string& cpp_content)
{
	return Result();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createEnumDefinition(const SymbolEnum& object)
{
	return std::vector<std::string>();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createConstructorDefinition(const SymbolClass& clazz, const SymbolMethod& object)
{
	return std::vector<std::string>();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createDestructorDefinition(const SymbolClass& clazz)
{
	return std::vector<std::string>();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createInterfaceMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object)
{
	return std::vector<std::string>();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createClassMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object)
{
	return std::vector<std::string>();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createDerivedMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object)
{
	return std::vector<std::string>();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createStaticMethodDefinition(const SymbolMethod& object)
{
	return std::vector<std::string>();
}

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::createParametersDefinition(const SymbolMethod& parameters)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::createParameterDefinition(const SymbolParameter& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::createPropertyName(const SymbolProperty& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::createPropertySetterDeclaration(const std::string& propertyName, const SymbolProperty& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::createPropertyGetterDeclaration(const std::string& propertyName, const SymbolProperty& object)
{
	return std::string();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createInterfacePropertyDefinition(const SymbolProperty& object)
{
	return std::vector<std::string>();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createClassPropertyDefinition(const SymbolProperty& object)
{
	return std::vector<std::string>();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createDerivedPropertyDefinition(const SymbolProperty& object)
{
	return std::vector<std::string>();
}
