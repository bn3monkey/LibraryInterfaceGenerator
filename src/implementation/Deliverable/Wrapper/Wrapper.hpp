#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_WRAPPER__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_WRAPPER__

#include <string>
#include <algorithm>

#include "../../FileSystem/FileSystem.hpp"
#include "../../Result/Result.hpp"
#include "../../SymbolTable/SymbolTable.hpp"
#include "../../Auxiliary/Definition.hpp"
#include "../../Auxiliary/StringHelper.hpp"
#include "../../SourceStream/CXXSourceStream.hpp"
#include "../NativeInterface/NativeInterface.hpp"
#include "../../Environment.hpp"
#include "../../ExternalLibrary/KotlinWrapperConverter/KotlinWrapperConverter.h"

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
            
            SourceStream createNativeWrapperContent(const SymbolPackage& symbolObject);
            SourceStream createKotlinWrapperContent(const SymbolPackage& symbolObject);

            void createNativePackageDefinition(SourceStream& ss, const SymbolPackage& symbolObject);
            void createNativeModuleDefinition(SourceStream& ss, const std::string& prefix, const SymbolModule& mod);
            void createNativeClassDefinition(SourceStream& ss, const std::string& prefix, const SymbolClass& clazz);
            
            void createConstructorDefinition(SourceStream& ss, const std::string& prefix, const SymbolClass& clazz, const SymbolMethod& constructor, int number);
            void callConstructor(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& constructor);

            void createDestructorDefinition(SourceStream& ss, const std::string& prefix, const SymbolClass& clazz);
            void callDesturctor(SourceStream& ss, const SymbolClass& clazz);

            void createAddReleasedEfinition(SourceStream& ss, const std::string& prefix, const SymbolClass& clazz);
            void callAddReleaser(SourceStream& ss, const SymbolClass& clazz);

            void createClassMethodDefinition(SourceStream& ss, const std::string& prefix, const SymbolClass& clazz, const SymbolMethod& object, int number);
            void callClassMethod(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object);

            void createStaticMethodDefinition(SourceStream& ss, const std::string& prefix, const SymbolMethod& object, int number);
            void callStaticMethod(SourceStream& ss, const SymbolMethod& object);

            void createParametersDefinition(SourceStream& ss, const SymbolMethod& object);
            void createParameterDefinition(SourceStream& ss, const SymbolParameter& object);

            void createReturnValueChanger(SourceStream& ss, const SymbolMethod& object);
            void createInputParameterChanger(SourceStream& ss, const SymbolParameter& object);
            void createOutputParameterChanger(SourceStream& ss, const SymbolParameter& object);

            void createPropertyName(SourceStream& ss, const SymbolProperty& object);
            void createPropertySetterDeclaration(SourceStream& ss, const std::string& prefix, const std::string& scope, const std::string& propertyName, const SymbolProperty& object);
            void createPropertyGetterDeclaration(SourceStream& ss, const std::string& prefix, const std::string& scope, const std::string& propertyName, const SymbolProperty& object);
            void callPropertySetter(SourceStream& ss, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object);
            void callPropertyGetter(SourceStream& ss, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object);
            void createPropertyDefinition(SourceStream& ss, const std::string& prefix, const SymbolClass& clazz, const SymbolProperty& object);

            void createInputPropertyChanger(SourceStream& ss, const SymbolProperty& object);
            void createOutputPropertyChanger(SourceStream& ss, const SymbolProperty& object);

            void createChangerFunction(SourceStream& ss);


            void createWrapperPackageDeclaration(SourceStream& ss, const SymbolPackage& symbolObject);
            void createWrapperModuleDeclaration(SourceStream& ss, const SymbolModule& mod);
            void createWrapperClassDeclaration(SourceStream& ss, const SymbolClass& clazz);

            void createWrapperConstructorDeclaration(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& constructor, int number);
            void createWrapperDestructorDeclaration(SourceStream& ss, const SymbolClass& clazz);
            void createWrapperAddReleaserDeclaration(SourceStream& ss, const SymbolClass& clazz);
            void createWrapperClassMethodDeclaration(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object, int number);
            void createWrapperStaticMethodDeclaration(SourceStream& ss, const SymbolMethod& object, int number);

            void createWrapperParametersDeclaration(SourceStream& ss, const SymbolMethod& object);
            void createWrapperParameterDeclaration(SourceStream& ss, const SymbolParameter& object);

            void createWrapperPropertySetterDeclaration(SourceStream& ss, const std::string& scope, const std::string& propertyName, const SymbolProperty& object);
            void createWrapperPropertyGetterDeclaration(SourceStream& ss, const std::string& scope, const std::string& propertyName, const SymbolProperty& object);
            void createWrapperPropertyDeclaration(SourceStream& ss, const SymbolClass& clazz, const SymbolProperty& object);


            /*
            Result createWrapperContent(const SymbolPackage& symbolObject, std::string& native_content, std::string& target_content);
            
            Result createPackageDefinition(const SymbolPackage& symbolObject, std::stringstream& ss);
            void createModuleDefinition(const std::string& prefix, const SymbolModule& mod, std::stringstream& ss);
            void createClassDefinition(const std::string& prefix, const SymbolClass& clazz, std::stringstream& ss);

            std::vector<std::string> createConstructorDefinition(const std::string& prefix, const SymbolClass& clazz, const SymbolMethod& constructor, int number);
            std::string callConstructor(const SymbolClass& clazz, const SymbolMethod& constructor);

            std::vector<std::string> createDestructorDefinition(const std::string& prefix, const SymbolClass& clazz);
            std::string callDestructor(const SymbolClass& clazz);

            std::vector<std::string> createAddReleaserDefinition(const std::string& prefix, const SymbolClass& clazz);
            std::vector<std::string> callAddReleaser(const SymbolClass& clazz);

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
            std::string createWrapperAddReleaserDeclaration(const SymbolClass& clazz);
            std::string createWrapperClassMethodDeclaration(const SymbolClass& clazz, const SymbolMethod& object, int number);
            std::string createWrapperStaticMethodDeclaration(const SymbolMethod& object, int number);

            std::string createWrapperParametersDeclaration(const SymbolMethod& object);
            std::string createWrapperParameterDeclaration(const SymbolParameter& object);

            std::string createWrapperPropertySetterDeclaration(const std::string& scope, const std::string& propertyName, const SymbolProperty& object);
            std::string createWrapperPropertyGetterDeclaration(const std::string& scope, const std::string& propertyName, const SymbolProperty& object);
            std::vector<std::string> createWrapperPropertyDeclaration(const SymbolClass& clazz, const SymbolProperty& object);

            std::string createWrapperScope(const SymbolClass& clazz);
            std::string createWrapperScope(const SymbolMethod& method);
            */
        };
    }
}
#endif