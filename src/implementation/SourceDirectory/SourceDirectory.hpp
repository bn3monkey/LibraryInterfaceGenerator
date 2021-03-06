#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SOURCEDIRECTORY__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SOURCEDIRECTORY__

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

            // 1. Module Directory 제작
            Result createModule(const SymbolModule& object, std::string& parent_path);
            Result createInterfaceFile(const SymbolClass& object, std::string& parent_path);
            Result createClassFile(const SymbolClass& object, std::string& parent_path);
            Result createEnumFile(const SymbolEnum& object, std::string& parent_path);
            Result createMethodFile(const SymbolModule& object, std::string& parent_path);
            
            // 2. Interface 전문 제작
            Result createInterfaceFileContent(const SymbolClass& object, std::string& content);

            // 3. Class 전문 제작
            Result createClassFileContent(const SymbolClass& object, std::string& content);

            // 4. Enum 전문 제작
            Result createEnumFileContent(const SymbolEnum& object, std::string& content);

            // 5. Method 전문 제작
            Result createMethodFileContent(const SymbolModule& object, std::string& content);

            // 7. Code Block 제작
            void addForwardDeclaration(std::stringstream& ss, std::string& indent, const SymbolClass& object);
            void addForwardDeclaration(std::stringstream& ss, std::string& indent, const SymbolModule& object);

            // Enum Block 생성
            std::vector<std::string> createEnumDefinition(const SymbolEnum& object);

            // Method Block 생성

            std::vector<std::string> createConstructorDefinition(const SymbolClass& clazz, const SymbolMethod& object, int number);
            std::vector<std::string> createDestructorDefinition(const SymbolClass& clazz);

            std::vector<std::string> createInterfaceMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object);
            std::vector<std::string> createClassMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object, int number);
            std::vector<std::string> createDerivedMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object, int number);
            std::vector<std::string> createStaticMethodDefinition(const SymbolMethod& object, int number);

            std::string callClassMethod(const SymbolClass& clazz, const SymbolMethod& object, int number);
            std::string callStaticMethod(const SymbolMethod& object, int number);

            std::string createReturnValueChanger(const SymbolMethod& object);
            std::string createInputParameterChanger(const SymbolParameter& object);
            std::string createOutputParameterChanger(const SymbolParameter& object);
            
            // Parameter Block 생성
            std::string createParametersDefinition(const SymbolMethod& object);
            std::string createParameterDefinition(const SymbolParameter& object);

            // Property Block 생성

            std::string createInputPropertyChanger(const SymbolProperty& object);
            std::string createOutputPropertyChanger(const SymbolProperty& object);

            std::string createPropertyName(const SymbolProperty& object);
            std::vector<std::string> createPropertySetterDefinition(const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object);
            std::vector<std::string> createPropertyGetterDefinition(const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object);

            std::vector<std::string> createInterfacePropertyDefinition(const SymbolClass& clazz, const SymbolProperty& object);
            std::vector<std::string> createClassPropertyDefinition(const SymbolClass& clazz, const SymbolProperty& object);
            std::vector<std::string> createDerivedPropertyDefinition(const SymbolClass& clazz, const SymbolProperty& object);

            std::string createWrapperScope(const SymbolClass& clazz);
            std::string createWrapperScope(const SymbolMethod& method);

        };
    }
}
#endif