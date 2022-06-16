#include "../include/LibraryInterfaceGenerator.hpp"
#include "SymbolTable/SymbolTable.hpp"
#include "NativeExternalLibraryDirectory/NativeExternalLibraryDirectory.hpp"
#include "NativeSourceDirectory/NativeSourceDirectory.hpp"
#include "NativeInterface/NativeInterface.hpp"
#include "Wrapper/Wrapper.hpp"
#include "SourceDirectory/SourceDirectory.hpp"
#include "FileSystem/FileSystem.hpp"

LibraryInterfaceGenerator::Error LibraryInterfaceGenerator::createRootDirectory(const std::string& root_dir_path)
{
	auto ret = Implementation::FileSystem::createDirectories(root_dir_path);
	if (!ret)
	{
		return Error(Error::Code::FAIL, ret.toString());
	}
	return Error(Error::Code::SUCCESS);
}

LibraryInterfaceGenerator::Error LibraryInterfaceGenerator::createNativeSourceDirectory(const std::string& json_content, const std::string& root_dir_path)
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

	LibraryInterfaceGenerator::Implementation::NativeExternalLibraryDirectory nativeExternalLibraryDirectory{root_dir_path };
	nativeExternalLibraryDirectory.createLibraryDirectory();
	nativeExternalLibraryDirectory.createExternalTool(NativeExternalLibraryDirectory::ExternalTool::Log);
	nativeExternalLibraryDirectory.createExternalTool(NativeExternalLibraryDirectory::ExternalTool::MemoryPool);

	LibraryInterfaceGenerator::Implementation::NativeSourceDirectory nativeSourceDirectory{ nativeExternalLibraryDirectory, symbolTable, root_dir_path };
	auto result = nativeSourceDirectory.make();
	if (!result)
	{
		return Error(Error::Code::FAIL, result.toString());
	}

	LibraryInterfaceGenerator::Implementation::NativeInterface nativeInterface{ Environment::Kotlin_Android, nativeExternalLibraryDirectory, nativeSourceDirectory, symbolTable, root_dir_path };
	result = nativeInterface.make();
	if (!result)
	{
		return Error(Error::Code::FAIL, result.toString());
	}

	LibraryInterfaceGenerator::Implementation::Wrapper wrapper{ Environment::Kotlin_Android, nativeInterface, symbolTable, root_dir_path };
	result = wrapper.make();
	if (!result)
	{
		return Error(Error::Code::FAIL, result.toString());
	}

	LibraryInterfaceGenerator::Implementation::SourceDirectory sourceDirectory{ Environment::Kotlin_Android, wrapper, symbolTable, root_dir_path };
	result = sourceDirectory.make();
	if (!result)
	{
		return Error(Error::Code::FAIL, result.toString());
	}
	return Error(Error::Code::SUCCESS);
}
LibraryInterfaceGenerator::Error LibraryInterfaceGenerator::createAPIDocumentation(const std::string& json_content, const std::string& root_dir_path)
{
	return Error(Error::Code::SUCCESS);
}
LibraryInterfaceGenerator::Error LibraryInterfaceGenerator::createNativeInterface(const std::string& json_content, const std::string& root_dir_path)
{
	return Error(Error::Code::SUCCESS);
}
LibraryInterfaceGenerator::Error LibraryInterfaceGenerator::createWrapper(const std::string& json_content, Framework env, const std::string& root_dir_path)
{
	return Error(Error::Code::SUCCESS);
}
LibraryInterfaceGenerator::Error LibraryInterfaceGenerator::createSourceDirectory(const std::string& json_content, Framework env, const std::string& root_dir_path)
{
	return Error(Error::Code::SUCCESS);
}