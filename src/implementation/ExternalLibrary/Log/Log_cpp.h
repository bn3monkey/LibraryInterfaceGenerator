#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_LOG_CPP__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_LOG_CPP__
static constexpr char* LOG_CPP = 
"#include \"Log.hpp\"\n"
"\n"
"using namespace Bn3Monkey;\n"
"\n"
"class LogStream\n"
"{\n"
"public :\n"
"    LogStream() : start_pos(0), end_pos(0) {\n"
"    }\n"
"    void read(const char(&buffer)[Log::MAX_LINE])\n"
"    {\n"
"        {\n"
"            std::lock_guard<std::mutex> lock(stream_mtx);\n"
"            char(&target)[Log::MAX_LINE] = data[end_pos];\n"
"\n"
"            std::copy(buffer, buffer + Log::MAX_LINE, target);\n"
"\n"
"            end_pos = (end_pos + 1) % Log::MAX_STORABLE_COUNT;\n"
"            if (start_pos == end_pos)\n"
"            {\n"
"                start_pos = (start_pos + 1) % Log::MAX_STORABLE_COUNT;\n"
"            }\n"
"        }\n"
"    }\n"
"    int32_t write(char* buffer)\n"
"    {\n"
"        int32_t written_pos = 0;\n"
"        {\n"
"            std::lock_guard<std::mutex> lock(stream_mtx);\n"
"            for (size_t writing_pos = start_pos; writing_pos != end_pos; writing_pos = (writing_pos + 1) % Log::MAX_STORABLE_COUNT)\n"
"            {\n"
"                char(&target)[Log::MAX_LINE] = data[writing_pos];\n"
"                size_t log_size = strlen(target);\n"
"                std::copy(target, target + log_size, buffer + written_pos);\n"
"                written_pos += log_size;\n"
"            }\n"
"        }\n"
"        return written_pos;\n"
"    }\n"
"private:\n"
"    std::mutex stream_mtx;\n"
"    size_t start_pos;\n"
"    size_t end_pos;\n"
"    char data[Log::MAX_STORABLE_COUNT][Log::MAX_LINE] = { {0} };\n"
"};\n"
"static LogStream logStream;\n"
"\n"
"#if defined _WIN32 //WINDOWS\n"
"const int Bn3Monkey::Log::PRIO_VERBOSE = 0;\n"
"const int Bn3Monkey::Log::PRIO_DEBUG = 1;\n"
"const int Bn3Monkey::Log::PRIO_INFO = 2;\n"
"const int Bn3Monkey::Log::PRIO_WARN = 3;\n"
"const int Bn3Monkey::Log::PRIO_ERROR = 4;\n"
"#elif defined __ANDROID__ //ANDROID\n"
"const int Bn3Monkey::Log::PRIO_VERBOSE = ANDROID_LOG_VERBOSE;\n"
"const int Bn3Monkey::Log::PRIO_DEBUG = ANDROID_LOG_DEBUG;\n"
"const int Bn3Monkey::Log::PRIO_INFO = ANDROID_LOG_INFO;\n"
"const int Bn3Monkey::Log::PRIO_WARN = ANDROID_LOG_WARN;\n"
"const int Bn3Monkey::Log::PRIO_ERROR = ANDROID_LOG_ERROR;\n"
"#elif (defined __linux__) && !(defined __ANDROID__) // LINUX\n"
"const int Bn3Monkey::Log::PRIO_VERBOSE = 0;\n"
"const int Bn3Monkey::Log::PRIO_DEBUG = 1;\n"
"const int Bn3Monkey::Log::PRIO_INFO = 2;\n"
"const int Bn3Monkey::Log::PRIO_WARN = 3;\n"
"const int Bn3Monkey::Log::PRIO_ERROR = 4;\n"
"#endif\n"
"\n"
"std::mutex console_mtx;\n"
"\n"
"void Bn3Monkey::Log::print(int priority, const char* tag, const char* format, va_list args)\n"
"{\n"
"    char formatted[Log::SIZE_FORMATTED_CONTENT] = { 0 };\n"
"    size_t formatted_size =\n"
"#if defined _WIN32\n"
"        vsprintf_s(formatted, format, args);\n"
"#elif defined __ANDROID__\n"
"        vsprintf(formatted, format, args);\n"
"#elif (defined __linux__) && !(defined __ANDROID__)\n"
"        vsprintf(formatted, format, args);\n"
"#endif\n"
"\n"
"#if defined __ANDROID__\n"
"    __android_log_print(priority, tag, \"%s\", formatted);\n"
"#endif\n"
"    formatted[formatted_size] = '\\n';\n"
"    formatted[formatted_size + 1] = '\\0';\n"
"    formatted_size += 1;\n"
"\n"
"    char buffer[Log::MAX_LINE] = { 0 };\n"
"    size_t written_size = setDate(buffer);\n"
"    written_size += setPriority(buffer + written_size, priority);\n"
"    written_size += setTag(buffer + written_size, tag);\n"
"    std::copy(formatted, formatted + formatted_size, buffer + written_size);\n"
"    \n"
"\n"
"#if defined _WIN32\n"
"    OutputDebugStringA(buffer);\n"
"#endif\n"
"    {\n"
"        std::lock_guard<std::mutex> lock(console_mtx);\n"
"        printf(\"%s\", buffer);\n"
"    }\n"
"\n"
"    logStream.read(buffer);\n"
"}\n"
"\n"
"int32_t Bn3Monkey::Log::exportLog(char* data)\n"
"{\n"
"    int32_t ret = logStream.write(data);\n"
"    return ret;\n"
"}\n"
"\n"
"size_t Bn3Monkey::Log::setDate(char(&buffer)[Log::MAX_LINE])\n"
"{\n"
"    time_t now = time(0);\n"
"    struct tm tstruct;\n"
"#if defined _WIN32\n"
"    localtime_s(&tstruct, &now);\n"
"#else\n"
"    localtime_r(&now, &tstruct);\n"
"#endif\n"
"\n"
"    size_t written_size =\n"
"#if defined _WIN32\n"
"    snprintf(buffer, SIZE_FORMATTED_DATE + 1, \"%04d-%02d-%02d %02d:%02d:%02d \", tstruct.tm_year + 1900, tstruct.tm_mon, tstruct.tm_mday, tstruct.tm_hour, tstruct.tm_min, tstruct.tm_sec);\n"
"#else\n"
"    snprintf(buffer, SIZE_FORMATTED_DATE + 1, \"%d-%d-%d %02d:%02d:%02d \", tstruct.tm_year + 1900, tstruct.tm_mon, tstruct.tm_mday, tstruct.tm_hour, tstruct.tm_min, tstruct.tm_sec);\n"
"#endif\n"
"    return SIZE_FORMATTED_DATE;\n"
"}\n"
"\n"
"size_t Bn3Monkey::Log::setPriority(char* buffer, int priority)\n"
"{\n"
"    const char* priority_str = \"[?] | \";\n"
"    switch (priority)\n"
"    {\n"
"    case Log::PRIO_VERBOSE:\n"
"        priority_str = \"[V] | \";\n"
"        break;\n"
"    case Log::PRIO_DEBUG:\n"
"        priority_str = \"[D] | \";\n"
"        break;\n"
"    case Log::PRIO_INFO:\n"
"        priority_str = \"[I] | \";\n"
"        break;\n"
"    case Log::PRIO_WARN:\n"
"        priority_str = \"[W] | \";\n"
"        break;\n"
"    case Log::PRIO_ERROR:\n"
"        priority_str = \"[E] | \";\n"
"        break;\n"
"    }\n"
"    size_t size = sizeof(\"[?] | \");\n"
"    std::copy(priority_str, priority_str + size, buffer);\n"
"    return SIZE_FORMATTED_PRIORITY;\n"
"}\n"
"\n"
"size_t Bn3Monkey::Log::setTag(char* buffer, const char* tag)\n"
"{\n"
"    buffer[0] = '{';\n"
"    size_t tag_size = strlen(tag);\n"
"    std::copy(tag, tag + tag_size, buffer + 1);\n"
"    buffer[tag_size + 1] = '}';\n"
"    buffer[tag_size + 2] = ' ';\n"
"    buffer[tag_size + 3] = '\\0';\n"
"    return tag_size + 3;\n"
"}\n"
"\n";
#endif //__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_LOG_CPP__
