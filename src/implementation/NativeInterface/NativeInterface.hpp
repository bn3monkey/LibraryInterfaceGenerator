#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCEDIRECTORY__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCEDIRECTORY__

#include <string>

#include "../FileSystem/FileSystem.hpp"
#include "../Result/Result.hpp"
#include "../SymbolTable/SymbolTable.hpp"
#include "../Auxiliary/Definition.hpp"
#include "../Auxiliary/StringHelper.hpp"
#include "../Environment.hpp"

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class NativeInterface
        {
        public:
            explicit NativeInterface(Environment environment, const SymbolTable& symbolTable, std::string root_dir_path = ".");
            inline operator bool() { return !_result; }
            Result toError() {
                return _result;
            }

            Result make();

        private:
            const SymbolTable& _symbolTable;
            Result _result;
            std::string _interface_dir_path;

            Result createInterfaceFile(const SymbolPackage& symbolObject, std::string& parent_include_path);
            Result createInterfaceContent(const SymbolPackage& symbolObject, std::string& header_content, std::string& cpp_content);
            
            void wrapPackageScope(const SymbolPackage& symbolObject, std::stringstream& ss);
            void wrapModuleScope(const SymbolModule& symbolObject, std::stringstream& ss, std::string& indent);

            std::vector<std::string> createClassDeclaration(const SymbolClass& clazz, const SymbolMethod& object);

            // Method Block 持失
            std::string createConstructorDeclaration(const SymbolClass& clazz, const SymbolMethod& object);
            std::vector<std::string> createConstructorDefinition(const SymbolClass& clazz, const SymbolMethod& object);
            std::string allocate(const SymbolClass& clazz, const SymbolMethod& object);

            std::string createDestructorDeclaration(const SymbolClass& clazz);
            std::vector<std::string> createDestructorDefinition(const SymbolClass& clazz);
            std::string deallocate(const SymbolClass& clazz);

            std::string createClassMethodDeclaration(const SymbolMethod& object);
            std::vector<std::string> createClassMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object);
            std::string callClassMethod(const SymbolClass& clazz, const SymbolMethod& object);

            std::string createStaticMethodDeclaration(const SymbolMethod& object);
            std::vector<std::string> createStaticMethodDefinition(const SymbolMethod& object);
            std::string callStaticMethod(const SymbolMethod& object);

            // Parameter Block 持失
            std::string createParametersDefinition(const SymbolMethod& parameters);
            std::string createParameterDefinition(const SymbolParameter& object);

            std::string createInputParameterChanger(const SymbolParameter& object);
            std::string createOutputParameterChanger(const SymbolParameter& object);

            // Property Block 持失
            std::string createPropertyName(const SymbolProperty& object);
            std::string createPropertySetterDeclaration(const std::string& propertyName, const SymbolProperty& object);
            std::string createPropertyGetterDeclaration(const std::string& propertyName, const SymbolProperty& object);
            std::string callPropertySetter(const std::string& propertyName, const SymbolProperty& object);
            std::string callPropertyGetter(const std::string& propertyName, const SymbolProperty& object);

            std::vector<std::string> createPropertyDeclaration(const SymbolProperty& object);
            std::vector<std::string> createPropertyDefinition(const SymbolClass& clazz, const SymbolProperty& object);
        };
    }
}

#endif