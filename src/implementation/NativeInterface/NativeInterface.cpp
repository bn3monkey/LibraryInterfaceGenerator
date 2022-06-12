#include "NativeInterface.hpp"

#ifdef __linux__
static char* delimeter = "/";
#elif _WIN32
static char* delimeter = "\\";
#else
static char* delimeter = "/";
#endif

LibraryInterfaceGenerator::Implementation::NativeInterface::NativeInterface(Environment environment, const NativeExternalLibraryDirectory& libDirectory, const SymbolTable& symbolTable, std::string root_dir_path)
	: _symbolTable(symbolTable), _libDirectory(libDirectory)
{
	_interface_dir_path = root_dir_path;
	_interface_dir_path += delimeter;
	_interface_dir_path += "interface";

	_include_dir_path = root_dir_path;
	_include_dir_path += delimeter;
	_include_dir_path += "include";
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::NativeInterface::make()
{
	auto& package = _symbolTable.getPackage();
    
    Result result{ Result::Code::SUCCESS };

    result = FileSystem::createDirectory(_interface_dir_path);
    if (!result)
        return result;

    result = createInterfaceFile(package, _interface_dir_path);
    if (!result)
        return result;

    return result;
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::NativeInterface::createInterfaceFile(const SymbolPackage& symbolObject, std::string& parent_include_path)
{
	std::string header_path{ parent_include_path };
    header_path += delimeter;
    header_path += symbolObject.name;
    header_path += "Interface.hpp";

	std::string cpp_path{ parent_include_path };
    cpp_path += delimeter;
    cpp_path += symbolObject.name;
    cpp_path += "Interface.cpp";

    Result result;
    std::string header_content;
	std::string cpp_content;
    result = createInterfaceContent(symbolObject, header_content, cpp_content);
    if (!result)
        return result;

    result = FileSystem::createFile(header_path, header_content);
    if (!result)
        return result;

	result = FileSystem::createFile(cpp_path, cpp_content);
    if (!result)
        return result;

    return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::NativeInterface::createInterfaceContent(const SymbolPackage& symbolObject, std::string& header_content, std::string& cpp_content)
{
	std::string libraryName = symbolObject.name;
	std::transform(libraryName.begin(), libraryName.end(), libraryName.begin(), ::toupper);

	export_macro = libraryName + "_EXPORT";
	api_macro = libraryName + "_API";
	root_namespace = libraryName;
	root_namespace += "API";

	std::stringstream ss {""};
	createPackageDeclaration(symbolObject, ss);
	header_content = ss.str();

	ss.str("");
	auto result = createPackageDefinition(symbolObject, ss);
	if (!result)
		return result;
	cpp_content = ss.str();

	return Result();
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createPackageDeclaration(const SymbolPackage& symbolObject, std::stringstream& ss)
{
	std::string indent;
	std::vector<std::string> moduleName{ symbolObject.name };

	{
		DefineOnce defineOnce(ss, moduleName, "INTERFACE", indent);

		{
			DefineInclude defineInclude(ss, indent);
			defineInclude.addExternal("string");
			defineInclude.addExternal("vector");
		}

		ss << "#ifdef " << export_macro << "\n";
		ss << "#define " << api_macro << "\n";
		ss << "#else" << "\n";
		ss << "#define " << api_macro << "\n";
		ss << "#endif\n\n";


		DefineNamespace defineNamespace(ss, root_namespace, indent);

		for (auto& mod : symbolObject.modules)
		{
			createModuleDeclaration(*mod, ss, indent);
		}
	}

}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createModuleDeclaration(const SymbolModule& symbolObject, std::stringstream& ss, std::string& indent)
{
	DefineNamespace defineNamespace(ss, symbolObject.name, indent);

	for (auto& method : symbolObject.globla_methods)
	{
		auto line = createStaticMethodDeclaration(*method);
		defineNamespace.addLine(line);
	}
	for (auto& interface : symbolObject.interfaces)
	{
		createClassDeclaration(*interface, ss, indent);
	}
	for (auto& clazz : symbolObject.classes)
	{
		createClassDeclaration(*clazz, ss, indent);
	}

	for (auto& subModule : symbolObject.submodules)
	{
		createModuleDeclaration(symbolObject, ss, indent);
	}

}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createClassDeclaration(const SymbolClass& clazz, std::stringstream& ss, std::string& indent)
{
	DefineNamespace defineNamespace{ ss, clazz.name, indent };
	{
		for (auto& constructor : clazz.constructors)
		{
			auto line = createConstructorDeclaration(clazz, *constructor);
			defineNamespace.addLine(line);
		}
		{
			auto line = createDestructorDeclaration(clazz);
			defineNamespace.addLine(line);
		}

		auto base_props = clazz.getBaseProperties();
		for (auto& base_prop : base_props)
		{
			auto lines = createPropertyDeclaration(*base_prop);
			for (auto& line : lines)
			{
				defineNamespace.addLine(line);
			}
		}
		for (auto& prop : clazz.properties)
		{
			auto lines = createPropertyDeclaration(*prop);
			for (auto& line : lines)
			{
				defineNamespace.addLine(line);
			}
		}

		auto base_methods = clazz.getBaseMethods();
		for (auto& base_method: base_methods)
		{
			auto line = createClassMethodDeclaration(*base_method);
			defineNamespace.addLine(line);
		}
		for (auto& method : clazz.methods)
		{
			auto line = createClassMethodDeclaration(*method);
			defineNamespace.addLine(line);
		}
	}
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::NativeInterface::createPackageDefinition(const SymbolPackage& symbolObject, std::stringstream& ss)
{
	std::string indent;
	{
		DefineInclude defineInclude{ ss, indent };
		defineInclude.addInternal(symbolObject.name + "Interface.hpp");

		defineInclude.addInternal("../include/" + symbolObject.name + ".hpp");
		
		if (_libDirectory.existsExternalTool(NativeExternalLibraryDirectory::ExternalTool::MemoryPool))
		{
			auto pool_path = _libDirectory.getRelativeHeaderPath(NativeExternalLibraryDirectory::ExternalTool::MemoryPool);
			defineInclude.addInternal(pool_path);
		}	
	}
	ss << "\n";
	ss << "using namespace " << root_namespace << ";\n";
	ss << "\n";

	auto lines = createMemoryPoolFunctions();
	for (auto& line : lines)
	{
		ss << line << "\n";
	}
	ss << "\n";


	for (auto& mod : symbolObject.modules)
	{
		createModuleDefinition(*mod, ss);
	}
	return Result();
}
void LibraryInterfaceGenerator::Implementation::NativeInterface::createModuleDefinition(const SymbolModule& mod, std::stringstream& ss)
{
	for (auto& method : mod.globla_methods)
	{
		auto lines = createStaticMethodDefinition(*method);
		for (auto& line : lines)
		{
			ss << line << "\n";
		}
	}
	for (auto& interface : mod.interfaces)
	{
		createClassDefinition(*interface, ss);
	}
	for (auto& clazz : mod.classes)
	{
		createClassDefinition(*clazz, ss);
	}
	for (auto& subModule : mod.submodules)
	{
		createModuleDefinition(*subModule, ss);
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createClassDefinition(const SymbolClass& clazz, std::stringstream& ss)
{
	for (auto& constructor : clazz.constructors)
	{
		auto lines = createConstructorDefinition(clazz, *constructor);
		for(auto& line : lines)
		{
			ss << line << "\n";
		}
	}
	{
		auto lines = createDestructorDefinition(clazz);
		for(auto& line : lines)
		{
			ss << line << "\n";
		}
	}
	auto base_props = clazz.getBaseProperties();
	for (auto& base_prop : base_props)
	{
		auto decls = createPropertyDefinition(clazz, *base_prop);
		for (auto& decl : decls)
		{
			ss << decl << "\n";
		}
	}
	for (auto& prop : clazz.properties)
	{
		auto decls = createPropertyDefinition(clazz, *prop);
		for (auto& decl : decls)
		{
			ss << decl << "\n";
		}
	}
	auto base_methods = clazz.getBaseMethods();
	for (auto& base_method: base_methods)
	{
		auto decls = createClassMethodDefinition(clazz, *base_method);
		for (auto& decl : decls)
		{
			ss << decl << "\n";
		}
	}
	for (auto& method : clazz.methods)
	{
		auto decls = createClassMethodDefinition(clazz, *method);
		for (auto& decl : decls)
		{
			ss << decl << "\n";
		}
	}
}


std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createConstructorDeclaration(const SymbolClass& clazz, const SymbolMethod& constructor)
{
	std::string ret;
	ret += "extern ";
	ret += api_macro;
	ret += " void* construct(";
	if (!constructor.parameters.empty())
	{
		ret += createParametersDefinition(constructor);
	}
	ret += ");";
	return ret;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createConstructorDefinition(const SymbolClass& clazz, const SymbolMethod& constructor)
{
	std::vector<std::string> ret;
	{
		std::string line {"void* "};
		line += createScope(clazz);
		line += "construct(";

		if (!constructor.parameters.empty())
			line += createParametersDefinition(constructor);

		line += ")";
		ret.push_back(line);
	}
	{
		ret.push_back("{");

		std::string indent{ "   " };
		for (auto& parameter : constructor.parameters)
		{
			auto lines = createInputParameterChanger(*parameter);
			for (auto& line : lines)
				ret.push_back(indent + line);
		}
		{
			std::string line = "    ";
			line += allocate(clazz, constructor);
			ret.push_back(line);
		}
		for (auto& parameter : constructor.parameters)
		{
			if (parameter->io == SymbolParameter::IO::OUT)
			{
				auto lines = createOutputParameterChanger(*parameter);
				for (auto& line : lines)
					ret.push_back(indent + line);
			}
		}
		ret.push_back("}");
	}
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::allocate(const SymbolClass& clazz, const SymbolMethod& constructor)
{
	std::string ret {"return createReference<"};
	for (auto& moduleName : clazz.parentModules)
	{
		ret += moduleName;
		ret += "::";
	}
	for (auto& className : clazz.parentObjects)
	{
		ret += className;
		ret += "::";
	}
	ret += clazz.name;
	ret += ">(";
	std::string parameters;
	if (!constructor.parameters.empty())
	{
		for (auto& parameter : constructor.parameters)
		{
			parameters += "i_";
			parameters += parameter->name;
			parameters += ", ";
		}
		parameters.pop_back();
		parameters.pop_back();
		ret += parameters;
	}
	ret += ");";
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createDestructorDeclaration(const SymbolClass& clazz)
{
	std::string line{ "extern " };
	line += api_macro;
	line += " void release(void* handle); ";
	return line;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createDestructorDefinition(const SymbolClass& clazz)
{
	std::vector<std::string> ret;
	{
		std::string line {"void "};
		line += createScope(clazz);
		line += "release(void* handle)";
		ret.push_back(line);
	}
	{
		ret.push_back("{");
		std::string line = "    ";
		line += deallocate(clazz);
		ret.push_back(line);
		ret.push_back("}");
	}
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::deallocate(const SymbolClass& clazz)
{
	std::string ret {"releaseReference<"};
	for (auto& moduleName : clazz.parentModules)
	{
		ret += moduleName;
		ret += "::";
	}
	for (auto& className : clazz.parentObjects)
	{
		ret += className;
		ret += "::";
	}
	ret += clazz.name;
	ret += ">(handle);";
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createClassMethodDeclaration(const SymbolMethod& object)
{
	std::string method{ "extern " };
	method += api_macro;
	method += " ";
	method += object.type->toCppInterfaceType();
    method += " ";
	method += object.name;
    method += "(";
	method += "void* handle";
	if (!object.parameters.empty())
	{
		method += ", ";
		method += createParametersDefinition(object);
	}
	method += ");";
	return method;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createClassMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object)
{
	std::vector<std::string> ret;
	{
		std::string line;
		line += object.type->toCppInterfaceType();
    	line += " ";
		line += createScope(clazz);
		line += object.name;
    	line += "(void* handle";
		if (!object.parameters.empty())
		{
			line += ", ";
			line += createParametersDefinition(object);
		}
		line += ")";
		ret.push_back(line);
	}
	ret.push_back("{");

	std::string indent{ "    " };
	{
		{
			std::string line = "    auto ptr = getReference<";
			line += clazz.getCppName();
			line += ">(handle);";
			ret.push_back(line);
		}
		for (auto& parameter : object.parameters)
		{
			auto lines = createInputParameterChanger(*parameter);
			for (auto& line : lines)
			{
				ret.push_back(indent + line);
			}
		}
		{
			ret.push_back(indent + callClassMethod(object));
		}
		for (auto& parameter : object.parameters)
		{
			if (parameter->io == SymbolParameter::IO::OUT)
			{
				auto lines = createOutputParameterChanger(*parameter);
				for (auto& line : lines)
				{
					ret.push_back(indent + line);
				}
			}
		}
		if (object.type->getTypeName() != SymbolType::Name::VOID)
		{
			auto lines = createReturnValueChanger(object);
			for (auto& line : lines)
			{
				ret.push_back(indent + line);
			}
			ret.push_back("    return __ret;");
		}
	}
	ret.push_back("}");
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::callClassMethod(const SymbolMethod& object)
{
	std::string ret;
	if (object.type->getTypeName() != SymbolType::Name::VOID)
	{
		ret += "auto __temp_ret = ";
	}
	ret += "ptr->";
	ret += object.name;
	ret += "(";

	if (!object.parameters.empty())
	{
		std::string parameters;
		for (auto& parameter : object.parameters)
		{
			parameters += "i_";
			parameters += parameter->name;
			parameters += ", ";
		}
		parameters.pop_back();
		parameters.pop_back();
		ret += parameters;
	}
	ret += ");";
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createStaticMethodDeclaration(const SymbolMethod& object)
{
	std::string method{ "extern " };
	method += api_macro;
	method += " ";
	method += object.type->toCppInterfaceType();
    method += " ";
	method += object.name;
    method += "(";
	method += createParametersDefinition(object);
	method += ");";
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
		std::string indent { "    " };
		for (auto& parameter : object.parameters)
		{
			
			auto lines = createInputParameterChanger(*parameter);
			for (auto& line : lines)
			{
				ret.push_back(indent + line);
			}
		}
		{
			ret.push_back(indent + callStaticMethod(object));
		}
		for (auto& parameter : object.parameters)
		{
			if (parameter->io == SymbolParameter::IO::OUT)
			{
				auto lines = createOutputParameterChanger(*parameter);
				for (auto& line : lines)
				{
					ret.push_back(indent + line);
				}
			}
		}
		if (object.type->getTypeName() != SymbolType::Name::VOID)
		{
			auto lines = createReturnValueChanger(object);
			for (auto& line : lines)
			{
				ret.push_back(indent + line);
			}
			ret.push_back("    return __ret;");
		}
	}
	ret.push_back("}");
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::callStaticMethod(const SymbolMethod& object)
{
	std::string ret;
	if (object.type->getTypeName() != SymbolType::Name::VOID)
	{
		ret += "auto __temp_ret = ";
	}
	for (auto& moduleName : object.parentModules)
	{
		ret += moduleName;
		ret += "::";
	}
	ret += object.name;
	ret += "(";

	if (!object.parameters.empty())
	{
		std::string parameters;
		for (auto& parameter : object.parameters)
		{
			parameters += "i_";
			parameters += parameter->name;
			parameters += ", ";
		}
		parameters.pop_back();
		parameters.pop_back();
		ret += parameters;
	}
	ret += ");";
	return ret;
}



std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createParametersDefinition(const SymbolMethod& object)
{
	std::string line;
	if (!object.parameters.empty())
	{
		for (auto& parameter : object.parameters)
		{
			line += createParameterDefinition(*parameter);
			line += ", ";
		}
		line.pop_back();
		line.pop_back();
	}
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

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createReturnValueChanger(const SymbolMethod& object)
{
	std::vector<std::string> ret;
	switch (object.type->getTypeName())
	{
	case SymbolType::Name::ENUM:
	{
		std::string line;
		line += "auto __ret = static_cast<";
		line += object.type->toCppInterfaceType();
		line += ">(__temp_ret);";
		ret.push_back(line);
	}
		break;
	case SymbolType::Name::OBJECT:
	{
		std::string line;
		line += "auto __ret = (";
		line += object.type->toCppInterfaceInnerType();
		line += ")";
		line += "cloneReference<";
		line += object.type->toCppType();
		line += ">(__temp_ret);";
		ret.push_back(line);
	}
		break;
	case SymbolType::Name::ENUMARRAY:
	case SymbolType::Name::ENUMVECTOR:
	{
		std::string line{ "auto __ret = " };
		line += object.type->toCppInterfaceType();
		line += "(); ";
		ret.push_back(line);
	}
	{
		std::string line;
		line += "for (auto& __temp : __temp_ret)";
		ret.push_back(line);
	}
		ret.push_back("{");
	{ 
		std::string line;
		line = "    __ret.push_back(static_cast<";
		line += object.type->toCppInterfaceInnerType();
		line += ">(__temp));";
		ret.push_back(line);
	}
		ret.push_back("}");
		break;
	case SymbolType::Name::OBJECTARRAY:
	case SymbolType::Name::OBJECTVECTOR:
	{
		std::string line{ "auto __ret = " };
		line += object.type->toCppInterfaceType();
		line += "(); ";
		ret.push_back(line);
	}
	{
		std::string line;
		line += "for (auto& __temp : __temp_ret)";
		ret.push_back(line);
	}
	ret.push_back("{");
	{
		std::string line;
		line += "    __ret.push_back((";
		line += object.type->toCppInterfaceInnerType();
		line += ")cloneReference<";
		line += object.type->toCppInnerType();
		line += ">(__temp));";
		ret.push_back(line);
	}
		ret.push_back("}");
		break;
	default:
		if (object.type->isPrimitive())
			ret.push_back("auto __ret = __temp_ret;");
		else
			ret.push_back("auto& __ret =  __temp_ret;");
		break;
	}
	return ret;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createInputParameterChanger(const SymbolParameter& object)
{
	std::vector<std::string> ret;
		
	switch(object.type->getTypeName())
	{
	case SymbolType::Name::ENUM:
		{
			std::string line;
			line = "auto i_";
			line += object.name;
			line += " = ";
			line += "static_cast<";
			line += object.type->toCppType();
			line += ">(";
			line += object.name;
			line += ");";
			ret.push_back(line);
		}
		break;
	case SymbolType::Name::OBJECT:
		{
			std::string line;
			line = "auto i_";
			line += object.name;
			line += " = ";
			line += "getReference<";
			line += object.type->toCppInnerType();
			line += ">((void *)";
			line += object.name;
			line += ");";
			ret.push_back(line);
		}
		break;
	case SymbolType::Name::ENUMARRAY:
	case SymbolType::Name::ENUMVECTOR:
		{
			std::string line{ "auto i_" };
			line += object.name;
			line += " = ";
			line += object.type->toCppType();
			line += "(); ";
			ret.push_back(line);
		}
		{
			std::string line;
			line += "for (auto& __temp : ";
			line += object.name;
			line += ")";
			ret.push_back(line);
		}
		ret.push_back("{");
		{
			std::string line;
			line = "	i_";
			line += object.name;
			line += ".push_back(static_cast<";
			line += object.type->toCppInnerType();
			line += ">(__temp));";
			ret.push_back(line);
		}
		ret.push_back("}");
		break;
	case SymbolType::Name::OBJECTARRAY:
	case SymbolType::Name::OBJECTVECTOR:
		{
			std::string line{ "auto i_" };
			line += object.name;
			line += " = ";
			line += object.type->toCppType();
			line += "(); ";
			ret.push_back(line);
		}
		{
			std::string line;
			line += "for (auto& __temp : ";
			line += object.name;
			line += ")";
			ret.push_back(line);
		}
		ret.push_back("{");
		{
			std::string line;
			line = "	i_";
			line += object.name;
			line += ".push_back(getReference<";
			line += object.type->toCppInnerType();
			line += ">((void *)__temp));";
			ret.push_back(line);
		}
		ret.push_back("}");
		break;
	default:
		{
			std::string line = "auto";
			if (!object.type->isPrimitive())
			{
				line += "&";
			}
			line += " i_";
			line += object.name;
			line += " = ";
			line += object.name;
			line += ";";
			ret.push_back(line);
		}
		break;	
	}
	return ret;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createOutputParameterChanger(const SymbolParameter& object)
{
	std::vector<std::string> ret;

	switch (object.type->getTypeName())
	{
	case SymbolType::Name::ENUM:
	{
		std::string line;
		line += object.name;
		line += " = ";
		line += "static_cast<";
		line += object.type->toCppInterfaceType();
		line += ">(i_";
		line += object.name;
		line += ")";
		ret.push_back(line);
	}
	break;
	case SymbolType::Name::OBJECT:
	{
		std::string line;
		line += object.name;
		line += " = ";
		line += "(";
		line += object.type->toCppInterfaceType();
		line += ")cloneReference<";
		line += object.type->toCppInnerType();
		line += ">(i_";
		line += object.name;
		line += ")";
		ret.push_back(line);
	}
	break;
	case SymbolType::Name::ENUMARRAY:
	case SymbolType::Name::ENUMVECTOR:
	{
		std::string line;
		line += "for (auto& __temp : i_";
		line += object.name;
		line += ")";
		ret.push_back(line);
	}
	ret.push_back("{");
	{
		std::string line;
		line += object.name;
		line += ".push_back(static_cast<";
		line += object.type->toCppInterfaceInnerType();
		line += ">(__temp));";
		ret.push_back(line);
	}
	ret.push_back("}");
	break;
	case SymbolType::Name::OBJECTARRAY:
	case SymbolType::Name::OBJECTVECTOR:
	{
		std::string line;
		line += "for (auto& __temp : i_";
		line += object.name;
		line += ")";
		ret.push_back(line);
	}
	ret.push_back("{");
	{
		std::string line{ "    " };
		line += object.name;
		line += ".push_back((";
		line += object.type->toCppInterfaceInnerType();
		line += ")cloneReference<";
		line += object.type->toCppInnerType();
		line += ">(__temp));";
		ret.push_back(line);
	}
	ret.push_back("}");
	break;
	default:
	{
		std::string line;
		line += object.name;
		line += " = i_";
		line += object.name;
		line += ";";
		ret.push_back(line);
	}
	break;
	}
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
	std::string setter{ "extern " };
	setter += api_macro;
	setter += " void set";
    setter += propertyName;
    setter += "(void* handle, ";
	std::string type = object.type->toCppInterfaceType();
	setter += type;
    if (!object.type->isPrimitive())
	{
		if(type != "void*")
        	setter += "&";
	}
    setter += " value);";
    return setter;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertyGetterDeclaration(const std::string& propertyName, const SymbolProperty& object)
{
	std::string getter{ "extern " };
	getter += api_macro;
	getter += " ";
	std::string type = object.type->toCppInterfaceType();
	getter += type;
    if (!object.type->isPrimitive())
	{
		if(type != "void*")
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
    setter += "(void* handle, ";
	std::string type = object.type->toCppInterfaceType();
	setter += type;
    if (!object.type->isPrimitive())
	{
		if(type != "void*")
        	setter += "&";
	}
    setter += " value)";
    return setter;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertyGetterDeclaration(const std::string& scope, const std::string& propertyName, const SymbolProperty& object)
{
	std::string type = object.type->toCppInterfaceType();
	std::string getter = type;
	if (!object.type->isPrimitive())
	{
		if(type != "void*")
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
		line += "(c_value);";
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
		std::string line = "auto";
		if (!object.type->isPrimitive())
			line += "&";
		line += " __temp_ret = ptr->get";
		line += propertyName;
		line += "();";
		ret.push_back(line);
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

	std::string indent = "    ";
	

	auto contents = callPropertyGetter(clazz, object);
	for (auto& content : contents)
	{
		ret.push_back(indent + content);
	}
	{
		auto lines = createOutputPropertyChanger(object);
		for (auto& line : lines)
		{
			ret.push_back(indent + line);
		}
	}
	{
		ret.push_back(indent + "return __ret;");
	}
    ret.push_back("}");

    if (!object.readonly)
    {
        std::string setter = createPropertySetterDeclaration(scope, propertyName, object);
        ret.push_back(setter);
        ret.push_back("{");
		{
			auto lines = createInputPropertyChanger(object);
			for (auto& line : lines)
			{
				ret.push_back(indent + line);
			}
		}
		auto contents = callPropertySetter(clazz, object);
		for (auto& content : contents)
		{
			ret.push_back(indent + content);
		}

		
        ret.push_back("}");
    }

    return ret;
}


std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createInputPropertyChanger(const SymbolProperty& object)
{
	std::vector<std::string> ret;
		
	switch(object.type->getTypeName())
	{
	case SymbolType::Name::ENUM:
		{
			std::string line;
			line += "auto c_value = static_cast<";
			line += object.type->toCppType();
			line += ">(value);";
			ret.push_back(line);
		}
		break;
	case SymbolType::Name::OBJECT:
		{
			std::string line;
			line = "auto c_value = getReference<";
			line += object.type->toCppInnerType();
			line += ">((void *)value);";
			ret.push_back(line);
		}
		break;
	case SymbolType::Name::ENUMARRAY:
	case SymbolType::Name::ENUMVECTOR:
		{
			std::string line{ "auto c_value = " };
			line += object.type->toCppType();
			line += "(); ";
			ret.push_back(line);
		}
		{
			std::string line;
			line += "for (auto& __temp : ";
			line += object.name;
			line += ")";
			ret.push_back(line);
		}
		ret.push_back("{");
		{
			std::string line;
			line += "    c_value.push_back(static_cast<";
			line += object.type->toCppInnerType();
			line += ">(__temp));";
			ret.push_back(line);
		}
		ret.push_back("}");
		break;
	case SymbolType::Name::OBJECTARRAY:
	case SymbolType::Name::OBJECTVECTOR:
		{
			std::string line{ "auto c_value = " };
			line += object.type->toCppType();
			line += "(); ";
			ret.push_back(line);
		}
		{
			std::string line;
			line += "for (auto& __temp : ";
			line += object.name;
			line += ")";
			ret.push_back(line);
		}
		ret.push_back("{");
		{
			std::string line;
			line = "	c_value.push_back(getReference<";
			line += object.type->toCppInnerType();
			line += ">((void *)__temp));";
			ret.push_back(line);
		}
		ret.push_back("}");
		break;
	default:
		{
			std::string line = "auto& c_value = value;";
			ret.push_back(line);
		}
		break;	
	}
	return ret;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeInterface::createOutputPropertyChanger(const SymbolProperty& object)
{
	std::vector<std::string> ret;
	switch (object.type->getTypeName())
	{
	case SymbolType::Name::ENUM:
	{
		std::string line;
		line += "auto __ret = static_cast<";
		line += object.type->toCppInterfaceType();
		line += ">(__temp_ret);";
		ret.push_back(line);
	}
		break;
	case SymbolType::Name::OBJECT:
	{
		std::string line;
		line += "auto __ret = (";
		line += object.type->toCppInterfaceInnerType();
		line += ")";
		line += "cloneReference<";
		line += object.type->toCppInnerType();
		line += ">(__temp_ret);";
		ret.push_back(line);
	}
		break;
	case SymbolType::Name::ENUMARRAY:
	case SymbolType::Name::ENUMVECTOR:
	{
		std::string line{ "auto __ret = " };
		line += object.type->toCppInterfaceType();
		line += "(); ";
		ret.push_back(line);
	}
	{
		std::string line;
		line += "for (auto& __temp : __temp_ret)";
		ret.push_back(line);
	}
		ret.push_back("{");
	{ 
		std::string line;
		line = "    __ret.push_back(static_cast<";
		line += object.type->toCppInterfaceInnerType();
		line += ">(__temp));";
		ret.push_back(line);
	}
		ret.push_back("}");
		break;
	case SymbolType::Name::OBJECTARRAY:
	case SymbolType::Name::OBJECTVECTOR:
	{
		std::string line{ "auto __ret = " };
		line += object.type->toCppInterfaceType();
		line += "(); ";
		ret.push_back(line);
	}
	{
		std::string line;
		line += "for (auto& __temp : __temp_ret)";
		ret.push_back(line);
	}
	ret.push_back("{");
	{
		std::string line;
		line += "    __ret.push_back((";
		line += object.type->toCppInterfaceInnerType();
		line += ")cloneReference<";
		line += object.type->toCppInnerType();
		line += ">(__temp));";
		ret.push_back(line);
	}
		ret.push_back("}");
		break;
	default:
		if (object.type->isPrimitive())
			ret.push_back("auto __ret = __temp_ret;");
		else
			ret.push_back("auto& __ret =  __temp_ret;");
		break;
	}
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createScope(const SymbolMethod& method)
{
    std::string scope;
    auto& moduleNames = method.parentModules;

	scope += root_namespace;
	scope += "::";
	for (size_t i = 1; i < moduleNames.size(); i++)
	{
		scope += moduleNames[i];
		scope += "::";
	}
    return scope;
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createScope(const SymbolClass& clazz)
{
	std::string scope;
    auto& objectNames = clazz.parentObjects;
	auto& moduleNames = clazz.parentModules;

	scope += root_namespace;
	scope += "::";

	for (size_t i = 1; i < moduleNames.size(); i++)
	{
		scope += moduleNames[i];
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
	ret.push_back("template<class T, class ...Args>");
	ret.push_back("void* createReference(Args... args)");
	ret.push_back("{");
	ret.push_back("    auto* ret = MemoryPool::allocate<std::shared_ptr<T>>();");
	ret.push_back("    ret = std::shared_ptr<T>(MemoryPool::allocate<T>(args), [](T* p) {");
	ret.push_back("        MemoryPool::deallocate<T>(p);");
	ret.push_back("    });");
	ret.push_back("    return ret;");
	ret.push_back("}");
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
	ret.push_back("void releaseReference(void* cptr)");
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