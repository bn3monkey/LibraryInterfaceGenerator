#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_FILESYSTEM__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_FILESYSTEM__

#include "../Result/Result.hpp"
#include <string>
#include <filesystem>
#include <fstream>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        namespace FileSystem
        {
            Result createDirectory(std::string& path);
            Result createDirectories(std::string& path);
            Result createFile(std::string& path, std::string& content);            
        }
    }
}

#endif