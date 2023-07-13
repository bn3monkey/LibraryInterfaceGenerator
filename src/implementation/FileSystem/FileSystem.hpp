#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_FILESYSTEM__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_FILESYSTEM__

#include "../Result/Result.hpp"
#include "../Auxiliary/StringHelper.hpp"
#include <string>
#include <filesystem>
#include <fstream>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        namespace FileSystem
        {
            Result createDirectory(const std::string& path);
            Result createDirectories(const std::string& path);
            Result createFile(const std::string& path, const std::string& content);
            Result findAllFilePath(const std::string& path, std::vector<std::string>& file_paths, const std::vector<std::string>& postfixes);

            Result copyDirectories(const std::string& src_path, const std::string& dest_path);
            Result removeDirectory(const std::string& path);
        }
    }
}

#endif