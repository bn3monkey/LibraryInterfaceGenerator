#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCESTREAM__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCESTREAM__

#include "SourceStream.hpp"
#include <vector>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class HeaderGuardNativeSourceStream
        {
        public:
            explicit HeaderGuardNativeSourceStream(SourceStream& sourceStream, const std::vector<std::string>& module_names, const std::string& name);
            virtual ~HeaderGuardNativeSourceStream();
        };

        class InternalIncludeNativeSourceStream
        {
        public:
            explicit InternalIncludeNativeSourceStream(SourceStream& sourceStream, const std::string& header_path);
            virtual ~InternalIncludeNativeSourceStream();
        };

        class ExternalIncludeNativeSourceStream
        {
        public:
            explicit ExternalIncludeNativeSourceStream(SourceStream& sourceStream, const std::string& header_path);
            virtual ~ExternalIncludeNativeSourceStream();
        };

        class NamespaceNativeSourceStream
        {
        public:
            explicit NamespaceNativeSourceStream(SourceStream& sourceStream, const std::string& namespace_names);
            explicit NamespaceNativeSourceStream(SourceStream& sourceStream, const std::string& namespace_name);
            virtual ~NamespaceNativeSourceStream();
        
        private:
            std::vector<NamespaceNativeSourceStream> _namespaces;
        };

        class ClassNativeSourceStream
        {
        public:
            explicit ClassNativeSourceStream(SourceStream& sourceStream, const std::vector<std::string>& base_names, const std::string& name);
            virtual ~ClassNativeSourceStream();
        };

        class AccessNativeSourceStream
        {
        public:
            enum class Specifier
            {
                Public,
                Private,
                Protected,
            };
            explicit AccessNativeSourceStream(SourceStream& sourceStream, const Specifier& specifier = Specifier::Public);
            virtual ~AccessNativeSourceStream();
        };

    }
}

#endif
