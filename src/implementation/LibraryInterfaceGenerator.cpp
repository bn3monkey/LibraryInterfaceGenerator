#include "../include/LibraryInterfaceGenerator.hpp"
#include "SymbolTable/SymbolTable.hpp"
#include "ExternalLibrary/NativeExternalLibraryDirectory.hpp"
#include "Deliverable/NativeSourceDirectory/NativeSourceDirectory.hpp"
#include "Deliverable/NativeInterface/NativeInterface.hpp"
#include "Deliverable/Wrapper/Wrapper.hpp"
#include "Deliverable/SourceDirectory/SourceDirectory.hpp"
#include "FileSystem/FileSystem.hpp"

LibraryInterfaceGenerator::Error LibraryInterfaceGenerator::createTempRootDirectory(const std::string& temp_root_dir_path)
{
	auto ret = Implementation::FileSystem::createDirectories(temp_root_dir_path);
	if (!ret)
	{
		return Error(Error::Code::FAIL, ret.toString());
	}
	return Error(Error::Code::SUCCESS);
}

LibraryInterfaceGenerator::Error LibraryInterfaceGenerator::createNativeSourceDirectory(const std::string& json_content, const std::string& temp_root_dir_path, const std::string& root_dir_path)
{
	using namespace LibraryInterfaceGenerator::Implementation;

	nlohmann::json object;
	try
	{
		object = nlohmann::json::parse(json_content, nullptr, true, true);
	}
	catch (std::exception& e)
	{
		printf("%s", e.what());
		return Error(Error::Code::FAIL, e.what());
	}

	SymbolTable symbolTable{ object };
	if (!symbolTable)
	{
		auto result = symbolTable.toResult();
		printf("%s", result.toString().c_str());
		return Error(Error::Code::FAIL, result.toString());
	}

	LibraryInterfaceGenerator::Implementation::NativeExternalLibraryDirectory nativeExternalLibraryDirectory{temp_root_dir_path };
	nativeExternalLibraryDirectory.createLibraryDirectory();
	nativeExternalLibraryDirectory.createExternalTool(NativeExternalLibraryDirectory::ExternalTool::Log);
	nativeExternalLibraryDirectory.createExternalTool(NativeExternalLibraryDirectory::ExternalTool::MemoryPool);
	nativeExternalLibraryDirectory.createExternalTool(NativeExternalLibraryDirectory::ExternalTool::KotlinTypeConverter);
	nativeExternalLibraryDirectory.createExternalTool(NativeExternalLibraryDirectory::ExternalTool::ManagedTypeConverter);

	LibraryInterfaceGenerator::Implementation::KotlinExternalLibraryDirectory kotlinExternalLibraryDirectory{};

	LibraryInterfaceGenerator::Implementation::NativeSourceDirectory nativeSourceDirectory{ nativeExternalLibraryDirectory, symbolTable, temp_root_dir_path };
	auto result = nativeSourceDirectory.make();
	if (!result)
	{
		printf("%s", result.toString().c_str());
		return Error(Error::Code::FAIL, result.toString());
	}

	LibraryInterfaceGenerator::Implementation::NativeInterface nativeInterface{ Environment::Kotlin_Android, nativeExternalLibraryDirectory, nativeSourceDirectory, symbolTable, temp_root_dir_path };
	result = nativeInterface.make();
	if (!result)
	{
		printf("%s", result.toString().c_str());
		return Error(Error::Code::FAIL, result.toString());
	}

	

	LibraryInterfaceGenerator::Implementation::Wrapper wrapper{ Environment::Kotlin_Android, nativeExternalLibraryDirectory, kotlinExternalLibraryDirectory, nativeInterface, symbolTable, temp_root_dir_path };
	result = wrapper.make();
	if (!result)
	{
		printf("%s", result.toString().c_str());
		return Error(Error::Code::FAIL, result.toString());
	}

	LibraryInterfaceGenerator::Implementation::SourceDirectory sourceDirectory{ Environment::Kotlin_Android, wrapper, symbolTable, temp_root_dir_path };
	result = sourceDirectory.make();
	if (!result)
	{
		printf("%s", result.toString().c_str());
		return Error(Error::Code::FAIL, result.toString());
	}
	
	Implementation::FileSystem::copyDirectories(temp_root_dir_path, root_dir_path);
	result = Implementation::FileSystem::removeDirectory(temp_root_dir_path);
	if (!result) {
		printf("%s", result.toString().c_str());
		return Error(Error::Code::FAIL, result.toString());
	}

	return Error(Error::Code::SUCCESS);
}
LibraryInterfaceGenerator::Error LibraryInterfaceGenerator::createAPIDocumentation(const std::string& json_content, const std::string& temp_root_dir_path, const std::string& root_dir_path)
{
	return Error(Error::Code::SUCCESS);
}
LibraryInterfaceGenerator::Error LibraryInterfaceGenerator::createNativeInterface(const std::string& json_content, const std::string& temp_root_dir_path, const std::string& root_dir_path)
{
	return Error(Error::Code::SUCCESS);
}
LibraryInterfaceGenerator::Error LibraryInterfaceGenerator::createWrapper(const std::string& json_content, Framework env, const std::string& temp_root_dir_path, const std::string& root_dir_path)
{
	return Error(Error::Code::SUCCESS);
}
LibraryInterfaceGenerator::Error LibraryInterfaceGenerator::createSourceDirectory(const std::string& json_content, Framework env, const std::string& temp_root_dir_path, const std::string& root_dir_path)
{
	return Error(Error::Code::SUCCESS);
}