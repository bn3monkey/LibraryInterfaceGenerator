#include "NativeInterface.hpp"
#include "../../Converter/CXXConverter.hpp"

using namespace LibraryInterfaceGenerator::Implementation;
using namespace LibraryInterfaceGenerator::Implementation::Definition;

#ifdef __linux__
static char* delimeter = "/";
#elif _WIN32
static char* delimeter = "\\";
#else
static char* delimeter = "/";
#endif

LibraryInterfaceGenerator::Implementation::NativeInterface::NativeInterface(
                Environment environment, 
                const NativeExternalLibraryDirectory& libDirectory,
                const NativeSourceDirectory& srcDirectory, 
                const SymbolTable& symbolTable, 
                std::string root_dir_path,
				const char* interface_dirname)
	: _symbolTable(symbolTable), _libDirectory(libDirectory), _srcDirectory(srcDirectory)
{
	_interface_dir_path = root_dir_path;
	_interface_dir_path += delimeter;
	_interface_dir_path += interface_dirname;

	std::string libraryName = _symbolTable.getPackage().name;
	std::transform(libraryName.begin(), libraryName.end(), libraryName.begin(), ::toupper);

	export_macro = libraryName + "_EXPORT";
	api_macro = libraryName + "_API";
	root_namespace = libraryName;
	root_namespace += "API";
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

	{
		auto ss = createInterfaceHeaderContent(symbolObject);
		auto str = ss.str();

		result = FileSystem::createFile(header_path, str);
		if (!result)
			return result;

	}
	{
		auto ss = createInterfaceCppContent(symbolObject);
		auto str = ss.str();
		result = FileSystem::createFile(cpp_path, str);
		if (!result)
			return result;
	}

    return Result();
}

LibraryInterfaceGenerator::Implementation::SourceStream LibraryInterfaceGenerator::Implementation::NativeInterface::createInterfaceHeaderContent(const SymbolPackage& symbolObject)
{
	SourceStream ss;
	createPackageDeclaration(ss, symbolObject);
	return ss;
}

