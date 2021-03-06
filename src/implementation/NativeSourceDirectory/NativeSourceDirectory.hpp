#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCEDIRECTORY__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCEDIRECTORY__

#include <string>

#include "../FileSystem/FileSystem.hpp"
#include "../Result/Result.hpp"
#include "../SymbolTable/SymbolTable.hpp"
#include "../Auxiliary/Definition.hpp"
#include "../Auxiliary/StringHelper.hpp"
#include "../NativeExternalLibraryDirectory/NativeExternalLibraryDirectory.hpp"
#include "../NativeSourceStream/NativeSourceStream.hpp"
#include <algorithm>
#include <set>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class NativeSourceDirectory
        {
        public:
            static constexpr char* DEFAULT_INCLUDE_DIR_NAME = "include";
            static constexpr char* DEFAULT_SRC_DIR_NAME = "src";

            explicit NativeSourceDirectory(
                const NativeExternalLibraryDirectory& libDirectory, 
                const SymbolTable& symbolTable, 
                std::string root_dir_path = ".",
                const char* include_dir_name = DEFAULT_INCLUDE_DIR_NAME,
                const char* src_dir_name = DEFAULT_SRC_DIR_NAME);
            inline operator bool() { return !_result; }
            Result toError() {
                return _result;
            }

            Result make();
            inline std::string getIncludeDirPath() const {return _include_dir_path;}
            inline std::string getIncludeDirName() const {return _include_dir_name;}
            inline std::string getSourceDirPath() const {return _src_dir_path;}
            inline std::string getSourceDirName() const {return _src_dir_name;}

        private:
            const NativeExternalLibraryDirectory& _libDirectory;
            const SymbolTable& _symbolTable;
            Result _result;

            std::string _include_dir_name;
            std::string _include_dir_path;
            std::string _src_dir_name;
            std::string _src_dir_path; 
            
            // lib/ external library or source code
            //   libraryA/
            //   libraryB/
            //   log/
            //     log.hpp
            //     log.cpp
            //   memorypool/
            //     memorypool.hpp
            //     memorypool.cpp
            // include/
            //     LibraryName.hpp
            //  module/
            //     submodule/
            //        A.hpp
            //        B.hpp
            //  module/
            //     submodule/
            //        C.hpp
            //        D.hpp
            // src/
            //   module/
            //       submodule/
            //         A.cpp
            //         B.cpp
            //   module/
            //      submodule/
            //         C.cpp
            //         D.cpp/
            // ?????????, ???????????????, ?????? ???????????? ??????????????? ??????, ?????? ????????? ????????? ?????? ????????? ??????.
            
            // 1. Module Directory ??????
            Result createModule(const SymbolModule& object, std::string& parent_include_path, std::string& parent_src_path);

            Result createInterfaceFile(const SymbolClass& object, std::string& parent_include_path);
            Result createClassFile(const SymbolClass& object, std::string& parent_include_path, std::string& parent_src_path);
            Result createEnumFile(const SymbolEnum& object, std::string& parent_include_path);
            Result createMethodFile(const SymbolModule& object, std::string& parent_include_path, std::string& parent_src_path);
            Result createIncludeFile(const SymbolPackage& package, std::string& parent_include_path);

            // 2. Interface ?????? ??????
            Result createInterfaceFileContent(const SymbolClass& object, std::string& header_content);

            // 3. Class ?????? ??????
            Result createClassFileContent(const SymbolClass& object, std::string& header_content, std::string& cpp_content);
            
            // 4. Enum ?????? ??????
            Result createEnumFileContent(const SymbolEnum& object, std::string& header_content);
            
            // 5. Method ?????? ??????
            Result createMethodFileContent(const SymbolModule& object, std::string& header_content, std::string& cpp_content);

            // 6. ?????? ????????? include file
            Result createIncludeFileContent(const SymbolPackage& package, std::string& parent_include_path, std::string& header_content);

            void addForwardDeclaration(std::stringstream& ss, std::string& indent, const SymbolClass& object);
            void addForwardDeclaration(std::stringstream& ss, std::string& indent, const SymbolModule& object);


            // 7. Code Block ??????
            
            // Enum Block ??????
            std::vector<std::string> createEnumDefinition(const SymbolEnum& object);

            // Method Block ??????
            std::string createMethodDeclaration(const SymbolMethod& object);

            std::string createStaticMethodDeclaration(const SymbolMethod& object);
            std::string createClassMethodDeclaration(const SymbolClass& clazz, const SymbolMethod& object);
            std::string createInterfaceMethodDeclaration(const SymbolClass& clazz, const SymbolMethod& object);
            std::string createDerivedMethodDeclaration(const SymbolMethod& object);

            std::string createConstructorDeclaration(const SymbolClass& clazz, const SymbolMethod& object);
            std::vector<std::string> createConstructorDefinition(const SymbolClass& clazz, const SymbolMethod& object);

            std::string createDestructorDeclaration(const SymbolClass& clazz);
            std::vector<std::string> createDestructorDefinition(const SymbolClass& clazz);
            
            std::vector<std::string> createStaticMethodDefinition(const SymbolMethod& object);
            std::vector<std::string> createClassMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object);

            // Parameter Block ??????
            std::string createParametersDefinition(const SymbolMethod& parameters);
            std::string createParameterDefinition(const SymbolParameter& object);
            
            // Property Block ??????
            std::string createPropertyName(const SymbolProperty& object);
            std::string createPropertySetterDeclaration(const std::string& propertyName, const SymbolProperty& object);
            std::string createPropertyGetterDeclaration(const std::string& propertyName, const SymbolProperty& object);

            std::vector<std::string> createClassPropertyDeclaration(const SymbolProperty& object);
            std::vector<std::string> createInterfacePropertyDeclaration(const SymbolProperty& object);
            std::vector<std::string> createDerivedPropertyDeclaration(const SymbolProperty& object);

            std::vector<std::string> createPropertyDefinition(const SymbolClass& clazz, const SymbolProperty& object);
            
            std::string createPropertyField(const SymbolProperty& object);

            // Property, Method??? ??????
            std::string createScope(const SymbolMethod& method);
            std::string createScope(const SymbolClass& clazz);


            // ?????? ???????????? ????????? ?????? ?????? ??????
            //std::set<std::string> collectDeclarations(const SymbolClass& clazz);
            //std::set<std::string> collectDeclarations(const SymbolMethod& method);
            
        };
    }
}
#endif