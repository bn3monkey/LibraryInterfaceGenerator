#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR__

#include <string>
#include <cstdint>

namespace LibraryInterfaceGenerator
{
    class Error
    {
    public:
        enum class Code : int32_t
        {
            SUCCESS = 0,
        };

        inline operator bool() const { return _code != Code::SUCCESS; } 
        std::string toString() const {return _reason;}
        
        explicit Error(const Code code) : _code(code), _reason("") {}
        explicit Error(const Code code, const std::string& reason) : _code(code), _reason(reason) {}

    private:
        Code _code;
        std::string _reason;
    };

    enum class Framework
    {
        Kotlin_Android,
    };

    Error createNativeSourceDirectory(const std::string& json_content);
    Error createAPIDocumentation(const std::string& json_content);
    Error createNativeInterface(const std::string& json_content);
    Error createWrapper(const std::string& json_content, Framework env);
    Error createSourceDirectory(const std::string& json_content, Framework env);
}

#endif