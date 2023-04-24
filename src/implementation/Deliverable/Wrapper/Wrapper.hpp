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
#include "../../SourceStream/KotlinSourceStream.hpp"
#include "../NativeInterface/NativeInterface.hpp"
#include "../../Environment.hpp"
#include "../../Converter/CXXConverter.hpp"
#include "../../Converter/KotlinConverter.hpp"
#include "../../ExternalLibrary/KotlinWrapperConverter/KotlinWrapperConverter_cpp.h"

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
            constexpr static char* JNIEXPORT = "extern \"C\" JNIEXPORT";

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
            
            void createNativeConstructorDefinition(SourceStream& ss, const std::string& prefix, const SymbolClass& clazz, const SymbolMethod& constructor, int number);
            void callNativeConstructor(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& constructor);

            void createNativeDestructorDefinition(SourceStream& ss, const std::string& prefix, const SymbolClass& clazz);
            void callNativeDesturctor(SourceStream& ss, const SymbolClass& clazz);

            void createNativeAddReleaserDefinition(SourceStream& ss, const std::string& prefix, const SymbolClass& clazz);
            void callNativeAddReleaser(SourceStream& ss, const SymbolClass& clazz);

            void createNativeClassMethodDefinition(SourceStream& ss, const std::string& prefix, const SymbolClass& clazz, const SymbolMethod& object, int number);
            void callNativeClassMethod(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object);

            void createNativeStaticMethodDefinition(SourceStream& ss, const std::string& prefix, const SymbolMethod& object, int number);
            void callNativeStaticMethod(SourceStream& ss, const SymbolMethod& object);

            void createNativeCallbackChanger(SourceStream& ss);
            void createNativeHandleChanger(SourceStream& ss);
            void createNativeReturnValueChanger(SourceStream& ss, const SymbolMethod& object);
            void createNativeInputParameterChanger(SourceStream& ss, const SymbolParameter& object);
            void createNativeOutputParameterChanger(SourceStream& ss, const SymbolParameter& object);

            std::string createPropertyName(const SymbolProperty& object);
            
            void createNativePropertySetterDeclaration(SourceStream& ss, const std::string& prefix, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object);
            void createNativePropertyGetterDeclaration(SourceStream& ss, const std::string& prefix, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object);
            void callNativePropertySetter(SourceStream& ss, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object);
            void callNativePropertyGetter(SourceStream& ss, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object);
            void createNativePropertyDefinition(SourceStream& ss, const std::string& prefix, const SymbolClass& clazz, const SymbolProperty& object);

            void createNativeInputPropertyChanger(SourceStream& ss, const SymbolProperty& object);
            void createNativeOutputPropertyChanger(SourceStream& ss, const SymbolProperty& object);

            void createChangerFunction(SourceStream& ss);


            void createWrapperPackageDeclaration(SourceStream& ss, const SymbolPackage& symbolObject);
            void createWrapperModuleDeclaration(SourceStream& ss, const SymbolModule& mod);
            void createWrapperClassDeclaration(SourceStream& ss, const SymbolClass& clazz);

            void createWrapperConstructorDeclaration(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& constructor, int number);
            void createWrapperDestructorDeclaration(SourceStream& ss, const SymbolClass& clazz);
            void createWrapperAddReleaserDeclaration(SourceStream& ss, const SymbolClass& clazz);
            void createWrapperClassMethodDeclaration(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object, int number);
            void createWrapperStaticMethodDeclaration(SourceStream& ss, const SymbolMethod& object, int number);

            
            void createWrapperPropertySetterDeclaration(SourceStream& ss, const std::string& property_name, const SymbolClass& clazz, const SymbolProperty& object);
            void createWrapperPropertyGetterDeclaration(SourceStream& ss, const std::string& property_name, const SymbolClass& clazz, const SymbolProperty& object);
            void createWrapperPropertyDeclaration(SourceStream& ss, const SymbolClass& clazz, const SymbolProperty& object);

        };
    }
}


#endif