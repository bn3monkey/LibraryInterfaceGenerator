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

            // 1. Module Directory 제작
            Result createModule(const SymbolModule& object, std::string& parent_path);

            Result createInterfaceFile(const SymbolClass& object, std::string& parent_path);
            Result createClassFile(const SymbolClass& object, std::string& parent_path);
            Result createEnumFile(const SymbolEnum& object, std::string& parent_path);
            Result createMethodFile(const SymbolModule& object, std::string& parent_path);
            Result createCallbackFile(const SymbolCallback& object, std::string& parent_path);
            
            // 2. Interface 전문 제작
            SourceStream createInterfaceFileContent(const SymbolClass& object);

            // 3. Class 전문 제작
            SourceStream createClassFileContent(const SymbolClass& object);

            // 4. Enum 전문 제작
            SourceStream createEnumFileContent(const SymbolEnum& object);

            // 5. Method 전문 제작
            SourceStream createMethodFileContent(const SymbolModule& object);

            SourceStream createCallbackFileContent(const SymbolCallback& callback);


            // 7. Code Block 제작
            void createForwardDeclaration(SourceStream& ss, const SymbolClass& object);
            void createForwardDeclaration(SourceStream& ss, const SymbolModule& object);
            void createForwardDeclaration(SourceStream& ss, const SymbolCallback& object);

            // Enum Block 생성
            void createEnumDefinition(SourceStream& ss, const SymbolEnum& object);
            void createCallbackDefinition(SourceStream& ss, const SymbolCallback& callback);

            // Method Block 생성
            std::string createMethodName(const SymbolClass& clazz, const std::string& method_name, int number);
            std::string createMethodName(const SymbolMethod& method, const std::string& method_name, int number);

            void createPrimaryConstructorDefinition(SourceStream& ss, const SymbolClass& clazz);
            void createSecondaryConstructorDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& constructor);
            
            void createConstructorDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& constructor, int number);
            void createDestructorDefinition(SourceStream& ss, const SymbolClass& clazz);

            void createInterfaceMethodDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object);
            void createClassMethodDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object, int number, bool is_overrided);
            void createStaticMethodDefinition(SourceStream& ss, const SymbolMethod& object, int number);

            void callConstructor(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object, int number);
            void callDestructor(SourceStream& ss, const SymbolClass& clazz);
            void callAddReleaser(SourceStream& ss, const SymbolClass& clazz);
            void callClassMethod(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object, int number);
            void callStaticMethod(SourceStream& ss, const SymbolMethod& object, int number);

            std::string findConverter(SymbolType& type);
            void createReturnValueChanger(SourceStream& ss, const SymbolMethod& object);
            void createInputParameterChanger(SourceStream& ss, const SymbolParameter& object);

            std::string createPropertyName(const SymbolProperty& object);
            void createInterfacePropertyDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolProperty& object);
            void createClassPropertyDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolProperty& object, bool is_overrided);

            void createInputPropertyChanger(SourceStream& ss, const SymbolProperty& object);
            void createOutputPropertyChanger(SourceStream& ss, const SymbolProperty& object);

            void callPropertyGetter(SourceStream& ss, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object);
            void callPropertySetter(SourceStream& ss, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object);

        };
    }
}
#endif