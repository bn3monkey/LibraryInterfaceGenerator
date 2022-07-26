#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_WRAPPER__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_WRAPPER__

#include <string>
#include <algorithm>

#include "../FileSystem/FileSystem.hpp"
#include "../Result/Result.hpp"
#include "../SymbolTable/SymbolTable.hpp"
#include "../Auxiliary/Definition.hpp"
#include "../Auxiliary/StringHelper.hpp"
#include "../NativeSourceStream/NativeSourceStream.hpp"
#include "../NativeInterface/NativeInterface.hpp"
#include "../Environment.hpp"
#include "../Converter/KotlinWrapperConverter.hpp"

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class Wrapper
        {
        public:
            static constexpr char* DEFAULT_DIRECTORY_NAME = "wrapper";
            explicit Wrapper(
                Environment environment, 
                const NativeInterface& interfaceDirectory, 
                const SymbolTable& symbolTable, 
                std::string root_dir_path = ".", 
                const char* directory_name = DEFAULT_DIRECTORY_NAME);
            virtual ~Wrapper();

            inline operator bool() { return !_result; }
            Result toError() {
                return _result;
            }

            Result make();

            inline std::string getKotlinPackageName() const {
                return _kotlin_package_name;
            }
            inline std::string getKotlinWrapperClassName() const {
                return _kotlin_wrapper_class_name;
            }

        private:
            const NativeInterface& _infDirectory;
            const SymbolTable& _symbolTable;
            Result _result;
            
            std::string _wrapper_dir_path;

            std::string _kotlin_package_name;
            std::string _kotlin_wrapper_class_name;

            Result createWrapperFile(const SymbolPackage& symbolObject, std::string& parent_include_path);
            Result createWrapperContent(const SymbolPackage& symbolObject, std::string& native_content, std::string& target_content);
            
            Result createPackageDefinition(const SymbolPackage& symbolObject, std::stringstream& ss);
            void createModuleDefinition(const std::string& prefix, const SymbolModule& mod, std::stringstream& ss);
            void createClassDefinition(const std::string& prefix, const SymbolClass& clazz, std::stringstream& ss);

            std::vector<std::string> createConstructorDefinition(const std::string& prefix, const SymbolClass& clazz, const SymbolMethod& constructor, int number);
            std::string callConstructor(const SymbolClass& clazz, const SymbolMethod& constructor);

            std::vector<std::string> createDestructorDefinition(const std::string& prefix, const SymbolClass& clazz);
            std::string callDestructor(const SymbolClass& clazz);

            std::vector<std::string> createClassMethodDefinition(const std::string& prefix, const SymbolClass& clazz, const SymbolMethod& object, int number);
            std::string callClassMethod(const SymbolClass& clazz, const SymbolMethod& object);

            std::vector<std::string> createStaticMethodDefinition(const std::string& prefix, const SymbolMethod& object, int number);
            std::string callStaticMethod(const SymbolMethod& object);

            std::string createParametersDefinition(const SymbolMethod& object);
            std::string createParameterDefinition(const SymbolParameter& object);

            std::string createReturnValueChanger(const SymbolMethod& object);
            std::string createInputParameterChanger(const SymbolParameter& object);
            std::string createOutputParameterChanger(const SymbolParameter& object);

            std::string createPropertyName(const SymbolProperty& object);
            std::string createPropertySetterDeclaration(const std::string& prefix, const std::string& scope, const std::string& propertyName, const SymbolProperty& object);
            std::string createPropertyGetterDeclaration(const std::string& prefix, const std::string& scope, const std::string& propertyName, const SymbolProperty& object);
            std::string callPropertySetter(const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object);
            std::string callPropertyGetter(const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object);
            std::vector<std::string> createPropertyDefinition(const std::string& prefix, const SymbolClass& clazz, const SymbolProperty& object);

            std::string createInputPropertyChanger(const SymbolProperty& object);
            std::string createOutputPropertyChanger(const SymbolProperty& object);

            std::string createScope(const SymbolClass& clazz);
            std::string createScope(const SymbolMethod& method);
            std::string createNativeScope(const SymbolClass& clazz);
            std::string createNativeScope(const SymbolMethod& method);

            std::string createChangerFunction();

            
            Result createWrapperPackageDeclaration(const SymbolPackage& symbolObject, std::stringstream& ss, std::string indent);
            void createWrapperModuleDeclaration(const SymbolModule& mod, std::stringstream& ss, std::string indent);
            void createWrapperClassDeclaration(const SymbolClass& clazz, std::stringstream& ss, std::string indent);

            std::string createWrapperConstructorDeclaration(const SymbolClass& clazz, const SymbolMethod& constructor, int number);
            std::string createWrapperDestructorDeclaration(const SymbolClass& clazz);
            std::string createWrapperClassMethodDeclaration(const SymbolClass& clazz, const SymbolMethod& object, int number);
            std::string createWrapperStaticMethodDeclaration(const SymbolMethod& object, int number);

            std::string createWrapperParametersDeclaration(const SymbolMethod& object);
            std::string createWrapperParameterDeclaration(const SymbolParameter& object);

            std::string createWrapperPropertySetterDeclaration(const std::string& scope, const std::string& propertyName, const SymbolProperty& object);
            std::string createWrapperPropertyGetterDeclaration(const std::string& scope, const std::string& propertyName, const SymbolProperty& object);
            std::vector<std::string> createWrapperPropertyDeclaration(const SymbolClass& clazz, const SymbolProperty& object);

            std::string createWrapperScope(const SymbolClass& clazz);
            std::string createWrapperScope(const SymbolMethod& method);
        };
    }
}
#endif