#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVEINTERFACE__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVEINTERFACE__

#include <string>
#include <algorithm>

#include "../FileSystem/FileSystem.hpp"
#include "../Result/Result.hpp"
#include "../SymbolTable/SymbolTable.hpp"
#include "../Auxiliary/Definition.hpp"
#include "../Auxiliary/StringHelper.hpp"
#include "../NativeSourceStream/NativeSourceStream.hpp"
#include "../NativeSourceDirectory/NativeSourceDirectory.hpp";
#include "../NativeExternalLibraryDirectory/NativeExternalLibraryDirectory.hpp";
#include "../Environment.hpp"

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class NativeInterface
        {
        public:
            static constexpr char* DEFAULT_DIRECTORY_NAME = "interface";

            explicit NativeInterface(
                Environment environment, 
                const NativeExternalLibraryDirectory& libDirectory,
                const NativeSourceDirectory& srcDirectory, 
                const SymbolTable& symbolTable,
                const char* interface_dirname = DEFAULT_DIRECTORY_NAME, 
                std::string root_dir_path = "."
                );
            inline operator bool() { return !_result; }
            Result toError() {
                return _result;
            }

            Result make();

            std::string getInterfaceDirName() const { return _interface_dir_name;}
            std::string getInterfaceDirPath() const { return _interface_dir_path;}

        private:
            const NativeSourceDirectory& _srcDirectory;
            const NativeExternalLibraryDirectory& _libDirectory;
            const SymbolTable& _symbolTable;
            Result _result;

            std::string _interface_dir_name;
            std::string _interface_dir_path;
            
            std::string export_macro;
            std::string api_macro;
            std::string root_namespace;

            Result createInterfaceFile(const SymbolPackage& symbolObject, std::string& parent_include_path);
            Result createInterfaceContent(const SymbolPackage& symbolObject, std::string& header_content, std::string& cpp_content);
            
            void createPackageDeclaration(const SymbolPackage& symbolObject, std::stringstream& ss);
            void createModuleDeclaration(const SymbolModule& symbolObject, std::stringstream& ss, std::string& indent);
            void createClassDeclaration(const SymbolClass& clazz, std::stringstream& ss, std::string& indent);

            Result createPackageDefinition(const SymbolPackage& symbolObject, std::stringstream& ss);
            void createModuleDefinition(const SymbolModule& mod, std::stringstream& ss);
            void createClassDefinition(const SymbolClass& clazz, std::stringstream& ss);

            // Method Block ����
            std::string createConstructorDeclaration(const SymbolClass& clazz, const SymbolMethod& constructor);
            std::vector<std::string> createConstructorDefinition(const SymbolClass& clazz, const SymbolMethod& constructor);
            std::string allocate(const SymbolClass& clazz, const SymbolMethod& constructor);

            std::string createDestructorDeclaration(const SymbolClass& clazz);
            std::vector<std::string> createDestructorDefinition(const SymbolClass& clazz);
            std::string deallocate(const SymbolClass& clazz);

            std::string createClassMethodDeclaration(const SymbolMethod& object);
            std::vector<std::string> createClassMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object);
            std::string callClassMethod(const SymbolMethod& object);

            std::string createStaticMethodDeclaration(const SymbolMethod& object);
            std::vector<std::string> createStaticMethodDefinition(const SymbolMethod& object);
            std::string callStaticMethod(const SymbolMethod& object);

            // Parameter Block ����
            std::string createParametersDefinition(const SymbolMethod& object);
            std::string createParameterDefinition(const SymbolParameter& object);

            std::vector<std::string> createReturnValueChanger(const SymbolMethod& object);
            std::vector<std::string> createInputParameterChanger(const SymbolParameter& object);
            std::vector<std::string> createOutputParameterChanger(const SymbolParameter& object);
            

            // Property Block ����
            std::string createPropertyName(const SymbolProperty& object);
            
            std::string createPropertySetterDeclaration(const std::string& propertyName, const SymbolProperty& object);
            std::string createPropertyGetterDeclaration(const std::string& propertyName, const SymbolProperty& object);
            std::vector<std::string> createPropertyDeclaration(const SymbolProperty& object);

            std::string createPropertySetterDeclaration(const std::string& scope, const std::string& propertyName, const SymbolProperty& object);
            std::string createPropertyGetterDeclaration(const std::string& scope, const std::string& propertyName, const SymbolProperty& object);
            std::vector<std::string> callPropertySetter(const SymbolClass& clazz, const SymbolProperty& object);
            std::vector<std::string> callPropertyGetter(const SymbolClass& clazz, const SymbolProperty& object);
            std::vector<std::string> createPropertyDefinition(const SymbolClass& clazz, const SymbolProperty& object);

            std::vector<std::string> createInputPropertyChanger(const SymbolProperty& object);
            std::vector<std::string> createOutputPropertyChanger(const SymbolProperty& object);

            std::string createScope(const SymbolClass& clazz);
            std::string createScope(const SymbolMethod& method);

            std::vector<std::string> createMemoryPoolFunctions();
        };
    }
}

#endif