#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_DEFINITION__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_DEFINITION__
#include <string>
#include <cstring>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        namespace Definition
        {
            namespace Field
            {
                static const char* Order = "order";
                static const char* Name = "name";
                static const char* Description = "description";
                static const char* Author = "author";
                static const char* Childs = "childs";
                static const char* Value = "value";
                static const char* Values = "values";
                static const char* Type = "type";
                static const char* ReadOnly = "readonly";
                static const char* Parameters = "parameters";
    
            }
            namespace Order
            {
                static const char* Package = "package";
                static const char* Module = "module";

                static const char* Enum = "enum";
                static const char* Class = "class";
                static const char* Interface = "interface";

                static const char* Property = "property";
                static const char* Method = "method";
                static const char* Parameter = "parameter";

                inline bool isLibraryItem(const std::string& order) const {
                    return !strcmp(order.c_str(), Package) || 
                        !strcmp(order.c_str(), Module) ||
                        !strcmp(order.c_str(), Enum) || 
                        !strcmp(order.c_str(), Class) || 
                        !strcmp(order.c_str(), Interface);
                }
                inline bool isLibraryFunction(const std::string& order) const {
                    !strcmp(order.c_str(), Property) || 
                    !strcmp(order.c_str(), Method) || 
                    !strcmp(order.c_str(), Parameter);
                }

            }
        }
    }
}

#endif