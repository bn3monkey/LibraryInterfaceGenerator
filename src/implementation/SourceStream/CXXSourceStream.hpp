#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_CXXSOURCESTREAM__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_CXXSOURCESTREAM__

#include "SourceStream.hpp"
#include <vector>
#include <sstream>
#include <string>
#include <queue>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class HeaderGuardCXXSourceScopedStream
        {
        public:
            explicit HeaderGuardCXXSourceScopedStream(SourceStream& sourceStream, const std::vector<std::string>& module_names, const std::string& name);
            virtual ~HeaderGuardCXXSourceScopedStream();

        private:
            std::string makeGUID(const std::vector<std::string>& strs);
            
            SourceStream& _stream;
            std::string _macro;
        };

        
        class InternalIncludeCXXSourceStream
        {
        public:
            explicit InternalIncludeCXXSourceStream(SourceStream& sourceStream, const std::string& header_path);
            virtual ~InternalIncludeCXXSourceStream();
            
        private:
            SourceStream& _stream;
        };

        class ExternalIncludeCXXSourceStream
        {
        public:
            explicit ExternalIncludeCXXSourceStream(SourceStream& sourceStream, const std::string& header_path);
            virtual ~ExternalIncludeCXXSourceStream();

            
        private:
            SourceStream& _stream;
        };

        class NamespaceCXXSourceScopedStream
        {
        public:
            explicit NamespaceCXXSourceScopedStream(SourceStream& sourceStream, const std::string& namespace_name);
            explicit NamespaceCXXSourceScopedStream(SourceStream& sourceStream, const std::vector<std::string>& namespace_names);
            virtual ~NamespaceCXXSourceScopedStream();
        
            NamespaceCXXSourceScopedStream(NamespaceCXXSourceScopedStream&& other);
        private:
            std::vector<NamespaceCXXSourceScopedStream> _stack;
            SourceScopedStream* _stream {nullptr};

#if _DEBUG
            std::string _namespace_name;
#endif
        };

        class ClassCXXSourceScopedStream
        {
        public:
            explicit ClassCXXSourceScopedStream(SourceStream& sourceStream, bool isDeclaration, const std::string& name, const std::vector<std::string>& base_names = std::vector<std::string>{});
            virtual ~ClassCXXSourceScopedStream();
            
        private:
            SourceScopedStream* _stream{ nullptr };
        };

        class AccessCXXSourceScopedStream
        {
        public:
            enum class Specifier
            {
                Public,
                Private,
                Protected,
            };
            explicit AccessCXXSourceScopedStream(SourceStream& sourceStream, const Specifier& specifier = Specifier::Public);
            virtual ~AccessCXXSourceScopedStream();
            
        private:
            UnindentedSourceScopedStream* _outer_stream {nullptr};
            SourceScopedStream* _inner_stream;
        };

        class EnumCXXSourceScopedStream
        {
        public:
            explicit EnumCXXSourceScopedStream(SourceStream& sourceStream, bool isDeclaration, const std::string& name);
            virtual ~EnumCXXSourceScopedStream();

            void addElement(const std::string& key, const std::string& value);
        private:
            SourceScopedStream* _stream{nullptr};
        };

        
        class MethodCXXSourceScopedStream
        {
        public:


            explicit MethodCXXSourceScopedStream(
                SourceStream& sourceStream, 
                bool isDeclaration, 
                const std::string& prefix,
                const std::string& postfix,
                const std::string& type, 
                const std::vector<std::string>& scopes,
                const std::string& name, 
                const std::vector<ParameterNode>& parameters = std::vector<ParameterNode>());
            virtual ~MethodCXXSourceScopedStream();

            
        private:
            SourceScopedStream* _stream{ nullptr };
        };

        class CallCXXSourceScopedStream
        {
        public:
            explicit CallCXXSourceScopedStream(
                SourceStream& sourceStream,
                const std::string& type,
                const std::vector<std::string>& scopes,
                const std::string name,
                const std::vector<ParameterNode>& parameters = std::vector<ParameterNode>()
                );
            virtual ~CallCXXSourceScopedStream();
        private:
        };


        class CommentCXXSourceStream
        {
        public:
            explicit CommentCXXSourceStream(SourceStream& sourceStream);
            virtual ~CommentCXXSourceStream();

            void addName(const std::string& name);
            void addDescription(const std::string& description);
            void addBrief(const std::string& brief);
            void addReturn(const std::string& return_value);
            void addParameter(const std::string& name, const std::string& description);


        private:
            SourceStream& _stream;
            std::vector<std::string> tokenize(const std::string& description);
        };
        
        class CallbackCXXSourceStream
        {
        public:
            explicit CallbackCXXSourceStream(SourceStream& sourceStream, const std::string& name, const std::string& ret_type, const std::vector<std::string>& param_types);
            virtual ~CallbackCXXSourceStream() {}
        };
    }
}

#endif
