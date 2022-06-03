#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_PATHTREE__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_PATHTREE__

#include <vector>
#include <algorithm>
#include <memory>
#include <unordered_map>

#include "IError.hpp"
#include "Definition.hpp"
#include "../../../json/json.hpp"

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        class ClassTable
        {
        public:
            explicit ClassTable(const nlohmann::json& object);

            std::vector<std::string> getModuleDependency(Error& error, const std::string& class_name);
            std::string getRelativeClassPath(Error& error, const std::string& current_class_name, const std::string& target_class_name);
            std::string getRelativeEnumPath(Error& error, const std::string& current_class_name, const std::string& target_class_name, const std::string& target_enum_name);

            inline operator bool() {
                return !static_cast<bool>(_error);
            }

            inline Error error() {
                return _error;
            }

        private:
            struct Element
            {
                static constexpr size_t size = 128;

                size_t depth{0};
                char name[size] = {0};
                std::weak_ptr<Element> parent;
                std::vector<std::shared_ptr<Element>> childs;
            };
            std::shared_ptr<Element> _root;
            std::unordered_map<std::string, std::shared_ptr<Element>> _symbols;
            Error _error;

            Error createElement(const nlohmann::json& object, std::shared_ptr<Element> parent, std::shared_ptr<Element>& dest);
        };
    }
}

#endif