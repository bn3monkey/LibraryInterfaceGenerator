#include "Nativeinterface.hpp"

#ifdef __linux__
static char* delimeter = "/";
#elif _WIN32
static char* delimeter = "\\";
#else
static char* delimeter = "/";
#endif

LibraryInterfaceGenerator::Implementation::NativeInterface::NativeInterface(Environment environment, const SymbolTable& symbolTable, std::string root_dir_path)
	: _symbolTable(symbolTable)
{
	_interface_dir_path = root_dir_path;
	_interface_dir_path += delimeter;
	_interface_dir_path += "interface";

}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::NativeInterface::make()
{
	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::NativeInterface::createInterfaceFile(const SymbolPackage& symbolObject, std::string& parent_include_path)
{
	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::NativeInterface::createInterfaceContent(const SymbolPackage& symbolObject, std::string& header_content, std::string& cpp_content)
{
	return Result();
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::wrapPackageScope(const SymbolPackage& symbolObject, std::stringstream& ss)
{
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::wrapModuleScope(const SymbolModule& symbolObject, std::stringstream& ss, std::string& indent)
{
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createClassDeclaration(const SymbolClass& clazz, const SymbolMethod& object)
{
	return std::vector<std::string>();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createConstructorDeclaration(const SymbolClass& clazz, const SymbolMethod& object)
{
	return std::string();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createConstructorDefinition(const SymbolClass& clazz, const SymbolMethod& object)
{
	return std::vector<std::string>();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::allocate(const SymbolClass& clazz, const SymbolMethod& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createDestructorDeclaration(const SymbolClass& clazz)
{
	return std::string();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createDestructorDefinition(const SymbolClass& clazz)
{
	return std::vector<std::string>();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::deallocate(const SymbolClass& clazz)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createClassMethodDeclaration(const SymbolMethod& object)
{
	return std::string();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createClassMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object)
{
	return std::vector<std::string>();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::callClassMethod(const SymbolClass& clazz, const SymbolMethod& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createStaticMethodDeclaration(const SymbolMethod& object)
{
	std::string method{ "extern LIBRARY_EXPORT " };
	method += object.type->toCppInterfaceType();
    method += " ";
	method += object.name;
    method += "(";
	method += createParametersDefinition(object);
	method += ")";
	return method;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createStaticMethodDefinition(const SymbolMethod& object)
{
	std::vector<std::string> ret;
	{
		std::string line;
		line += object.type->toCppInterfaceType();
    	line += " ";
		line += createScope(object);
		line += object.name;
    	line += "(";
		line += createParametersDefinition(object);
		line += ")";
		ret.push_back(line);
	}
	ret.push_back("{");
	{
		for (auto& parameter : object.parameters)
		{
			ret.push_back(createInputParameterChanger(*parameter));
		}
		ret.push_back(callStaticMethod(object));
		for (auto& parameter : object.parameters)
		{
			ret.push_back(createOutputParameterChanger(*parameter));
		}
		if (object.type->getTypeName() != SymbolType::Name::VOID)
		{
			ret.push_back(createReturnValueChanger(object));
			ret.push_back("return __ret");
		}
	}
	ret.push_back("}");
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::callStaticMethod(const SymbolMethod& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createReturnValueChanger(const SymbolMethod& object)
{
	std::string ret;
	ret += "__ret = ";
		
	switch(object.type->getTypeName())
	{
	case SymbolType::Name::ENUM:
		ret += "static_cast<";
		ret += object.type->toCppInterfaceType();
		ret += ">(";
		ret += object.name;
		ret += ")";
		break;
	case SymbolType::Name::OBJECT:
		ret += "(";
		ret += object.type->toCppInterfaceType();
		ret += ")";
		ret += object.name;
		break;
	case SymbolType::Name::ENUMARRAY:
	case SymbolType::Name::ENUMVECTOR:
		ret += "for (auto& __temp : i_";
		ret += object.name;
		ret += ") { ";
		ret += object.name;
		ret += ".push_back(static_cast<";
		ret += object.type->toCppInterfaceType();
		ret += ">(";
		ret += object.name;
		ret += "));}";
		break;
	case SymbolType::Name::OBJECTARRAY:
	case SymbolType::Name::OBJECTVECTOR:
		ret += "for (auto& __temp : i_";
		ret += object.name;
		ret += ") { ";
		ret += object.name;
		ret += ".push_back((";
		ret += object.type->toCppInterfaceType();
		ret += ")>(";
		ret += object.name;
		ret += "));}";
		break;
	default:
		ret += "i_";
		ret += object.name;
		break;	
	}
	ret += ";";
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createParametersDefinition(const SymbolMethod& object)
{
	std::string line;
    for (auto& parameter : object.parameters)
    {
        line += createParameterDefinition(*parameter);
        line += ", ";
    }
    line.pop_back();
    line.pop_back();
    return line;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createParameterDefinition(const SymbolParameter& object)
{
	std::string content = object.io == SymbolParameter::IO::IN ? "const " : "";
    content += object.type->toCppInterfaceType();
    content += object.type->isPrimitive() ? " " : "& ";
    content += object.name;
    return content;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createInputParameterChanger(const SymbolParameter& object)
{
	std::string ret;
	ret += "auto i_";
	ret += object.name;
	ret += " = ";
		
	switch(object.type->getTypeName())
	{
	case SymbolType::Name::ENUM:
		ret += "static_cast<";
		ret += object.type->toCppType();
		ret += ">(";
		ret += object.name;
		ret += ")";
		break;
	case SymbolType::Name::OBJECT:
		ret += "getReference<";
		ret += object.type->toCppType();
		ret += ">(";
		ret += object.name;
		ret += ")";
		break;
	case SymbolType::Name::ENUMARRAY:
	case SymbolType::Name::ENUMVECTOR:
		ret += "std::vector<";
		ret += object.type->toCppType();
		ret += ">(); ";
		ret += "for (auto& __temp : ";
		ret += object.name;
		ret += ") { i_";
		ret += object.name;
		ret += ".push_back(static_cast<";
		ret += object.type->toCppType();
		ret += ">(";
		ret += object.name;
		ret += "));}";
		break;
	case SymbolType::Name::OBJECTARRAY:
	case SymbolType::Name::OBJECTVECTOR:
		ret += "std::vector<";
		ret += object.type->toCppType();
		ret += ">(); ";
		ret += "for (auto& __temp : ";
		ret += object.name;
		ret += ") { i_";
		ret += object.name;
		ret += ".push_back(getReference<";
		ret += object.type->toCppType();
		ret += ">(";
		ret += object.name;
		ret += "));}";
		break;
	default:
		ret += object.name;
		break;	
	}
	ret += ";";
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createOutputParameterChanger(const SymbolParameter& object)
{
	std::string ret;
	ret += object.name;
	ret += " = ";
		
	switch(object.type->getTypeName())
	{
	case SymbolType::Name::ENUM:
		ret += "static_cast<";
		ret += object.type->toCppInterfaceType();
		ret += ">(";
		ret += object.name;
		ret += ")";
		break;
	case SymbolType::Name::OBJECT:
		ret += "(";
		ret += object.type->toCppInterfaceType();
		ret += ")";
		ret += object.name;
		break;
	case SymbolType::Name::ENUMARRAY:
	case SymbolType::Name::ENUMVECTOR:
		ret += "for (auto& __temp : i_";
		ret += object.name;
		ret += ") { ";
		ret += object.name;
		ret += ".push_back(static_cast<";
		ret += object.type->toCppInterfaceType();
		ret += ">(";
		ret += object.name;
		ret += "));}";
		break;
	case SymbolType::Name::OBJECTARRAY:
	case SymbolType::Name::OBJECTVECTOR:
		ret += "for (auto& __temp : i_";
		ret += object.name;
		ret += ") { ";
		ret += object.name;
		ret += ".push_back((";
		ret += object.type->toCppInterfaceType();
		ret += ")>(";
		ret += object.name;
		ret += "));}";
		break;
	default:
		ret += "i_";
		ret += object.name;
		break;	
	}
	ret += ";";
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertyName(const SymbolProperty& object)
{
	std::string propertyName = object.name;
    char firstChar = propertyName[0];
    if ('a' <= firstChar && firstChar <= 'z')
        firstChar += ('A' - 'a');
    propertyName[0] = firstChar;
	return propertyName;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertySetterDeclaration(const std::string& propertyName, const SymbolProperty& object)
{
	std::string setter{ "extern LIBRARY_EXPORT void set" };
    setter += propertyName;
    setter += "(void* handle, const ";
    setter += object.type->toCppInterfaceType();
    if (!object.type->isPrimitive())
	{
		if(setter != "void*")
        	setter += "&";
	}
    setter += " value);";
    return setter;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertyGetterDeclaration(const std::string& propertyName, const SymbolProperty& object)
{
	std::string getter{"extern LIBRARY_EXPORT "};
	getter += object.type->toCppInterfaceType();
    if (!object.type->isPrimitive())
	{
		if(getter != "void*")
        	getter += "&";
	}
    getter += " get";
    getter += propertyName;
    getter += "(void* handle);";
    return getter;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertyDeclaration(const SymbolProperty& object)
{
	std::vector<std::string> ret;
	std::string propertyName = createPropertyName(object);

	{
		std::string line = createPropertyGetterDeclaration(propertyName, object);
		ret.push_back(line);
	}
	{
		std::string line = createPropertySetterDeclaration(propertyName, object);
		ret.push_back(line);
	}
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertySetterDeclaration(const std::string& scope, const std::string& propertyName, const SymbolProperty& object)
{
	std::string setter{ "void "};
	setter += scope;
	setter += "set";
    setter += propertyName;
    setter += "(void* handle, const ";
    setter += object.type->toCppInterfaceType();
    if (!object.type->isPrimitive())
	{
		if(setter != "void*")
        	setter += "&";
	}
    setter += " value)";
    return setter;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertyGetterDeclaration(const std::string& scope, const std::string& propertyName, const SymbolProperty& object)
{
	std::string getter = object.type->toCppInterfaceType();
    if (!object.type->isPrimitive())
	{
		if(getter != "void*")
        	getter += "&";
	}
    getter += " ";
	getter += scope;
	getter += "get";
    getter += propertyName;
    getter += "(void* handle)";
    return getter;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::callPropertySetter(const SymbolClass& clazz, const SymbolProperty& object)
{
	std::vector<std::string> ret;
	std::string propertyName = createPropertyName(object);

	{
		std::string line = "auto ptr = getReference<";
		line += clazz.getCppName();
		line += ">(handle);";
		ret.push_back(line);
	}
	{
		std::string line = "ptr->set";
		line += propertyName;
		line += "(value);";
		ret.push_back(line);
	}

	return ret;	
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::callPropertyGetter(const SymbolClass& clazz, const SymbolProperty& object)
{
	std::vector<std::string> ret;
	std::string propertyName = createPropertyName(object);

	{
		std::string line = "auto ptr = getReference<";
		line += clazz.getCppName();
		line += ">(handle);";
		ret.push_back(line);
	}
	{
		std::string line = "auto ret = ptr->get";
		line += propertyName;
		line += "();";
		ret.push_back(line);
	}
	{
		ret.push_back("return ret;");
	}
	return ret;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertyDefinition(const SymbolClass& clazz, const SymbolProperty& object)
{
	std::vector<std::string> ret;

    std::string scope = createScope(clazz);

    std::string propertyName = createPropertyName(object);

    std::string getter = createPropertyGetterDeclaration(scope, propertyName, object);

    ret.push_back(getter);
    ret.push_back("{");

	auto contents = callPropertyGetter(clazz, object);
	for(auto& content : contents)
    	ret.push_back(content);

    ret.push_back("}");

    if (!object.readonly)
    {
        std::string setter = createPropertySetterDeclaration(scope, propertyName, object);
        ret.push_back(setter);
        ret.push_back("{");

		auto contents = callPropertySetter(clazz, object);
		for(auto& content : contents)
    		ret.push_back(content);
        
        ret.push_back("}");
    }

    return ret;
}

std::string LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createScope(const SymbolMethod& method)
{
    std::string scope;
    auto& moduleNames = method.parentModules;
    for (auto& moduleName : moduleNames)
    {
        scope += moduleName;
        scope += "::";
    }
    return scope;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createScope(const SymbolClass& clazz)
{
	std::string scope;
    auto& moduleNames = clazz.parentModules;
    auto& objectNames = clazz.parentObjects;
    for (auto& moduleName : moduleNames)
    {
        scope += moduleName;
        scope += "::";
    }
    for (auto& objectName : objectNames)
    {
        scope += objectName;
        scope += "::";
    }
    scope += clazz.name;
    scope += "::";

    return scope;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createMemoryPoolFunctions()
{
	std::vector<std::string> ret;
	ret.push_back("template<class T>");
	ret.push_back("void* createReference()");
	ret.push_back("{");
	ret.push_back("    auto* ret = MemoryPool::allocate<std::shared_ptr<T>>();");
	ret.push_back("    ret = std::shared_ptr<T>(MemoryPool::allocate<T>(), [](T* p) {");
	ret.push_back("        MemoryPool::deallocate<T>(p);");
	ret.push_back("    });");
	ret.push_back("    return ret;");
	ret.push_back("}");
	ret.push_back("template<class T>");
	ret.push_back("void releaseReference()");
	ret.push_back("{");
	ret.push_back("    MemoryPool::deallocate(reinterpret_cast<std::shared_ptr<T>*>(cptr));");
	ret.push_back("}");
	ret.push_back("template<class T>");
	ret.push_back("std::shared_ptr<T>& getReference(void* cptr)");
	ret.push_back("{");
	ret.push_back("    auto* ret = reinterpret_cast<std::shared_ptr<T>*>(cptr);");
	ret.push_back("    return *ret;");
	ret.push_back("}");
	ret.push_back("template<class T>");
	ret.push_back("std::shared_ptr<T>* cloneReference(std::shared_ptr<T>& cptr)");
	ret.push_back("{");
	ret.push_back("    auto* ret = MemoryPool::allocate<std::shared_ptr<T>>();");
	ret.push_back("    *ret = value;");
	ret.push_back("    return ret;");
	ret.push_back("}");
	ret.push_back("template<class T>");
	ret.push_back("std::shared_ptr<T> copyReference(std::shared_ptr<T>& cptr)");
	ret.push_back("{");
	ret.push_back("    auto* ret = MemoryPool::allocate<std::shared_ptr<T>>();");
	ret.push_back("    *ret = value;");
	ret.push_back("    return *ret;");
	ret.push_back("}");
	return ret;
}