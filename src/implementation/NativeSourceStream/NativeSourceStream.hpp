#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCESTREAM__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_NATIVESOURCESTREAM__

#include <string>
#include <sstream>
#include <vector>
#include <random>
#include <iostream>

#include "../SymbolTable/SymbolTable.hpp"

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        static const char new_indent[] = "    ";
        static size_t indent_size = sizeof(new_indent)-1;

        class [[deprecated]] DefineOnce
        {
        public:
            explicit DefineOnce(std::stringstream& ss, const std::vector<std::string>& moduleNames, const std::string& name, std::string& indent);
            virtual ~DefineOnce();
        private:
            std::string makeGUID();
            std::stringstream& _ss;
            std::string& _indent;
            std::string macro{ "" };
        };

       class [[deprecated]] DefineInclude
       {
       public:
           explicit DefineInclude(std::stringstream& ss, std::string& indent);
           void addExternal(const std::string& header_path);
           void addInternal(const std::string& header_path);
       private:
           std::stringstream& _ss;
           std::string& _indent;
       };

       class [[deprecated]] DefineNamespace
       {
       public:
           explicit DefineNamespace(std::stringstream& ss, const std::vector<std::string>& modules, std::string& indent);
           explicit DefineNamespace(std::stringstream&ss, const std::string& name, std::string& indent);

            void addLine(const std::string& line);

           virtual ~DefineNamespace();
       private:
           std::stringstream& _ss;
           std::string& _indent;
           size_t _depth{ 0 };
       };
              

       class [[deprecated]] DefineObject
       {
       public:
           explicit DefineObject(std::stringstream& ss, const std::string& object, std::string& indent);
           virtual ~DefineObject();

           void addLine(const std::string& line);
       private:
           std::stringstream& _ss;
           std::string& _indent;
       };

       class [[deprecated]] Comment
       {
       public:
           Comment(std::stringstream& ss, std::string& indent);
           virtual ~Comment();
           void add(const SymbolPackage& obj);
           void add(const SymbolModule& obj);
           void add(const SymbolClass& obj);
           void add(const SymbolEnum& obj);
           void add(const SymbolMethod& obj);
           void add(const SymbolParameter& obj);
           void add(const SymbolProperty& obj);

       private:
           std::vector<std::string> tokenize(const std::string& description);
           std::stringstream& _ss;
           std::string& _indent;
       };
    }
}

#endif