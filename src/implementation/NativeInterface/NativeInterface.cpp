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
	return std::string();
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createStaticMethodDefinition(const SymbolMethod& object)
{
	return std::vector<std::string>();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::callStaticMethod(const SymbolMethod& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createParametersDefinition(const SymbolMethod& parameters)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createParameterDefinition(const SymbolParameter& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createInputParameterChanger(const SymbolParameter& object)
{
	return std::string();
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createOutputParameterChanger(const SymbolParameter& object)
{
	return std::string();
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
	ret.push_back("std::shared_ptr<T> getReference(void* cptr)");
	ret.push_back("{");
	ret.push_back("    auto* ret = reinterpret_cast<std::shared_ptr<T>*>(cptr);");
	ret.push_back("    return *ret;");
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