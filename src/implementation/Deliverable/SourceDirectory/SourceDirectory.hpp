#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SOURCEDIRECTORY__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SOURCEDIRECTORY__

#include <string>

#include "../../FileSystem/FileSystem.hpp"
#include "../../Result/Result.hpp"
#include "../../SymbolTable/SymbolTable.hpp"
#include "../../Auxiliary/Definition.hpp"
#include "../../Auxiliary/StringHelper.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../../Environment.hpp"

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
            Result createModule(const SymbolModule& object, std::string& parent_path);

            Result createInterfaceFile(const SymbolClass& object, std::string& parent_path);
            Result createClassFile(const SymbolClass& object, std::string& parent_path);
            Result createEnumFile(const SymbolEnum& object, std::string& parent_path);
            Result createMethodFile(const SymbolModule& object, std::string& parent_path);
            
            // 2. Interface 傈巩 力累
            SourceStream createInterfaceFileContent(const SymbolClass& object);

            // 3. Class 傈巩 力累
            SourceStream createClassFileContent(const SymbolClass& object);

            // 4. Enum 傈巩 力累
            SourceStream createEnumFileContent(const SymbolEnum& object);

            // 5. Method 傈巩 力累
            SourceStream createMethodFileContent(const SymbolModule& object);

            // 7. Code Block 力累
            void createForwardDeclaration(SourceStream& ss, const SymbolClass& object);
            void createForwardDeclaration(SourceStream& ss, const SymbolModule& object);

            // Enum Block 积己
            void createEnumDefinition(SourceStream& ss, const SymbolEnum& object);

            // Method Block 积己
            void createNativeHandle(SourceStream& ss);
            void createConstructorDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& constructor, int number);
            void createDestructorDefinition(SourceStream& ss, const SymbolClass& clazz);

            void createInterfaceMethodDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object);
            void createClassMethodDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object, int number);
            void createDerivedMethodDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object, int number);
            void createStaticMethodDefinition(SourceStream& ss, const SymbolMethod& object, int number);

            void callConstructor(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object, int number);
            void callDestructor(SourceStream& ss, const SymbolClass& clazz);
            void callAddReleaser(SourceStream& ss, const SymbolClass& clazz);
            void callClassMethod(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object, int number);
            void callStaticMethod(SourceStream& ss, const SymbolMethod& object, int number);

            void createReturnValueChanger(SourceStream& ss, const SymbolMethod& object);
            void createInputParameterChanger(SourceStream& ss, const SymbolParameter& object);
            void createOutputParameterChanger(SourceStream& ss, const SymbolParameter& object);

            void createInputPropertyChanger(SourceStream& ss, const SymbolProperty& object);
            void createOutputPropertyChanger(SourceStream& ss, const SymbolProperty& object);

            std::string createPropertyName(const SymbolProperty& object);
            void callPropertyGetter(SourceStream& ss, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object);
            void callPropertySetter(SourceStream& ss, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object);

            void createInterfacePropertyDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolProperty& object);
            void createClassPropertyDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolProperty& object);
            void createDerivedPropertyDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolProperty& object);

            /*
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
            
            // Parameter Block 积己
            std::string createParametersDefinition(const SymbolMethod& object);
            std::string createParameterDefinition(const SymbolParameter& object);

            // Property Block 积己

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
            */
        };
    }
}
#endif