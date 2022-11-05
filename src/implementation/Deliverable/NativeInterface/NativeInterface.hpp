#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVEINTERFACE__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVEINTERFACE__

#include <string>
#include <algorithm>

#include "../../FileSystem/FileSystem.hpp"
#include "../../Result/Result.hpp"
#include "../../SymbolTable/SymbolTable.hpp"
#include "../../Auxiliary/Definition.hpp"
#include "../../Auxiliary/StringHelper.hpp"
#include "../../SourceStream/CXXSourceStream.hpp"
#include "../NativeSourceDirectory/NativeSourceDirectory.hpp"
#include "../../ExternalLibrary/NativeExternalLibraryDirectory.hpp"
#include "../../Environment.hpp"
#include "../../Converter/CXXConverter.hpp"

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
                std::string root_dir_path = ".",
                const char* interface_dirname = DEFAULT_DIRECTORY_NAME 
            );

            inline operator bool() { return !_result; }
            Result toError() {
                return _result;
            }

            Result make();

            std::string getInterfaceDirName() const { return _interface_dir_name;}
            std::string getInterfaceDirPath() const { return _interface_dir_path;}
            std::string getRootNamespace() const { return root_namespace; }

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

            SourceStream createInterfaceHeaderContent(const SymbolPackage& symbolObject);
            SourceStream createInterfaceCppContent(const SymbolPackage& symbolObject);
            
            void createPackageDeclaration(SourceStream& ss, const SymbolPackage& obj);
            void createModuleDeclaration(SourceStream& ss, const SymbolModule& obj);
            void createClassDeclaration(SourceStream& ss, const SymbolClass& clazz);

            void createPackageDefinition(SourceStream& ss, const SymbolPackage& obj);
            void createModuleDefinition(SourceStream& ss, const SymbolModule& obj);
            void createClassDefinition(SourceStream& ss, const SymbolClass& clazz);

            void createConstructorDeclaration(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& consturctor);
            void createConstructorDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& constructor);
            void allocate(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& consturctor);

            void createDestructorDeclaration(SourceStream& ss, const SymbolClass& clazz);
            void createDestructorDefinition(SourceStream& ss, const SymbolClass& clazz);
            void deallocate(SourceStream& ss, const SymbolClass& clazz);

            void createAddReleaserDeclaration(SourceStream& ss, const SymbolClass& clazz);
            void createAddReleaserDefinition(SourceStream& ss, const SymbolClass& clazz);
            void addReleaser(SourceStream& ss, const SymbolClass& clazz);

            void createClassMethodDeclaration(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& obj);
            void createClassMethodDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& obj);
            void callClassMethod(SourceStream& ss, const SymbolMethod& obj);

            void createStaticMethodDeclaration(SourceStream& ss, const SymbolMethod& obj);
            void createStaticMethodDefinition(SourceStream& ss, const SymbolMethod& obj);
            void callStaticMethod(SourceStream& ss, const SymbolMethod& obj);


            void createReturnValueChanger(SourceStream& ss, const SymbolMethod& obj);
            void createInputParameterChanger(SourceStream& ss, const SymbolParameter& obj);
            void createOutputParameterChanger(SourceStream& ss, const SymbolParameter& obj);

            // Property Block ����
            std::string createPropertyName(const SymbolProperty& object);

            void createPropertySetterDeclaration(SourceStream& ss, const std::string& propertyName, const SymbolProperty& obj);
            void createPropertyGetterDeclaration(SourceStream& ss, const std::string& propertyName, const SymbolProperty& obj);
            void createPropertyDeclaration(SourceStream& ss, const SymbolProperty& obj);

            void callPropertySetter(SourceStream& ss, const SymbolClass& clazz, const SymbolProperty& obj);
            void callPropertyGetter(SourceStream& ss, const SymbolClass& clazz, const SymbolProperty& obj);
            void createPropertySetterDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolProperty& obj);
            void createPropertyGetterDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolProperty& obj);
            void createPropertyDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolProperty& obj);

            void createInputPropertyChanger(SourceStream& ss, const SymbolProperty& obj);
            void createOutputPropertyChanger(SourceStream& ss, const SymbolProperty& obj);

            std::string createNativeInterfaceConverter();
        };
    }
}

#endif