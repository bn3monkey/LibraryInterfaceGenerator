#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCEDIRECTORY__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCEDIRECTORY__

#include <string>

#include "../FileSystem/FileSystem.hpp"
#include "../Result/Result.hpp"
#include "../SymbolTable/SymbolTable.hpp"
#include "../Auxiliary/Definition.hpp"
#include "../Auxiliary/StringHelper.hpp"
#include "NativeSourceStream.hpp"
#include <algorithm>
#include <set>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class NativeSourceDirectory
        {
        public:
            explicit NativeSourceDirectory(const SymbolTable& symbolTable, std::string root_dir_path = ".");
            inline operator bool() { return !_result; }
            Result toError() {
                return _result;
            }

            Result make();

        private:
            const SymbolTable& _symbolTable;
            Result _result;
            std::string _include_dir_path;
            std::string _src_dir_path; 

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
            Result createInterfaceFileContent(const SymbolClass& object, std::string& header_content);

            // 3. Class 전문 제작
            Result createClassFileContent(const SymbolClass& object, std::string& header_content, std::string& cpp_content);
            
            // 4. Enum 전문 제작
            Result createEnumFileContent(const SymbolEnum& object, std::string& header_content);
            
            // 5. Method 전문 제작
            Result createMethodFileContent(const SymbolModule& object, std::string& header_content, std::string& cpp_content);

            // 6. 전체 파일의 include file
            Result createIncludeFileContent(const SymbolPackage& package, std::string& parent_include_path, std::string& header_content);

            // 7. Code Block 제작
            
            // Enum Block 생성
            std::vector<std::string> createEnumDefinition(const SymbolEnum& object);

            // Method Block 생성
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

            // Parameter Block 생성
            std::string createParametersDefinition(const SymbolMethod& parameters);
            std::string createParameterDefinition(const SymbolParameter& object);
            
            // Property Block 생성
            std::string createPropertyName(const SymbolProperty& object);
            std::string createPropertySetterDeclaration(const std::string& propertyName, const SymbolProperty& object);
            std::string createPropertyGetterDeclaration(const std::string& propertyName, const SymbolProperty& object);

            std::vector<std::string> createClassPropertyDeclaration(const SymbolProperty& object);
            std::vector<std::string> createInterfacePropertyDeclaration(const SymbolProperty& object);
            std::vector<std::string> createDerivedPropertyDeclaration(const SymbolProperty& object);

            std::vector<std::string> createPropertyDefinition(const SymbolClass& clazz, const SymbolProperty& object);
            
            std::string createPropertyField(const SymbolProperty& object);

            // Property, Method의 범위
            std::string createScope(const SymbolMethod& method);
            std::string createScope(const SymbolClass& clazz);

            // 특정 헤더에서 상대적 헤더 위치 찾기
            //std::set<std::string> collectDeclarations(const SymbolClass& clazz);
            //std::set<std::string> collectDeclarations(const SymbolMethod& method);
            
        };
    }
}
#endif