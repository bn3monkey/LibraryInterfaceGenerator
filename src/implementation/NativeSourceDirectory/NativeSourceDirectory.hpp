#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCEDIRECTORY__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCEDIRECTORY__

#include "../Auxiliary/IError.hpp"
#include "../../json/json.hpp"

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class NativeSourceDirectory
        {
        public:
            explicit NativeSourceDirectory(std::string json_content);

        private:
            nlohmann::json _object;

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
            Error createClassTable(const nlohmann::json& _object);

            // 2. Module Directory 제작
            Error createModule(const nlohmann::json& _object);

            // 3. Interface 제작
            Error createInterface(const nlohmann::json& object, std::string& content);

            // 4. Class 제작
            Error createClass(const nlohmann::json& object, std::string& content);
            
            // 5. Enum 제작
            Error createEnum(const nlohmann::json& object, std::string& content);

            // 5. Method 제작
            Error createMethod(const nlohmann::json& object, std::string& content);

            // 6. Parameter 제작
            Error createParameter(const nlohmann::json& object, std::string& content);

            // 7. Property 제작
            Error createProperty(const nlohmann::json& object, std::string& content);
        };
    }
}
#endif