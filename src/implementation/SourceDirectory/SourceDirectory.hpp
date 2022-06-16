#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCEDIRECTORY__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCEDIRECTORY__

#include <string>

#include "../FileSystem/FileSystem.hpp"
#include "../Result/Result.hpp"
#include "../SymbolTable/SymbolTable.hpp"
#include "../Auxiliary/Definition.hpp"
#include "../Auxiliary/StringHelper.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Environment.hpp"

#include <algorithm>
#include <set>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SourceDirectory
        {
        public:
            static constexpr char* DEFAULT_DIR_NAME = "java";

            explicit SourceDirectory(
                Environment environment,
                const Wrapper& wrapperDirectory,
                const SymbolTable& symbolTable,
                std::string root_dir_path = ".",
                const char* dir_name = DEFAULT_DIR_NAME);

            inline operator bool() { return !_result; }
            Result toError() {
                return _result;
            }

            Result make();
            inline std::string getSourceDirectory() { return _src_dir_path; }

        private:
            const Wrapper& _wrapperDirectory;
            const SymbolTable& _symbolTable;
            Result _result;

            std::string _src_dir_path;

            // 1. Module Directory 力累
            Result createModule(const SymbolModule& object, std::string& parent_include_path, std::string& parent_src_path);

            Result createInterfaceFile(const SymbolClass& object, std::string& parent_include_path);
            Result createClassFile(const SymbolClass& object, std::string& parent_include_path, std::string& parent_src_path);
            Result createEnumFile(const SymbolEnum& object, std::string& parent_include_path);
            Result createMethodFile(const SymbolModule& object, std::string& parent_include_path, std::string& parent_src_path);
            
            // 2. Interface 傈巩 力累
            Result createInterfaceFileContent(const SymbolClass& object, std::string& header_content);

            // 3. Class 傈巩 力累
            Result createClassFileContent(const SymbolClass& object, std::string& header_content, std::string& cpp_content);

            // 4. Enum 傈巩 力累
            Result createEnumFileContent(const SymbolEnum& object, std::string& header_content);

            // 5. Method 傈巩 力累
            Result createMethodFileContent(const SymbolModule& object, std::string& header_content, std::string& cpp_content);

            // 7. Code Block 力累

            // Enum Block 积己
            std::vector<std::string> createEnumDefinition(const SymbolEnum& object);

            // Method Block 积己

            std::vector<std::string> createConstructorDefinition(const SymbolClass& clazz, const SymbolMethod& object);
            std::vector<std::string> createDestructorDefinition(const SymbolClass& clazz);

            std::vector<std::string> createInterfaceMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object);
            std::vector<std::string> createClassMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object);
            std::vector<std::string> createDerivedMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object);

            std::vector<std::string> createStaticMethodDefinition(const SymbolMethod& object);
            
            // Parameter Block 积己
            std::string createParametersDefinition(const SymbolMethod& parameters);
            std::string createParameterDefinition(const SymbolParameter& object);

            // Property Block 积己
            std::string createPropertyName(const SymbolProperty& object);
            std::string createPropertySetterDeclaration(const std::string& propertyName, const SymbolProperty& object);
            std::string createPropertyGetterDeclaration(const std::string& propertyName, const SymbolProperty& object);

            std::vector<std::string> createInterfacePropertyDefinition(const SymbolProperty& object);
            std::vector<std::string> createClassPropertyDefinition(const SymbolProperty& object);
            std::vector<std::string> createDerivedPropertyDefinition(const SymbolProperty& object);

        };
    }
}
#endif