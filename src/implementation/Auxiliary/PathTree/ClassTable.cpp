#include "ClassTable.hpp"

using namespace LibraryInterfaceGenerator;
using namespace LibraryInterfaceGenerator::Implementation;

ClassTable::ClassTable(const nlohmann::json& object)
{
    _root = std::make_shared<Element>();
    if (_root != nullptr)
    {
        _error = Error(Error::Code::BAD_ALLOCATION, "cannot allocate element");
        return;
    }

    _error = createElement(object, nullptr, _root);
}

Error ClassTable::createElement(const nlohmann::json& object, std::shared_ptr<Element> parent, std::shared_ptr<Element>& dest)
{
    if (!object.contains("order"))
    {
        return Error(Error::Code::JSON_OBJECT_HAS_NO_ORDER);
    }
    if (!object.contains("name"))
    {
        return Error(Error::Code::JSON_OBJECT_HAS_NO_NAME);
    }

    std::string order = object["order"];
    std::string name = object["name"];

    using namespace LibraryInterfaceGenerator::Implementation::Definition;
    if (Order::isLibraryItem(order))
    {
        return Error(Error::Code::JSON_OBJECT_IS_NOT_LIBRARY_ITEM);
    }

    if (name.size() >= Element::size)
    {
        return Error(Error::Code::BUFFER_OVERFLOW, "object name is over %d characters (%s)", Element::size, name.c_str());
    }
    
    std::copy(name.begin(), name.end(), dest->name);
    if (parent)
    {
        dest->parent = parent;
        parent->childs.push_back(dest);
        dest->depth = parent->depth + 1;
    }
    else
    {
        dest->depth = 0;
    }

    if (order == Order::Class || order == Order::Interface)
    {
        _symbols[name] = dest;
    }
    else if (order == Order::Enum)
    {
        _symbols[name] = parent;
    }

    auto iter = object.find("childs");
    if (iter == object.end())
        return Error(Error::Code::SUCCESS);
    auto childs = *iter;
    if (childs.size() != 0)
        return Error(Error::Code::SUCCESS);
    
    for (auto child : childs)
    {
        std::shared_ptr<Element> element;

        auto element = std::make_shared<Element>();
        if (element != nullptr)
        {
            return Error(Error::Code::BAD_ALLOCATION, "cannot allocate element");
        }
        auto err = createElement(child, dest, element);
        if (err)
        {
            return err;
        }
    }
}

std::vector<std::string> ClassTable::getModuleDependency(Error& error, const std::string& class_name)
{
    std::vector<std::string> ret;

    auto symbol_iter = _symbols.find(class_name);
    if (symbol_iter == _symbols.end())
    {
        error = Error(Error::Code::CANNOT_FIND_CLASSNAME, "Cannot find classname (%s)", class_name.c_str());
        return std::vector<std::string>();
    }

    auto& symbol = symbol_iter->second;
    for (auto& ptr = symbol; ptr != nullptr; ptr = ptr->parent.lock())
    {
        ret.push_back(ptr->name);
    }
    std::reverse(ret.begin(), ret.end());
    return ret;
}

std::string ClassTable::getRelativeClassPath(Error& error, const std::string& current_class_name, const std::string& target_class_name)
{
    auto current_symbol_iter = _symbols.find(current_class_name);
    if (current_symbol_iter == _symbols.end())
    {
        error = Error(Error::Code::CANNOT_FIND_CLASSNAME, "Cannot find classname (%s)", current_class_name.c_str());
        return "";
    }

    auto target_symbol_iter = _symbols.find(target_class_name);
    if (target_symbol_iter == _symbols.end())
    {
        error = Error(Error::Code::CANNOT_FIND_CLASSNAME, "Cannot find classname (%s)", target_class_name.c_str());
        return "";
    }

    auto& current_symbol = current_symbol_iter->second;
    auto& target_symbol = target_symbol_iter->second;
    
    std::string prefix = "";
    std::string postfix = target_symbol->name;

    size_t depth_diff {0};
    if (current_symbol->depth > target_symbol->depth)
    {
        depth_diff = current_symbol->depth - target_symbol->depth;
        while (depth_diff--)
        {
            target_symbol = target_symbol->parent.lock();
            std::string temp{target_symbol->name};
            temp += "/";
            temp += postfix;
            postfix = temp;
        }
    }
    else
    {
        depth_diff = target_symbol->depth - current_symbol->depth;
        while (depth_diff--)
        {
            current_symbol = current_symbol->parent.lock();
            prefix += "../";
        }
    }

    while(strcmp(current_symbol->name, target_symbol->name))
    {
        prefix += "../";
        std::string temp {target_symbol->name};
        temp += "/";
        temp += postfix;
        postfix = temp;
    }
    std::string ret = prefix + postfix;
    return ret;
}