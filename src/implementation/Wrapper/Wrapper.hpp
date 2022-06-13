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

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class Wrapper
        {
        public:
            explicit Wrapper(Environment& environment, const NativeInterface& interfaceDirectory, const SymbolTable& symbolTable, std::string root_dir_path = ".");
            virtual ~Wrapper();

            inline operator bool() { return !_result; }
            Result toError() {
                return _result;
            }

            Result make();

        private:
            const NativeInterface& _infDirectory;
            const SymbolTable& _symbolTable;
            Result _result;
            std::string _wrapper_dir_path;

            Result createWrapperFile(const SymbolPackage& symbolObject, std::string& parent_include_path);
            Result createWrapperContent(const SymbolPackage& symbolObject, std::string& header_content, std::string& cpp_content);
            
            Result createPackageDefinition(const SymbolPackage& symbolObject, std::stringstream& ss);
            void createModuleDefinition(const SymbolModule& mod, std::stringstream& ss);
            void createClassDefinition(const SymbolClass& clazz, std::stringstream& ss);

            std::vector<std::string> createConstructorDefinition(const SymbolClass& clazz, const SymbolMethod& constructor);
            std::string callConstructor(const SymbolClass& clazz, const SymbolMethod& constructor);

            std::vector<std::string> createDestructorDefinition(const SymbolClass& clazz);
            std::string callDestructor(const SymbolClass& clazz);

            std::vector<std::string> createClassMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object);
            std::string callClassMethod(const SymbolMethod& object);

            std::vector<std::string> createStaticMethodDefinition(const SymbolMethod& object);
            std::string callStaticMethod(const SymbolMethod& object);

            std::string createParametersDefinition(const SymbolMethod& object);
            std::string createParameterDefinition(const SymbolParameter& object);

            std::vector<std::string> createReturnValueChanger(const SymbolMethod& object);
            std::vector<std::string> createInputParameterChanger(const SymbolParameter& object);
            std::vector<std::string> createOutputParameterChanger(const SymbolParameter& object);

            std::string createPropertyName(const SymbolProperty& object);
            std::string createPropertySetterDeclaration(const std::string& scope, const std::string& propertyName, const SymbolProperty& object);
            std::string createPropertyGetterDeclaration(const std::string& scope, const std::string& propertyName, const SymbolProperty& object);
            std::vector<std::string> callPropertySetter(const SymbolClass& clazz, const SymbolProperty& object);
            std::vector<std::string> callPropertyGetter(const SymbolClass& clazz, const SymbolProperty& object);
            std::vector<std::string> createPropertyDefinition(const SymbolClass& clazz, const SymbolProperty& object);

            std::vector<std::string> createInputPropertyChanger(const SymbolProperty& object);
            std::vector<std::string> createOutputPropertyChanger(const SymbolProperty& object);

            std::string createScope(const SymbolClass& clazz);
            std::string createScope(const SymbolMethod& method);

            std::vector<std::string> createChangerFunction();
        };
    }
}
#endif