// LibraryInterfaceGeneratorWindow.cpp : 애플리케이션의 진입점을 정의합니다.
//

#include "LibraryInterfaceGeneratorWindow.h"
#include <fstream>
#include <filesystem>

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;

	using std::filesystem::current_path;
	auto currentPath = current_path();
	auto currentPathStr = currentPath.native();

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

	std::string root_dir_path = ".\\library";
	LibraryInterfaceGenerator::createRootDirectory(root_dir_path);
	LibraryInterfaceGenerator::createNativeSourceDirectory(content, root_dir_path);

	return 0;
}