LibraryInterfaceGenerator::Implementation::SourceStream LibraryInterfaceGenerator::Implementation::NativeInterface::createInterfaceCppContent(const SymbolPackage& symbolObject)
{
	SourceStream ss;
	createPackageDefinition(ss, symbolObject);
	return ss;
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createPackageDeclaration(SourceStream& ss, const SymbolPackage& obj)
{
	{
		HeaderGuardCXXSourceScopedStream headerGuard{ ss, { obj.name}, "INTERFACE" };

		{
			ExternalIncludeCXXSourceStream exclude{ ss, "string" };
		}
		{
			ExternalIncludeCXXSourceStream exclude{ ss, "vector" };
		}
		{
			ExternalIncludeCXXSourceStream exclude{ ss, "functional" };
		}

		ss << "#ifdef " << export_macro << "\n";
		ss << "#define " << api_macro << "\n";
		ss << "#else" << "\n";
		ss << "#define " << api_macro << "\n";
		ss << "#endif\n\n";

		{
			NamespaceCXXSourceScopedStream nameSpace{ ss, root_namespace };

			createReleaserDeclaration(ss);
			createCallbackDeclaration(ss, obj);

			for (auto& module_ : obj.modules)
			{
				createCXXComment(ss, obj);
				createModuleDeclaration(ss, *module_);
			}
		}
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createModuleDeclaration(SourceStream& ss, const SymbolModule& obj)
{
	{
		NamespaceCXXSourceScopedStream nameSpace{ ss, obj.name };

		for (auto& methodObject : obj.global_methods)
		{
			auto& method = methodObject.first;
			createCXXComment(ss, *method);
			createStaticMethodDeclaration(ss, *method);
		}

		for (auto& clazz : obj.classes)
		{
			createCXXComment(ss, *clazz);
			createClassDeclaration(ss, *clazz);
		}

		for (auto& subModule : obj.submodules)
		{
			createCXXComment(ss, *subModule);
			createModuleDeclaration(ss, *subModule);
		}
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createClassDeclaration(SourceStream& ss, const SymbolClass& clazz)
{
	{
		NamespaceCXXSourceScopedStream nameSpace{ ss, clazz.name };

		for (auto& consturctorObject : clazz.constructors)
		{
			auto& constructor = consturctorObject.first;
			createConstructorDeclaration(ss, clazz, *constructor);
		}
		{
			createDestructorDeclaration(ss, clazz);
		}
		{
			createAddReleaserDeclaration(ss, clazz);
		}

		auto base_props = clazz.getBaseProperties();
		for (auto& base_prop : base_props)
		{
			createCXXComment(ss, *base_prop);
			createPropertyDeclaration(ss, *base_prop);
		}

		for (auto& prop : clazz.properties)
		{
			createCXXComment(ss, *prop);
			createPropertyDeclaration(ss, *prop);
		}

		auto base_methods = clazz.getBaseMethods();
		for (auto& baseMethodObject : base_methods)
		{
			auto base_method = baseMethodObject.first;
			createCXXComment(ss, *base_method);
			createClassMethodDeclaration(ss, clazz, *base_method);
		}

		for (auto& methodObject : clazz.methods)
		{
			auto method = methodObject.first;
			createCXXComment(ss, *method);
			createClassMethodDeclaration(ss, clazz, *method);
		}
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createReleaserDeclaration(SourceStream& ss)
{
	CallbackCXXSourceStream callback_scope{
		ss,
		"Releaser",
		"void",
		{}
	};
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createCallbackDeclaration(SourceStream& ss, const SymbolPackage& obj)
{
	for (auto& module_ : obj.modules) {
		createCallbackDeclaration(ss, *module_);
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createCallbackDeclaration(SourceStream& ss, const SymbolModule& obj)
{
	std::vector<std::string> param_types;
	for (auto& callback : obj.callbacks)
	{
		for (auto& param : callback->parameters)
		{
			param_types.push_back(param->type->toManagedType());
		}

		{
			CallbackCXXSourceStream callback_scope{
				ss,
				callback->name,
				callback->type->toManagedType(),
				param_types
			};
		}
	}

	for (auto& module_ : obj.submodules) {
		createCallbackDeclaration(ss, *module_);
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createPackageDefinition(SourceStream& ss, const SymbolPackage& obj)
{
	{
		{
			InternalIncludeCXXSourceStream selfHeaderInclude{ ss,  obj.name + "Interface.hpp" };
		}
		{
			std::stringstream libraryHeaderPathSS;
			libraryHeaderPathSS << "../" << _srcDirectory.getIncludeDirName() << "/" << obj.name << ".hpp";
			InternalIncludeCXXSourceStream libraryHeaderInclude{ ss, libraryHeaderPathSS.str() };
		}
		if (_libDirectory.existsExternalTool(NativeExternalLibraryDirectory::ExternalTool::MemoryPool))
		{
			auto poolPath = _libDirectory.getRelativeHeaderPath(NativeExternalLibraryDirectory::ExternalTool::MemoryPool);
			InternalIncludeCXXSourceStream poolInclude{ ss, poolPath };
		}
		if (_libDirectory.existsExternalTool(NativeExternalLibraryDirectory::ExternalTool::ManagedTypeConverter))
		{
			auto poolPath = _libDirectory.getRelativeHeaderPath(NativeExternalLibraryDirectory::ExternalTool::ManagedTypeConverter);
			InternalIncludeCXXSourceStream poolInclude{ ss, poolPath };
		}

		ss << "\n";
		ss << "using namespace " << root_namespace << ";\n";
		ss << "\n";

		ss << createNativeInterfaceConverter() << "\n";

		for (auto& module_ : obj.modules)
		{
			createModuleDefinition(ss, *module_);
		}
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createModuleDefinition(SourceStream& ss, const SymbolModule& obj)
{
	for (auto& methodObject : obj.global_methods)
	{
		auto& method = methodObject.first;
		createStaticMethodDefinition(ss, *method);
	}
	for (auto& clazz : obj.classes)
	{
		createClassDefinition(ss, *clazz);
	}
	for (auto& subModule : obj.submodules)
	{
		createModuleDefinition(ss, *subModule);
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createClassDefinition(SourceStream& ss, const SymbolClass& clazz)
{
	for (auto& consturctorObject : clazz.constructors)
	{
		auto& consturctor = consturctorObject.first;
		createConstructorDefinition(ss, clazz, *consturctor);
	}
	{
		createDestructorDefinition(ss, clazz);
	}
	{
		createAddReleaserDefinition(ss, clazz);
	}
	auto base_props = clazz.getBaseProperties();
	for (auto& base_prop : base_props)
	{
		 createPropertyDefinition(ss, clazz, *base_prop);
	}
	
	for (auto& prop : clazz.properties)
	{
		createPropertyDefinition(ss, clazz, *prop);
	}

	auto base_methods = clazz.getBaseMethods();
	for (auto& baseMethodObject : base_methods)
	{
		auto& base_method = baseMethodObject.first;
		createClassMethodDefinition(ss, clazz, *base_method);
	}

	for (auto& methodObject : clazz.methods)
	{
		auto& method = methodObject.first;
		createClassMethodDefinition(ss, clazz, *method);
	}
}

// Parameter Block ����
static ParameterNode createParameter(const SymbolParameter& parameter)
{
	int io;
	if (parameter.type->isPrimitive())
	{
		io = ParameterNode::VALUE;
	}
	else
	{
		if (parameter.io == SymbolParameter::IO::OUT)
		{
			io = ParameterNode::REFERENCE_OUT;
		}
		else
		{
			io = ParameterNode::REFERENCE_IN;
		}
	}
	return ParameterNode(io, parameter.type->toManagedType(), parameter.name);
}

static std::vector<ParameterNode> createParameters(const SymbolMethod& object)
{
	std::vector<ParameterNode> ret;
	for (auto& parameter : object.parameters)
	{
		ret.push_back(createParameter(*parameter));
	}
	return ret;
}

static std::vector<ParameterNode> createHandleParameter()
{
	std::vector<ParameterNode> ret;
	ret.push_back(ParameterNode(ParameterNode::VALUE, "void*", "handle"));
	return ret;
}

static std::vector<ParameterNode> createParametersWithHandle(const SymbolMethod& object)
{
	auto ret = createHandleParameter();
	auto parameters = createParameters(object);
	ret.insert(ret.end(), parameters.begin(), parameters.end());
	return ret;
}

static std::vector<ParameterNode> createPropertyParameters(const SymbolProperty& obj)
{
	auto ret = createHandleParameter();
	ret.push_back(

		ParameterNode(
			ParameterNode::REFERENCE_IN,
			obj.type->toManagedType(),
			"value")
	);
	return ret;
}

static std::vector<ParameterNode> createReleaserParameters()
{
	std::vector<ParameterNode> ret{
		ParameterNode(
			ParameterNode::REFERENCE_IN,
			"Releaser",
			"releaser")
	};
	return ret;
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createConstructorDeclaration(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& constructor)
{
	{
		auto parameters = createParameters(constructor);
		std::string prefix = "extern " + api_macro;
		MethodCXXSourceScopedStream method_scope{ ss, true, prefix, "", "void*", {}, "construct", parameters };
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createConstructorDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& constructor)
{
	{
		auto parameters = createParameters(constructor);
		auto scopes = createInterfaceScope(root_namespace, clazz);
		MethodCXXSourceScopedStream method_scope{ ss, false, "", "", "void*", scopes, "construct", parameters };

		for (auto& parameter : constructor.parameters)
		{
			createInputParameterChanger(ss, *parameter);
		}
		allocate(ss, clazz, constructor);
		for (auto& parameter : constructor.parameters)
		{
			createOutputParameterChanger(ss, *parameter);
		}
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::allocate(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& construrctor)
{
	auto scopes = createScope(clazz);
	ss << "return Bn3Monkey::MObject<" << clazz.getCppName() << ">().construct(";

	if (!construrctor.parameters.empty())
	{
		for (auto& parameter : construrctor.parameters)
		{
			ss << "i_" << parameter->name << ", ";
		}
		ss.pop(2);
	}
	ss << ");\n";
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createDestructorDeclaration(SourceStream& ss, const SymbolClass& clazz)
{
	{
		auto parameters = createHandleParameter();
		std::string prefix = "extern " + api_macro;
		MethodCXXSourceScopedStream method_scope{ ss, true, prefix, "", "void", {}, "release", parameters };
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createDestructorDefinition(SourceStream& ss, const SymbolClass& clazz)
{
	{
		auto parameters = createHandleParameter();
		auto scopes = createInterfaceScope(root_namespace, clazz);
		MethodCXXSourceScopedStream method_scope{ ss, false, "", "", "void", scopes, "release", parameters };

		deallocate(ss, clazz);
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::deallocate(SourceStream& ss, const SymbolClass& clazz)
{
	auto scopes = createScope(clazz);
	ss << "return Bn3Monkey::MObject<" << clazz.getCppName() << ">().release(handle);\n";

}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createAddReleaserDeclaration(SourceStream& ss, const SymbolClass& clazz)
{
	{
		auto parameters = createReleaserParameters();

		std::string prefix = "extern " + api_macro;
		MethodCXXSourceScopedStream method_scope{ ss, true, prefix, "", "void", {}, "addReleaser", parameters };
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createAddReleaserDefinition(SourceStream& ss, const SymbolClass& clazz)
{
	{
		auto parameters = createReleaserParameters();
		auto scopes = createInterfaceScope(root_namespace, clazz);

		MethodCXXSourceScopedStream method_scope{ ss, false, "", "", "void", scopes, "addReleaser", parameters };

		addReleaser(ss, clazz);
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::addReleaser(SourceStream& ss, const SymbolClass& clazz)
{
	ss << "Bn3Monkey::ManagedTypeConverter::addReleaser(releaser);\n";
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createClassMethodDeclaration(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& obj)
{
	{
		auto parameters = createParametersWithHandle(obj);
	
		std::string prefix = "extern " + api_macro;

		MethodCXXSourceScopedStream method_scope{ ss, true, prefix, "", obj.type->toManagedType(), {}, obj.name, parameters};
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createClassMethodDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& obj)
{
	{
		auto parameters = createParametersWithHandle(obj);
		auto scope = createInterfaceScope(root_namespace, clazz);

		MethodCXXSourceScopedStream method_scope{ ss, false, "", "", obj.type->toManagedType(), scope, obj.name, parameters };

		ss << "auto ptr = Bn3Monkey::MObject<" << clazz.getCppName() << ">().toNativeType(handle);\n";
		for (auto& parameter : obj.parameters)
		{
			createInputParameterChanger(ss, *parameter);
		}
		callClassMethod(ss, obj);
		for (auto& parameter : obj.parameters)
		{
			if (parameter->io == SymbolParameter::IO::OUT)
			{
				createOutputParameterChanger(ss, *parameter);
			}
		}

		if (obj.type->getTypeName() != SymbolType::Name::VOID)
		{
			createReturnValueChanger(ss, obj);
			ss << "return __ret;\n";
		}
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::callClassMethod(SourceStream& ss, const SymbolMethod& obj)
{
	if (obj.type->getTypeName() != SymbolType::Name::VOID)
	{
		ss << "auto __temp_ret = ";
	}
	ss << "ptr->" << obj.name << "(";
	if (!obj.parameters.empty())
	{
		for (auto& parameter : obj.parameters)
		{
			ss << "i_" << parameter->name << ", ";
		}
		ss.pop(2);
	}
	ss << ");\n";
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createStaticMethodDeclaration(SourceStream& ss, const SymbolMethod& obj)
{
	{
		auto parameters = createParameters(obj);
		std::string prefix = "extern " + api_macro;

		MethodCXXSourceScopedStream method_scope{ ss, true, prefix, "", obj.type->toManagedType(), {}, obj.name, parameters };
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createStaticMethodDefinition(SourceStream& ss, const SymbolMethod& obj)
{
	{
		
		auto parameters = createParameters(obj);
		auto scope = createInterfaceScope(root_namespace, obj);

		MethodCXXSourceScopedStream method_scope{ ss, false, "", "", obj.type->toManagedType(), scope, obj.name, parameters };

		for (auto& parameter : obj.parameters)
		{
			createInputParameterChanger(ss, *parameter);
		}
		callStaticMethod(ss, obj);
		for (auto& parameter : obj.parameters)
		{
			if (parameter->io == SymbolParameter::IO::OUT)
			{
				createOutputParameterChanger(ss, *parameter);
			}
		}

		if (obj.type->getTypeName() != SymbolType::Name::VOID)
		{
			createReturnValueChanger(ss, obj);
			ss << "return __ret;\n";
		}
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::callStaticMethod(SourceStream& ss, const SymbolMethod& obj)
{
	if (obj.type->getTypeName() != SymbolType::Name::VOID)
	{
		ss << "auto __temp_ret = ";
	}
	auto scopes = createScope(obj);
	for (auto& scope : scopes)
	{
		ss << scope << "::";
	}

	ss << obj.name << "(";
	if (!obj.parameters.empty())
	{
		for (auto& parameter : obj.parameters)
		{
			ss << "i_" << parameter->name << ", ";
		}
		ss.pop(2);
	}
	ss << ");\n";
}

/*
void LibraryInterfaceGenerator::Implementation::NativeInterface::createParameters(SourceStream& ss, const SymbolMethod& obj)
{
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createParameter(SourceStream& ss, const SymbolParameter& obj)
{
}
*/

void LibraryInterfaceGenerator::Implementation::NativeInterface::findConverter(SourceStream& ss, SymbolType& type)
{
	ss << "Bn3Monkey::";
	switch (type.getTypeName())
	{
	case SymbolType::Name::VOID:
		ss << "MVoid";
		break;
	case SymbolType::Name::BOOL:
		ss << "MBool";
		break;
	case SymbolType::Name::INT8:
		ss << "MInt8";
		break;
	case SymbolType::Name::INT16:
		ss << "MInt16";
		break;
	case SymbolType::Name::INT32:
		ss << "MInt32";
		break;
	case SymbolType::Name::INT64:
		ss << "MInt64";
		break;
	case SymbolType::Name::FLOAT:
		ss << "MFloat";
		break;
	case SymbolType::Name::DOUBLE:
		ss << "MDouble";
		break;
	case SymbolType::Name::STRING:
		ss << "MString";
		break;
	case SymbolType::Name::ENUM:
		ss << "MEnum<"  << type.toNativeType() << ">";
		break;
	case SymbolType::Name::OBJECT:
		{
			auto& object = dynamic_cast<SymbolTypeObject&>(type);
			ss << "MObject<" << object.toNativeName() << ">";
		}
		break;
	case SymbolType::Name::CALLBACK:
	{
		ss << "MCallback<";
		auto types = type.toElementTypes();
		if (!types.empty())
		{
			for (auto& type : type.toElementTypes())
			{
				findConverter(ss, *type);
				ss << ", ";
			}
			ss.pop(2);
		}
		ss << ">";
	}
		break;
	case SymbolType::Name::BOOLARRAY:
	case SymbolType::Name::INT8ARRAY:
	case SymbolType::Name::INT16ARRAY:
	case SymbolType::Name::INT32ARRAY:
	case SymbolType::Name::INT64ARRAY:
	case SymbolType::Name::FLOATARRAY:
	case SymbolType::Name::DOUBLEARRAY:
	case SymbolType::Name::STRINGARRAY:
	case SymbolType::Name::ENUMARRAY:
	case SymbolType::Name::OBJECTARRAY:
	case SymbolType::Name::CALLBACKARRAY:
		ss << "MArray<";
		for (auto& type : type.toElementTypes())
		{
			findConverter(ss, *type);
			ss << ", ";
		}
		ss.pop(2);
		ss << ">";
		break;

	case SymbolType::Name::BOOLVECTOR:
	case SymbolType::Name::INT8VECTOR:
	case SymbolType::Name::INT16VECTOR:
	case SymbolType::Name::INT32VECTOR:
	case SymbolType::Name::INT64VECTOR:
	case SymbolType::Name::FLOATVECTOR:
	case SymbolType::Name::DOUBLEVECTOR:
	case SymbolType::Name::STRINGVECTOR:
	case SymbolType::Name::ENUMVECTOR:
	case SymbolType::Name::OBJECTVECTOR:
	case SymbolType::Name::CALLBACKVECTOR:
		ss << "MVector<";
		for (auto& type : type.toElementTypes())
		{
			findConverter(ss, *type);
			ss << ", ";
		}
		ss.pop(2);
		ss << ">";
		break;
	}
	return;
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createReturnValueChanger(SourceStream& ss, const SymbolMethod& obj)
{
	if (obj.type)
	{
		ss << "auto __ret = ";
		findConverter(ss, *(obj.type));
		ss << "().toManagedType(__temp_ret);\n";
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createInputParameterChanger(SourceStream& ss, const SymbolParameter& obj)
{
	if (obj.type)
	{
		ss << "auto i_" << obj.name << " = ";
		findConverter(ss, *(obj.type));
		ss << "().toNativeType(" << obj.name << ");\n";
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createOutputParameterChanger(SourceStream& ss, const SymbolParameter& obj)
{
	if (obj.io == SymbolParameter::IO::OUT)
	{
		if (obj.type)
		{
			ss << obj.name << " = ";
			findConverter(ss, *(obj.type));
			ss << "().toManagedType(i_" << obj.name << ");\n";
		}
	}
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

void LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertySetterDeclaration(SourceStream& ss, const std::string& property_name, const SymbolProperty& obj)
{
	{
		std::string prefix = "extern " + api_macro;
		std::string name = "set" + property_name;
		auto parameters = createPropertyParameters(obj);

		MethodCXXSourceScopedStream method_scope{ ss, true, prefix, "", "void", {}, name, parameters };
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertyGetterDeclaration(SourceStream& ss, const std::string& property_name, const SymbolProperty& obj)
{
	{
		std::string prefix = "extern " + api_macro;
		std::string name = "get" + property_name;
		auto parameters = createHandleParameter();

		MethodCXXSourceScopedStream method_scope{ ss, true, prefix, "", obj.type->toManagedType(), {}, name, parameters};
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertyDeclaration(SourceStream& ss, const SymbolProperty& obj)
{
	auto property_name = createPropertyName(obj);
	createPropertyGetterDeclaration(ss, property_name, obj);
	if (!obj.readonly)
	{
		createPropertySetterDeclaration(ss, property_name, obj);
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::callPropertySetter(SourceStream& ss, const std::string& property_name, const SymbolClass& clazz, const SymbolProperty& obj)
{
	ss << "auto ptr = Bn3Monkey::MObject<" << clazz.getCppName() << ">().toNativeType(handle);\n";
	ss << "ptr->set" << property_name << "(i_value);\n";
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::callPropertyGetter(SourceStream& ss, const std::string& property_name, const SymbolClass& clazz, const SymbolProperty& obj)
{
	ss << "auto ptr = Bn3Monkey::MObject<" << clazz.getCppName() << ">().toNativeType(handle);\n";
	ss << "auto __temp_ret = ptr->get" << property_name << "();\n";
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertySetterDefinition(SourceStream& ss, const std::string& property_name, const SymbolClass& clazz, const SymbolProperty& obj)
{
	{
		auto parameters = createPropertyParameters(obj);
		auto scopes = createInterfaceScope(root_namespace, clazz);
		std::string name = "set" + property_name;		

		MethodCXXSourceScopedStream method_scope{ ss, false, "", "", "void", scopes, name, parameters };
	
		createInputPropertyChanger(ss, obj);
		callPropertySetter(ss, property_name, clazz, obj);
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertyGetterDefinition(SourceStream& ss, const std::string& property_name, const SymbolClass& clazz, const SymbolProperty& obj)
{
	{
		auto parameters = createHandleParameter();
		auto scopes = createInterfaceScope(root_namespace, clazz);
		std::string name = "get" + property_name;

		MethodCXXSourceScopedStream method_scope{ ss, false, "", "",  obj.type->toManagedType(), scopes, name, parameters };

		callPropertyGetter(ss, property_name, clazz, obj);
		createOutputPropertyChanger(ss, obj);
		ss << "return __ret;\n";
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createPropertyDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolProperty& obj)
{
	auto property_name = createPropertyName(obj);
	createPropertyGetterDefinition(ss, property_name, clazz, obj);
	if (!obj.readonly)
	{
		createPropertySetterDefinition(ss, property_name, clazz, obj);
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createInputPropertyChanger(SourceStream& ss, const SymbolProperty& obj)
{
	if (obj.type)
	{
		ss << "auto i_value = ";
		findConverter(ss, *(obj.type));
		ss << "().toNativeType(value);\n";
	}
}

void LibraryInterfaceGenerator::Implementation::NativeInterface::createOutputPropertyChanger(SourceStream& ss, const SymbolProperty& obj)
{
	if (obj.type)
	{
		ss << "auto __ret = ";
		findConverter(ss, *(obj.type));
		ss << "().toManagedType(__temp_ret);\n";
	}
}

std::string LibraryInterfaceGenerator::Implementation::NativeInterface::createNativeInterfaceConverter()
{
	return "";
}


// Return Value (Create)

// enum : enum class -> int32_t
// object : std::shared_ptr<T> -> (void*)(std::shared_ptr<T>*)

// enum array : std::vector<enum class> -> std::vector<int>
// enum vector : std::vector<enum class> -> std::vector<int>

// object array : std::vector<std::shared_ptr<T>> -> std::vector<void*> + cloneReference
// object vector : ::vector<std::shared_ptr<T>> -> std::vector<void*> + cloneReference


// Input (Create)

// enum : int -> enum class
// object : (void*)(std::shared_ptr<T>*) -> std::shared_ptr<T> 

// enum array : std::vector<int> -> std::vector<enum class> 
// enum vector : std::vector<int> -> std::vector<enum class>

// object array : std::vector<void*> -> std::vector<std::shared_ptr<T>> -> 
// object vector :  std::vector<void*> -> std::vector<std::shared_ptr<T>>


// Output (Copy)

// enum : enum class -> int
// object : std::shared_ptr<T> -> (void*)(std::shared_ptr<T>*)

// enum array : std::vector<enum class> -> std::vector<int>
// enum vector : std::vector<enum class> -> std::vector<int>

// object array : std::vector<std::shared_ptr<T>> -> std::vector<void*> + cloneReference
// object vector : ::vector<std::shared_ptr<T>> -> std::vector<void*> + cloneReference
