#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_LOG_HPP__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_LOG_HPP__
static constexpr char* LOG_HPP = 
"#ifndef __LIBRARYINTERFACEGENERATOR_LOG__\n"
"#define __LIBRARYINTERFACEGENERATOR_LOG__\n"
"\n"
"#include <cstdarg>\n"
"#include <cstdio>\n"
"#include <string>\n"
"#include <mutex>\n"
"\n"
"#if defined _WIN32 //WINDOWS\n"
"#include <Windows.h>\n"
"#elif defined __ANDROID__ //ANDROID\n"
"#include <android/log.h>\n"
"#elif (defined __linux__) && !(defined __ANDROID__) // LINUX\n"
"\n"
"#endif\n"
"\n"
"class Log\n"
"{\n"
"public:\n"
"    static const int PRIO_VERBOSE;\n"
"    static const int PRIO_DEBUG;\n"
"    static const int PRIO_INFO;\n"
"    static const int PRIO_WARN;\n"
"    static const int PRIO_ERROR;\n"
"\n"
"    static constexpr const size_t MAX_LINE = 512;\n"
"    static constexpr const size_t MAX_STORABLE_COUNT = 8196;\n"
"    static constexpr const size_t SIZE_FORMATTED_DATE = sizeof(\"2014-04-21 13:00:00 \") - 1;\n"
"    static constexpr const size_t SIZE_FORMATTED_PRIORITY = sizeof(\"[?] | \") - 1;\n"
"    static constexpr const size_t SIZE_FORMATTED_HEADER = SIZE_FORMATTED_DATE + SIZE_FORMATTED_PRIORITY;\n"
"    static constexpr const size_t SIZE_FORMATTED_CONTENT = MAX_LINE - SIZE_FORMATTED_HEADER;\n"
"\n"
"    static inline void V(const char* tag, const char* format, ...) {\n"
"        va_list args;\n"
"        va_start(args, format);\n"
"        print(PRIO_VERBOSE, tag, format, args);\n"
"        va_end(args);\n"
"    }\n"
"    static inline void D(const char* tag, const char* format, ...) {\n"
"        va_list args;\n"
"        va_start(args, format);\n"
"        print(PRIO_DEBUG, tag, format, args);\n"
"        va_end(args);\n"
"    }\n"
"    static inline void E(const char* tag, const char* format, ...) {\n"
"        va_list args;\n"
"        va_start(args, format);\n"
"        print(PRIO_ERROR, tag, format, args);\n"
"        va_end(args);\n"
"    }\n"
"    static inline void W(const char* tag, const char* format, ...) {\n"
"        va_list args;\n"
"        va_start(args, format);\n"
"        print(PRIO_WARN, tag, format, args);\n"
"        va_end(args);\n"
"    }\n"
"    static inline void I(const char* tag, const char* format, ...) {\n"
"        va_list args;\n"
"        va_start(args, format);\n"
"        print(PRIO_INFO, tag, format, args);\n"
"        va_end(args);\n"
"    }\n"
"\n"
"    static int32_t exportLog(char* data);\n"
"\n"
"private:\n"
"    static void print(int priority, const char* tag, const char* format, va_list args);\n"
"    static size_t setDate(char (&buffer)[Log::MAX_LINE]);\n"
"    static size_t setPriority(char* buffer, int priority);\n"
"    static size_t setTag(char* buffer, const char* tag);\n"
"};\n"
"\n"
"#endif\n";
#endif //__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_LOG_HPP__
