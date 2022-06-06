// LibraryInterfaceGeneratorWindow.cpp : 애플리케이션의 진입점을 정의합니다.
//

#include "LibraryInterfaceGeneratorWindow.h"
#include <fstream>

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;

	std::string content;

	{
		std::ifstream ifs("../../../test/sample/HsnUltrasoundDICOM.json");
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

	LibraryInterfaceGenerator::createNativeSourceDirectory(content);

	return 0;
}
