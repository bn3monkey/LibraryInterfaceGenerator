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
                static const char* IO = "io";
                static const char* Return = "return";
                static const char* Base = "base";
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

                static const char* Callback = "callback";

                inline bool isLibraryItem(const std::string& order) {
                    return !strcmp(order.c_str(), Package) || 
                        !strcmp(order.c_str(), Module) ||
                        !strcmp(order.c_str(), Enum) || 
                        !strcmp(order.c_str(), Class) || 
                        !strcmp(order.c_str(), Interface) || 
                        !strcmp(order.c_str(), Callback);
                }
                inline bool isLibraryFunction(const std::string& order) {
                    !strcmp(order.c_str(), Property) || 
                    !strcmp(order.c_str(), Method) || 
                    !strcmp(order.c_str(), Parameter);
                }

            }
        }
    }
}

#endif