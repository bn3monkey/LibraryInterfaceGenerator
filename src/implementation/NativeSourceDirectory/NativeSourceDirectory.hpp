#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCEDIRECTORY__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCEDIRECTORY__

#include <string>

#include "../FileSystem/FileSystem.hpp"
#include "../Result/Result.hpp"
#include "../SymbolTable/SymbolTable.hpp"
#include "../Auxiliary/Definition.hpp"
#include "../Auxiliary/StringHelper.hpp"
#include <algorithm>


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
            Result createMethodFile(const SymbolMethod& object, std::string& parent_include_path, std::string& parent_src_path);
            Result createIncludeFile(const SymbolPackage& package, std::string& parent_include_path);

            // 2. Interface 전문 제작
            Result createInterfaceFileContent(const SymbolClass& object, std::string& header_content);

            // 3. Class 전문 제작
            Result createClassFileContent(const SymbolClass& object, std::string& header_content, std::string& cpp_content);
            
            // 4. Enum 전문 제작
            Result createEnumFileContent(const SymbolEnum& object, std::string& header_content);
            
            // 5. Method 전문 제작
            Result createMethodFileContent(const SymbolMethod& object, std::string& header_content, std::string& cpp_content);

            // 6. 전체 파일의 include file
            Result createIncludeFileContent(const SymbolPackage& package, std::string& parent_include_path, std::string& header_content);

            // 7. Code Block 제작
            /*
            // Enum Block 생성
            Result createEnumBlock(const SymbolEnum& object, std::vector<std::string>& header_content);            
            // Method Block 생성
            Result createInterfaceMethodBlock(const SymbolMethod& object, std::vector<std::string>& header_content);
            Result createClassMethodBlock(const SymbolMethod& object, std::vector<std::string>& header_content, std::vector<std::string>& cpp_content);
            Result createDerivedMethodBlock(const SymbolMethod& object, std::vector<std::string>& header_content, std::vector<std::string>& cpp_content);
            // Parameter Block 생성
            Result createParameterBlock(const SymbolParameter& object, std::string& content);
            // Property Block 생성
            Result createInterfacePropertyBlock(const SymbolProperty& object, std::vector<std::string>& header_content);
            Result createClassPropertyBlock(const SymbolProperty& object, std::vector<std::string>& header_content, std::vector<std::string>& cpp_content);
            Result createDerivedPropertyBlock(const SymbolProperty& object, std::vector<std::string>& header_content, std::vector<std::string>& cpp_content);
            Result addPropertyDataBlock(const SymbolProperty& object, std::vector<std::string>& properties);
            */
        };
    }
}
#endif