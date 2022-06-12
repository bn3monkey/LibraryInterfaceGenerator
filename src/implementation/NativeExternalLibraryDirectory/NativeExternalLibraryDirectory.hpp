#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVEEXTERNALLIBRARYDIRECTORY__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVEEXTERNALLIBRARYDIRECTORY__

#include <string>
#include "../FileSystem/FileSystem.hpp"
#include "../Result/Result.hpp"
#include "../SymbolTable/SymbolTable.hpp"

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class NativeExternalLibraryDirectory
        {
        public:
            static constexpr char* DirectoryName = "lib";
            enum class ExternalTool : size_t
            {
                Log,
                MemoryPool,
            };

            explicit NativeExternalLibraryDirectory(std::string root_dir_path = ".");
            inline operator bool() { return !_result; }
            Result toError() {
                return _result;
            }

            Result createLibraryDirectory();
            Result createExternalTool(ExternalTool tool);

            inline bool existsExternalTool(ExternalTool tool) const {
                return is_created[(size_t)tool];
            }

            std::string getRelativeHeaderPath(ExternalTool tool) const;
            std::string getRelativeHeaderPath(ExternalTool tool, const SymbolModule& modull) const;
            std::string getRelativeHeaderPath(ExternalTool tool, const SymbolEnum& enumm) const;
            std::string getRelativeHeaderPath(ExternalTool tool, const SymbolClass& clazz) const;

        private:
            Result _result;
            std::string _lib_dir_path;

            bool is_created[2] {false, false};
        };
    }
}
#endif