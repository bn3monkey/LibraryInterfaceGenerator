#include <Deliverable/SourceDirectory/SourceDirectory.hpp>
#include <SymbolTable/SymbolTable.hpp>
#include <ExternalLibrary/NativeExternalLibraryDirectory.hpp>

#include <../json/json.hpp>
#include <gtest/gtest.h>

#include <cstdio>
#include <filesystem>

TEST(SourceDirectory, SourceDirectory)
{
	using std::filesystem::current_path;
	auto currentPath = current_path();
	auto currentPathStr = currentPath.native();
	std::string root_dir_path = ".\\library";
	std::string content;

	{
		// build ..\\
		// out ..\..\
		// LibraryInerfaceGenerator ..\..\..\
		// LibraryInterfaceGenreatorWindow ..\..\..\..\
		// prj ..\..\..\..\..\
		// LibraryInterfacegnerartor ..\\..\\..\\..\\..\\..\\

		std::ifstream ifs("..\\..\\..\\..\\..\\..\\test\\sample\\HsnUltrasoundDICOM.json");
		if (ifs.is_open())
		{
			ifs.seekg(0, ifs.end);
			int length = ifs.tellg();
			ifs.seekg(0, ifs.beg);

			char* buffer = new char[length];
			memset(buffer, 0, length);

			ifs.read(buffer, length);
			content = std::move(std::string{ buffer });

			delete[] buffer;
		}
	}

	using namespace LibraryInterfaceGenerator::Implementation;

	nlohmann::json object;
	try
	{
		object = nlohmann::json::parse(content, nullptr, true, true);
	}
	catch (std::exception& e)
	{
		printf("%s", e.what());
	}

	SymbolTable symbolTable{ object };
	if (!symbolTable)
	{
		auto result = symbolTable.toResult();
		printf("%s", result.toString().c_str());
	}

	LibraryInterfaceGenerator::Implementation::NativeExternalLibraryDirectory nativeExternalLibraryDirectory{ root_dir_path };
	nativeExternalLibraryDirectory.createLibraryDirectory();
	nativeExternalLibraryDirectory.createExternalTool(NativeExternalLibraryDirectory::ExternalTool::Log);
	nativeExternalLibraryDirectory.createExternalTool(NativeExternalLibraryDirectory::ExternalTool::MemoryPool);

	LibraryInterfaceGenerator::Implementation::NativeSourceDirectory nativeSourceDirectory{ nativeExternalLibraryDirectory, symbolTable, root_dir_path };

	LibraryInterfaceGenerator::Implementation::NativeInterface nativeInterface{
		Environment::Kotlin_Android,
		nativeExternalLibraryDirectory,
		nativeSourceDirectory,
		symbolTable,
		root_dir_path
	};

	LibraryInterfaceGenerator::Implementation::Wrapper wrapper{ Environment::Kotlin_Android, nativeExternalLibraryDirectory, nativeInterface, symbolTable, root_dir_path };
	
	LibraryInterfaceGenerator::Implementation::SourceDirectory sourceDirectory{ Environment::Kotlin_Android, wrapper, symbolTable, root_dir_path };

	auto result = sourceDirectory.make();
	if (!result)
	{
		printf("%s", result.toString().c_str());
	}
}