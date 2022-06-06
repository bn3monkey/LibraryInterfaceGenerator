#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCEDIRECTORY__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCEDIRECTORY__

#include <string>

#include "../FileSystem/FileSystem.hpp"
#include "../Result/Result.hpp"
#include "../SymbolTable/SymbolTable.hpp"
#include "../Auxiliary/Definition.hpp"


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
            nlohmann::json _object;
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
            
            // 1. Class Table 제작 (Class, Interface, enum class)
            /*
            Result createClassTable(const nlohmann::json& object);

            // 2. Module Directory 제작
            Result createModule(const nlohmann::json& object, std::string& parent_dir_path);

            Result createInterfaceFile(const nlohmann::json& object, std::string& parent_dir_path);
            Result createClassFile(const nlohmann::json& object, std::string& parent_dir_path);
            Result createEnumFile(const nlohmann::json& object, std::string& parent_dir_path);
            Result createMethodFile(const nlohmann::json& object, std::string& parent_dir_path);

            // 3. Interface 제작
            Result createInterface(const nlohmann::json& object, std::string& header_content, std::string& cpp_content);

            // 4. Class 제작
            Result createClass(const nlohmann::json& object, std::string& header_content, std::string& cpp_content);
            
            // 5. Enum 제작
            Result createGlobalEnum(const nlohmann::json& object, std::string& header_content);
            Result createEnum(const nlohmann::json& object, std::string& header_content);

            // 5. Method 제작
            Result createGlobalMethod(const nlohmann::json& object, std::string& header_content, std::string& cpp_content);
            Result createMethod(const nlohmann::json& object, std::string& header_content, std::string& cpp_content);

            // 6. Parameter 제작
            Result createParameter(const nlohmann::json& object, std::string& header_content, std::string& cpp_content);

            // 7. Property 제작
            Result createInterfaceProperty(const nlohmann::json& object, std::string& header_content, std::string& cpp_content);
            */
            
        };
    }
}
#endif