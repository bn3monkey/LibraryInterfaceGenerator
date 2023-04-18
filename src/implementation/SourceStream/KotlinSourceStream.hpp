#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_KOTLINSOURCESTREAM__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_KOTLINSOURCESTREAM__

#include "SourceStream.hpp"
#include <vector>
#include <sstream>
#include <string>
#include <queue>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class PackageKotlinSourceStream
        {
        public:
            explicit PackageKotlinSourceStream(SourceStream& sourceStream, const std::string& root_package_name, const std::vector<std::string>& modules);
            virtual ~PackageKotlinSourceStream();
        private:
            SourceStream& _stream;

        };
        class ImportKotlinSourceStream
        {
        public:
            explicit ImportKotlinSourceStream(SourceStream& sourceStream, const std::string& root_package_name, const std::vector<std::string>& scopes);
            explicit ImportKotlinSourceStream(SourceStream& sourceStream, const std::string& external_class_name);
            virtual ~ImportKotlinSourceStream();
            
        private:
            SourceStream& _stream;
        };

        
        class ClassKotlinSourceScopedStream
        {
        public:
            explicit ClassKotlinSourceScopedStream(SourceStream& sourceStream, const std::string& name, const std::vector<std::string>& base_names = std::vector<std::string>{});
            virtual ~ClassKotlinSourceScopedStream();
            
        private:
            SourceScopedStream* _stream{ nullptr };
        };

        class CompanionObjectKotlinSourceScopedStream
        {
        public:
            explicit CompanionObjectKotlinSourceScopedStream(SourceStream& sourceStream);
            virtual ~CompanionObjectKotlinSourceScopedStream();

            void addSingleTon(const std::string& class_name);

            class Init {
            public:
                explicit Init(SourceStream& sourceStream);
                virtual ~Init();
                
                void loadLibrary(const std::string& library_name);
            private:
                SourceScopedStream* _stream{ nullptr };
            };

        private:
            SourceScopedStream* _stream{ nullptr };
        };

        class EnumKotlinSourceScopedStream
        {
        public:
            explicit EnumKotlinSourceScopedStream(SourceStream& sourceStream, const std::string& name);
            virtual ~EnumKotlinSourceScopedStream();

            void addElement(const std::string& key, const std::string& value);
        private:
            SourceScopedStream* _stream{nullptr};
        };

        class AdvancedEnumKotlinSourceScopedStream
        {
        public:
            explicit AdvancedEnumKotlinSourceScopedStream(SourceStream& sourceStream, const std::string& name);
            virtual ~AdvancedEnumKotlinSourceScopedStream();

            void addElement(const std::string& key, const std::string& value, const std::string& fullname);
        private:
            SourceScopedStream* _stream{ nullptr };

        };


        class MethodKotlinSourceScopedStream
        {
        public:
            enum class Access {
                PUBLIC,
                PROTECTED,
                PRIVATE,
                INTERNAL,
                EXTERNAL,
            };

            explicit MethodKotlinSourceScopedStream(
                SourceStream& sourceStream, 
                Access access, 
                const std::string& prefix,
                const std::string& postfix,
                const std::string& type,
                const std::string& name, 
                const std::vector<ParameterNode>& parameters = std::vector<ParameterNode>());
            virtual ~MethodKotlinSourceScopedStream();

            
        private:
            SourceScopedStream* _stream{ nullptr };
        };

        class CallKotlinSourceScopedStream
        {
        public:
            explicit CallKotlinSourceScopedStream(
                SourceStream& sourceStream,
                const std::string& type,
                const std::vector<std::string>& scopes,
                const std::string name,
                const std::vector<ParameterNode>& parameters = std::vector<ParameterNode>()
            );
            virtual ~CallKotlinSourceScopedStream();
        private:
        };

        class CommentKotlinSourceStream
        {
        public:
            explicit CommentKotlinSourceStream(SourceStream& sourceStream);
            virtual ~CommentKotlinSourceStream();

            void addName(const std::string& name);
            void addDescription(const std::string& description);
            void addBrief(const std::string& brief);
            void addReturn(const std::string& return_value);
            void addParameter(bool isInputParameter, const std::string& name, const std::string& description);


        private:
            SourceStream& _stream;
            std::vector<std::string> tokenize(const std::string& description);
        };
    }
}

#endif