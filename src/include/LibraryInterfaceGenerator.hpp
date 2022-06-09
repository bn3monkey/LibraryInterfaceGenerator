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
            FAIL = 1,
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

    Error createRootDirectory(const std::string& root_dir_path);
    Error createNativeSourceDirectory(const std::string& json_content, const std::string& root_dir_path);
    Error createAPIDocumentation(const std::string& json_content, const std::string& root_dir_path);
    Error createNativeInterface(const std::string& json_content, const std::string& root_dir_path);
    Error createWrapper(const std::string& json_content, Framework env, const std::string& root_dir_path);
    Error createSourceDirectory(const std::string& json_content, Framework env, const std::string& root_dir_path);
}

#endif