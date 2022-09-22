#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCESTREAM__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCESTREAM__

#include "SourceStream.hpp"
#include <vector>
#include <sstream>
#include <string>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        

        class HeaderGuardNativeSourceScopedStream
        {
        public:
            explicit HeaderGuardNativeSourceScopedStream(SourceStream& sourceStream, const std::vector<std::string>& module_names, const std::string& name);
            virtual ~HeaderGuardNativeSourceScopedStream();

        private:
            SourceScopedStream& stream;
            std::string makeGUID(const std::vector<std::string>& strs);
        };

        class InternalIncludeNativeSourceStream
        {
        public:
            explicit InternalIncludeNativeSourceStream(SourceStream& sourceStream, const std::string& header_path);
            virtual ~InternalIncludeNativeSourceStream();
            
        private:
            SourceStream& stream;
        };

        class ExternalIncludeNativeSourceStream
        {
        public:
            explicit ExternalIncludeNativeSourceStream(SourceStream& sourceStream, const std::string& header_path);
            virtual ~ExternalIncludeNativeSourceStream();

            
        private:
            SourceStream& stream;
        };

        class NamespaceNativeSourceScopedStream
        {
        public:
            explicit NamespaceNativeSourceScopedStream(SourceStream& sourceStream, const std::string& namespace_names);
            explicit NamespaceNativeSourceScopedStream(SourceStream& sourceStream, const std::string& namespace_name);
            virtual ~NamespaceNativeSourceScopedStream();
        
        private:
            std::vector<NamespaceNativeSourceScopedStream> _namespaces;
            SourceScopedStream& stream;
        };

        class ClassNativeSourceScopedStream
        {
        public:
            explicit ClassNativeSourceScopedStream(SourceStream& sourceStream, const std::vector<std::string>& base_names, const std::string& name);
            virtual ~ClassNativeSourceScopedStream();
            
        private:
            SourceScopedStream& stream;
        };

        class AccessNativeSourceScopedStream
        {
        public:
            enum class Specifier
            {
                Public,
                Private,
                Protected,
            };
            explicit AccessNativeSourceScopedStream(SourceStream& sourceStream, const Specifier& specifier = Specifier::Public);
            virtual ~AccessNativeSourceScopedStream();
            
        private:
            UnindentedSourceScopedStream& stream;
        };

        class EnumNativeSourceScopedStream
        {
        public:
            explicit EnumNativeSourceScopedStream(SourceStream& sourceStream, const std::string& name);
            virtual ~EnumNativeSourceScopedStream();

            void addElement(std::string& key, std::string& value);
        private:
            SourceScopedStream& stream;
        };

        
        class MethodNativeSourceStream
        {
        public:
            explicit MethodNativeSourceStream(SourceStream& sourceStream, const std::string& prefix, const std::string& type, const std::string& name);
            virtual ~MethodNativeSourceStream();

            void addParameter(bool isInputParameter, const std::string& type, const std::string& name);
            
        private:
            SourceStream& stream;
        };


        class CommentNativeSourceStream
        {
        public:
            explicit CommentNativeSourceStream(SourceStream& sourceStream);
            virtual ~CommentNativeSourceStream();

            void addName(const std::string& name);
            void addDescription(const std::string& description);
            void addBrief(const std::string& brief);
            void addReturn(const std::string& return_value);
            void addParameter(bool isInputParameter, const std::string& name, const std::string& description);


        private:
            SourceStream& stream;
            std::vector<std::string> tokenize(const std::string& description);
        }
    }
}

#endif
