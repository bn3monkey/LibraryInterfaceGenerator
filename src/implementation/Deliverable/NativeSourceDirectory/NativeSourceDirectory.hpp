#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCEDIRECTORY__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCEDIRECTORY__

#include <string>

#include "../../FileSystem/FileSystem.hpp"
#include "../../Result/Result.hpp"
#include "../../Auxiliary/Definition.hpp"
#include "../../Auxiliary/StringHelper.hpp"

#include "../../SymbolTable/SymbolTable.hpp"
#include "../../SourceStream/CXXSourceStream.hpp"
#include "../../Converter/CXXConverter.hpp"
#include "../../ExternalLibrary/NativeExternalLibraryDirectory.hpp"

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
            // 클래스, 인터페이스, 열거 클래스가 파라미터일 경우, 헤더 파일의 상대적 위치 분석이 필요.
            
            // 1. Module Directory 제작
            Result createModule(const SymbolModule& object, std::string& parent_include_path, std::string& parent_src_path);

            Result createInterfaceFile(const SymbolClass& object, std::string& parent_include_path);
            Result createClassFile(const SymbolClass& object, std::string& parent_include_path, std::string& parent_src_path);
            Result createEnumFile(const SymbolEnum& object, std::string& parent_include_path);
            Result createMethodFile(const SymbolModule& object, std::string& parent_include_path, std::string& parent_src_path);
            Result createIncludeFile(const SymbolPackage& package, std::string& parent_include_path);

            // 2. Interface 전문 제작
            SourceStream createInterfaceFileContent(const SymbolClass& object);

            // 3. Class 전문 제작
            SourceStream createClassHeaderFileContent(const SymbolClass& object);
            SourceStream createClassCppFileContent(const SymbolClass& object);
            
            // 4. Enum 전문 제작
            SourceStream createEnumFileContent(const SymbolEnum& object);
            
            // 5. Method 전문 제작
            SourceStream createMethodHeaderFileContent(const SymbolModule& object);
            SourceStream createMethodCppFileContent(const SymbolModule& object);

            // 6. 전체 파일의 include file
            SourceStream createIncludeFileContent(const SymbolPackage& package, const std::vector<std::string>& include_files);

            // void addForwardDeclaration(std::stringstream& ss, std::string& indent, const SymbolClass& object);
            // void addForwardDeclaration(std::stringstream& ss, std::string& indent, const SymbolModule& object);

            void createForwardDeclaration(SourceStream& ss, const SymbolClass& object);
            void createForwardDeclaration(SourceStream& ss, const SymbolModule& object);


            // 7. Code Block 제작
            
            // Enum Block 생성
            void createEnumDefinition(SourceStream& ss, const SymbolEnum& object);

            // Method Block 생성
            void createStaticMethodDeclaration(SourceStream& ss, const SymbolMethod& object);
            void createClassMethodDeclaration(SourceStream& ss, const SymbolMethod& object);
            void createInterfaceMethodDeclaration(SourceStream& ss, const SymbolMethod& object);
            void createDerivedMethodDeclaration(SourceStream& ss, const SymbolMethod& object);

            void createStaticMethodDefinition(SourceStream& ss, const SymbolMethod& object);
            void createClassMethodDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object);

            void createConstructorDeclaration(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object);
            void createConstructorDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object);

            void createDestructorDeclaration(SourceStream& ss, const SymbolClass& clazz);
            void createDestructorDefinition(SourceStream& ss, const SymbolClass& clazz);
                        
            // Property Block 생성
            std::string createPropertyName(const SymbolProperty& object);
            void createClassPropertySetterDeclaration(SourceStream& ss, const std::string& propertyName, const SymbolProperty& object);
            void createClassPropertyGetterDeclaration(SourceStream& ss, const std::string& propertyName, const SymbolProperty& object);
            void createClassPropertyDeclaration(SourceStream& ss, const SymbolProperty& object);

            void createInterfacePropertySetterDeclaration(SourceStream& ss, const std::string& propertyName, const SymbolProperty& object);
            void createInterfacePropertyGetterDeclaration(SourceStream& ss, const std::string& propertyName, const SymbolProperty& object);
            void createInterfacePropertyDeclaration(SourceStream& ss, const SymbolProperty& object);
            
            void createDerivedPropertySetterDeclaration(SourceStream& ss, const std::string& propertyName, const SymbolProperty& object);
            void createDerivedPropertyGetterDeclaration(SourceStream& ss, const std::string& propertyName, const SymbolProperty& object);
            void createDerivedPropertyDeclaration(SourceStream& ss, const SymbolProperty& object);


            void createPropertyGetterDefinition(SourceStream& ss, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object);
            void createPropertySetterDefinition(SourceStream& ss, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object);
            void createPropertyDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolProperty& object);
            
            void createPropertyField(SourceStream& ss, const SymbolProperty& object);


            // 특정 헤더에서 상대적 헤더 위치 찾기
            //std::set<std::string> collectDeclarations(const SymbolClass& clazz);
            //std::set<std::string> collectDeclarations(const SymbolMethod& method);
        };
    }

}
#endif