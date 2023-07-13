// LibraryInterfaceGeneratorWindow.cpp : 애플리케이션의 진입점을 정의합니다.
//

#include "LibraryInterfaceGeneratorWindow.h"
#include <fstream>
#include <filesystem>

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;

	std::string path;
	std::string content;

	{
		std::ifstream ifs("path.json");
		if (ifs.is_open())
		{
			ifs.seekg(0, ifs.end);
			int length = ifs.tellg();
			ifs.seekg(0, ifs.beg);

			path.reserve(length);
			path.resize(length);

			ifs.read(path.data(), length);
		}
	}

	auto config = nlohmann::json::parse(path);
	auto project_path = config["project_path"].get<std::string>();
	auto input_directory = config["input_directory"].get<std::string>();
	auto input_file = config["input_file"].get<std::string>();
	auto output_dir_name = config["output_directory"].get<std::string>();

	std::string input_file_path;
	{
		std::stringstream ss{ input_file_path };
		ss << project_path << "\\" << input_directory << "\\" << input_file;
		input_file_path = ss.str();
	}
	std::string temp_output_directory_path;
	{
		std::stringstream ss{ temp_output_directory_path };
		ss << project_path << "\\" << output_dir_name << "__";
		temp_output_directory_path = ss.str();
	}
	std::string output_directory_path;
	{
		std::stringstream ss{ output_directory_path };
		ss << project_path << "\\" << output_dir_name;
		output_directory_path = ss.str();
	}

	{
		std::ifstream ifs(input_file_path);
		if (ifs.is_open())
		{
			ifs.seekg(0, ifs.end);
			int length = ifs.tellg();
			ifs.seekg(0, ifs.beg);

			content.reserve(length);
			content.resize(length);

			ifs.read(content.data(), length);
		}
	}

	LibraryInterfaceGenerator::createTempRootDirectory(temp_output_directory_path);
	LibraryInterfaceGenerator::createNativeSourceDirectory(content, temp_output_directory_path, output_directory_path);

	return 0;
}
