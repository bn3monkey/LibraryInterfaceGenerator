#include "Wrapper.hpp"
#include "Converter.hpp"

LibraryInterfaceGenerator::Implementation::Wrapper::Wrapper(Environment& environment, const NativeInterface& interfaceDirectory, const SymbolTable& symbolTable, std::string root_dir_path)
{
}

LibraryInterfaceGenerator::Implementation::Wrapper::~Wrapper()
{
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::Wrapper::make()
{
	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperFile(const SymbolPackage& symbolObject, std::string& parent_include_path)
{
	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperContent(const SymbolPackage& symbolObject, std::string& header_content, std::string& cpp_content)
{
	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::Wrapper::createPackageDefinition(const SymbolPackage& symbolObject, std::stringstream& ss)
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

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createChangerFunction()
{

	// bool : bool <-> jboolean
	// int8 : int8_t <-> jbyte
	// int16 : int16_t <-> jshort
	// int32 : int32_t <-> jint
	// int64 : int64_t <-> jlong
	// float : float <-> jfloat
	// double : double <-> jdouble
	// string : std::string <-> jstring (�Լ� �ʿ�)
	// enum : int <-> jint
	// object : void* <-> jlong
	// 
	// array<bool>   : std::vector<bool> <-> jbooleanArray
	// array<int8>   : std::vector<int8_t> <-> jbyteArray
	// array<int16>  : std::vector<int16_t> <-> jshortArray
	// array<int32>  : std::vector<int32_t> <-> jintArray
	// array<int64>  : std::vector<int64_t> <-> jlongArray
	// array<float>  : std::vector<float> <-> jfloatArray
	// array<double> : std::vector<double> <-> jdoubleArray
	// array<string> : std::vector<std::string> <-> jobjectArray
	// array<enum>   : std::vector<int> <-> jintArray
	// array<object> : std::vector<void*> <-> jlongArray
	// 
	// vector<bool>   : std::vector<bool> <-> java/util/ArrayList & java/lang/Boolean
	// vector<int8>   : std::vector<int8_t> <-> java/util/ArrayList & java/lang/Byte
	// vector<int16>  : std::vector<int16_t> <-> java/util/ArrayList & java/lang/Short
	// vector<int32>  : std::vector<int32_t> <-> java/util/ArrayList & java/lang/Integer
	// vector<int64>  : std::vector<int64_t> <-> java/util/ArrayList & java/lang/Long
	// vector<float>  : std::vector<float> <-> java/util/ArrayList & java/lang/Float
	// vector<double> : std::vector<double> <-> java/util/ArrayList & java/lang/Double
	// vector<string> : std::vector<std::string> <-> java/util/ArrayList & java/lang/String
	// vector<enum>   : std::vector<int> <-> java/util/ArrayList & java/lang/Integer
	// vector<object> : std::vector<void*> <-> java/util/ArrayList & java/lang/Long 

	return converter;
}
