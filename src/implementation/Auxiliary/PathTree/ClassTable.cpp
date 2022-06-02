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
        auto err = createElement(child, dest, element);
        if (err)
        {
            return err;
        }
    }
}

std::vector<std::string> ClassTable::getModuleDependency(const std::string& class_name)
{

}

std::string ClassTable::getRelativeClassPath(const std::string& current_class_name, const std::string& target_class_name)
{

}

std::string ClassTable::getRelativeEnumPath(const std::string& current_class_name, const std::string& target_class_name, const std::string& target_enum_name)
{

}