#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SOURCESTREAM__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SOURCESTREAM__

#include <string>
#include <sstream>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class SourceStream;
        class SourceScopedStream;
        class UnindentedSourceScopedStream;

        enum class CodeStyle
        {
            Cpp,
            CppClass,
            Kotlin,
            Python,
            None
        };

        class SourceStream
        {
        public:
            static constexpr size_t MAX_INDENT = 256;
            static constexpr char indent = '\t';
            static constexpr size_t MAX_STREAM_SIZE = 1024 * 1024 * 10; // 10Mbyte

            explicit SourceStream();
            virtual ~SourceStream();

            SourceStream(SourceStream&& other);

            friend LibraryInterfaceGenerator::Implementation::SourceStream& operator<<(LibraryInterfaceGenerator::Implementation::SourceStream& stream, const char* str);
            friend LibraryInterfaceGenerator::Implementation::SourceStream& operator<<(LibraryInterfaceGenerator::Implementation::SourceStream& stream, const std::string& str);
            
            void pop(size_t end_string_size = 1);
            
            void addIndent();
            void removeIndent();

            char*& str();

            operator bool() { return _content != nullptr; }

            void clear();
        private:
            size_t _indent_size {0};
            char _current_indent[MAX_INDENT] {0, };

            size_t _size{ 0 };
            char* _content = nullptr;

            bool _is_new_line_started{ false };
        };

        class SourceScopedStream
        {
        public:
            SourceScopedStream() = delete;
            explicit SourceScopedStream(SourceStream& ss, CodeStyle style = CodeStyle::Cpp);
            explicit SourceScopedStream(SourceScopedStream& ss, CodeStyle style = CodeStyle::Cpp);
            explicit SourceScopedStream(UnindentedSourceScopedStream& ss, CodeStyle style = CodeStyle::Cpp);
            virtual ~SourceScopedStream();

            friend LibraryInterfaceGenerator::Implementation::SourceScopedStream& operator<<(LibraryInterfaceGenerator::Implementation::SourceScopedStream& stream, const char* str);
            friend LibraryInterfaceGenerator::Implementation::SourceScopedStream& operator<<(LibraryInterfaceGenerator::Implementation::SourceScopedStream& stream, const std::string& str);


            inline SourceStream& stream() { return _stream; }

            void pop(size_t end_string_size = 1);

        private:
            SourceStream& _stream;
            const char* _scopeStart;
            const char* _scopeEnd;
        };

        class UnindentedSourceScopedStream
        {
        public:
            UnindentedSourceScopedStream() = delete;
            explicit UnindentedSourceScopedStream(SourceStream& ss, CodeStyle style = CodeStyle::Cpp);
            explicit UnindentedSourceScopedStream(SourceScopedStream& ss, CodeStyle style = CodeStyle::Cpp);
            explicit UnindentedSourceScopedStream(UnindentedSourceScopedStream& ss, CodeStyle style = CodeStyle::Cpp);
            virtual ~UnindentedSourceScopedStream();

            friend LibraryInterfaceGenerator::Implementation::UnindentedSourceScopedStream& operator<<(LibraryInterfaceGenerator::Implementation::UnindentedSourceScopedStream& stream, const char* str);
            friend LibraryInterfaceGenerator::Implementation::UnindentedSourceScopedStream& operator<<(LibraryInterfaceGenerator::Implementation::UnindentedSourceScopedStream& stream, const std::string& str);

            void pop(size_t end_string_size = 1);

            inline SourceStream& stream() { return _stream; }

        private:
            SourceStream& _stream;
        };

        struct ParameterNode
        {
            const static int REFERENCE_IN = 0;
            const static int VALUE = 1;

            int io;
            std::string type;
            std::string name;

            ParameterNode(int io, const std::string& type, const std::string& name) :
                io(io),
                type(type),
                name(name)
            {
            }
        };
    }
}

LibraryInterfaceGenerator::Implementation::SourceStream& LibraryInterfaceGenerator::Implementation::operator<<(LibraryInterfaceGenerator::Implementation::SourceStream& stream, const char* str);
LibraryInterfaceGenerator::Implementation::SourceStream& LibraryInterfaceGenerator::Implementation::operator<<(LibraryInterfaceGenerator::Implementation::SourceStream& stream, const std::string& str);
LibraryInterfaceGenerator::Implementation::SourceScopedStream& LibraryInterfaceGenerator::Implementation::operator<<(LibraryInterfaceGenerator::Implementation::SourceScopedStream& stream, const char* str);
LibraryInterfaceGenerator::Implementation::SourceScopedStream& LibraryInterfaceGenerator::Implementation::operator<<(LibraryInterfaceGenerator::Implementation::SourceScopedStream& stream, const std::string& str);
LibraryInterfaceGenerator::Implementation::UnindentedSourceScopedStream& LibraryInterfaceGenerator::Implementation::operator<<(LibraryInterfaceGenerator::Implementation::UnindentedSourceScopedStream& stream, const char* str);
LibraryInterfaceGenerator::Implementation::UnindentedSourceScopedStream& LibraryInterfaceGenerator::Implementation::operator<<(LibraryInterfaceGenerator::Implementation::UnindentedSourceScopedStream& stream, const std::string& str);

#endif // __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SOURCESTREAM__