#include "Wrapper.hpp"

#ifdef __linux__
static char* delimeter = "/";
#elif _WIN32
static char* delimeter = "\\";
#else
static char* delimeter = "/";
#endif

LibraryInterfaceGenerator::Implementation::Wrapper::Wrapper(Environment environment, const NativeExternalLibraryDirectory& libDirectory,
	const KotlinExternalLibraryDirectory& klibDirectory, const NativeInterface& interfaceDirectory, const SymbolTable& symbolTable, std::string root_dir_path, const char* directory_name)
	: _libDirectory(libDirectory), _klibDirectory(klibDirectory), _infDirectory(interfaceDirectory), _symbolTable(symbolTable)
{
	_wrapper_dir_path = root_dir_path;
	_wrapper_dir_path += delimeter;
	_wrapper_dir_path += directory_name;

	auto& package = _symbolTable.getPackage();

	std::string packageName = package.name;
	std::transform(packageName.begin(), packageName.end(), packageName.begin(), ::tolower);

	_kotlin_package_name = "com." + package.author + "." + packageName;
	_kotlin_wrapper_class_name = package.name + "Wrapper";
	_kotlin_class_prefix = "com/" + package.author + "/" + packageName;
}

LibraryInterfaceGenerator::Implementation::Wrapper::~Wrapper()
{
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::Wrapper::make()
{
	auto& package = _symbolTable.getPackage();

	Result result{ Result::Code::SUCCESS };

	result = FileSystem::createDirectory(_wrapper_dir_path);
	if (!result)
		return result;

	result = createWrapperFile(package, _wrapper_dir_path);
	if (!result)
		return result;

	return result;
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperFile(const SymbolPackage& symbolObject, std::string& parent_include_path)
{
	std::string native_path{ parent_include_path };
	native_path += delimeter;
	native_path += symbolObject.name;
	native_path += "Wrapper.cpp";

	std::string target_path{ parent_include_path };
	target_path += delimeter;
	target_path += symbolObject.name;
	target_path += "Wrapper.kt";

	Result result;
	auto native_content = createNativeWrapperContent(symbolObject);
	auto target_content = createKotlinWrapperContent(symbolObject);


	result = FileSystem::createFile(native_path, native_content.str());
	if (!result)
		return result;

	result = FileSystem::createFile(target_path, target_content.str());
	if (!result)
		return result;

	return Result();
}

LibraryInterfaceGenerator::Implementation::SourceStream LibraryInterfaceGenerator::Implementation::Wrapper::createNativeWrapperContent(const SymbolPackage& symbolObject)
{
	SourceStream ss;

	{
		ExternalIncludeCXXSourceStream exclude{ss,  "string" };
	}
	{
		ExternalIncludeCXXSourceStream exclude{ ss,  "vector" };
	}
	{
		ExternalIncludeCXXSourceStream exclude{ ss, "functional" };
	}
	{
		ExternalIncludeCXXSourceStream exclude{ ss,  "jni.h" };
	}
	{
		ExternalIncludeCXXSourceStream exclude{ ss,  "android/log.h" };
	}
	{
		if (_libDirectory.existsExternalTool(NativeExternalLibraryDirectory::ExternalTool::KotlinTypeConverter))
		{
			auto poolPath = _libDirectory.getRelativeHeaderPath(NativeExternalLibraryDirectory::ExternalTool::KotlinTypeConverter);
			InternalIncludeCXXSourceStream poolInclude{ ss, poolPath };
		}
	}
	{
		std::string interface_header_path = symbolObject.name;
		interface_header_path += "Interface.hpp";
		ExternalIncludeCXXSourceStream exclude{ ss, interface_header_path };
	}

	ss << "\n";

	createNativePackageDefinition(ss, symbolObject);

	return ss;
}

LibraryInterfaceGenerator::Implementation::SourceStream LibraryInterfaceGenerator::Implementation::Wrapper::createKotlinWrapperContent(const SymbolPackage& symbolObject)
{
	SourceStream ss;

	{
		PackageKotlinSourceStream package(ss, _kotlin_package_name, {});
	}
	{
		ImportKotlinSourceStream autoClosableImport(ss, "java.lang.AutoCloseable");
	}

	{
		ss << _klibDirectory.createExternalToolCode(KotlinExternalLibraryDirectory::ExternalTool::KotlinTypeConverter);
	}
	
	{
		ClassKotlinSourceScopedStream wrapper_class{ ss, _kotlin_wrapper_class_name, {} };
		{
			CompanionObjectKotlinSourceScopedStream companion_object{ ss };
			{
				CompanionObjectKotlinSourceScopedStream::Init init{ ss };
				init.loadLibrary(_kotlin_wrapper_class_name);
			}
			companion_object.addSingleTon(_kotlin_wrapper_class_name);
		}

		createWrapperPackageDeclaration(ss, symbolObject);
	}


	return ss;
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativePackageDefinition(SourceStream& ss, const SymbolPackage& symbolObject)
{
	std::string package_name{ symbolObject.name };
	std::transform(package_name.begin(), package_name.end(), package_name.begin(), ::tolower);

	std::stringstream prefix_ss;
	prefix_ss << "Java_com_" << symbolObject.author << "_" << package_name << "_" << _kotlin_wrapper_class_name << "_";
	auto prefix = prefix_ss.str();

	for (auto& mod : symbolObject.modules)
	{
		createNativeModuleDefinition(ss, prefix, *mod);
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeModuleDefinition(SourceStream& ss, const std::string& prefix, const SymbolModule& mod)
{
	for (auto& global_method : mod.global_methods)
	{
		auto& method = global_method.first;
		auto& method_count = global_method.second;

		createNativeStaticMethodDefinition(ss, prefix, *method, method_count);
	}
	/*
	for (auto& inf : mod.interfaces)
	{
		createNativeClassDefinition(ss, prefix, *inf);
	}
	*/
	for (auto& clazz : mod.classes)
	{
		createNativeClassDefinition(ss, prefix, *clazz);
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeClassDefinition(SourceStream& ss, const std::string& prefix, const SymbolClass& clazz)
{
	for (auto& constructor : clazz.constructors)
	{
		auto& method = constructor.first;
		auto& method_count = constructor.second;

		createNativeConstructorDefinition(ss, prefix, clazz, *method, method_count);
	}

	createNativeDestructorDefinition(ss, prefix, clazz);
	createNativeAddReleaserDefinition(ss, prefix, clazz);

	auto& base_member_methods = clazz.getBaseMethods();
	for (auto& base_member_method : base_member_methods)
	{
		auto& method = base_member_method.first;
		auto& method_count = base_member_method.second;
		createNativeClassMethodDefinition(ss, prefix, clazz, *method, method_count);
	}

	auto& member_methods = clazz.methods;
	for (auto& member_method : member_methods)
	{
		auto& method = member_method.first;
		auto& method_count = member_method.second;

		createNativeClassMethodDefinition(ss, prefix, clazz, *method, method_count);
	}

	auto& base_properties = clazz.getBaseProperties();
	for (auto& base_prop : base_properties)
	{
		createNativePropertyDefinition(ss, prefix, clazz, *base_prop);
	}

	auto& properties = clazz.properties;
	for (auto& prop : properties)
	{
		createNativePropertyDefinition(ss, prefix, clazz, *prop);
	}
}

using namespace LibraryInterfaceGenerator::Implementation;

static ParameterNode createNativeParameter(const SymbolParameter& parameter)
{
	return ParameterNode(ParameterNode::VALUE, parameter.type->toJNIType(), parameter.name);
}

static std::vector<ParameterNode> createNativeParameters(const SymbolMethod& object)
{
	std::vector<ParameterNode> ret;
	for (auto& parameter : object.parameters)
	{
		ret.push_back(createNativeParameter(*parameter));
	}
	return ret;
}

static std::vector<ParameterNode> createJNIParameters()
{
	std::vector<ParameterNode> ret;
	ret.push_back(ParameterNode(ParameterNode::VALUE, "JNIEnv*", "env"));
	ret.push_back(ParameterNode(ParameterNode::VALUE, "jobject", "thiz"));
	return ret;
}

static ParameterNode createNativeSelfParameters()
{
	return ParameterNode(ParameterNode::VALUE, "jlong", "self");
}

static std::vector<ParameterNode> createNativeStaticParameters(const SymbolMethod& object)
{
	auto ret = createJNIParameters();
	auto parameters = createNativeParameters(object);
	ret.insert(ret.end(), parameters.begin(), parameters.end());
	return ret;
}

static std::vector<ParameterNode> createNativeConstructorParameters(const SymbolMethod& object)
{
	return createNativeStaticParameters(object);
}

static std::vector<ParameterNode> createNativeDestructorParameters()
{
	auto ret = createJNIParameters();
	ret.push_back(createNativeSelfParameters());
	return ret;
}

static std::vector<ParameterNode> createNativeMemberParameters(const SymbolMethod& object)
{
	auto ret = createJNIParameters();
	ret.push_back(createNativeSelfParameters());
	auto parameters = createNativeParameters(object);
	ret.insert(ret.end(), parameters.begin(), parameters.end());
	return ret;
}

static std::vector<ParameterNode> createNativeReleaserParameters()
{
	auto ret = createJNIParameters();
	ret.push_back(ParameterNode(ParameterNode::VALUE, "jobject", "releaser"));
	return ret;
}

static std::vector<ParameterNode> createNativePropertyGetterParameters(const SymbolProperty& obj)
{
	auto ret = createJNIParameters();
	ret.push_back(createNativeSelfParameters());
	return ret;
}
static std::vector<ParameterNode> createNativePropertySetterParameters(const SymbolProperty& obj)
{
	auto ret = createJNIParameters();
	ret.push_back(createNativeSelfParameters());
	ret.push_back(
		ParameterNode(
			ParameterNode::VALUE,
			obj.type->toJNIType(),
			"value")
	);
	return ret;
}

static std::vector<ParameterNode> createJNIInputParameter()
{
	std::vector<ParameterNode> ret;
	ret.push_back(ParameterNode(
		ParameterNode::VALUE, "void*", "i_self"
	));
	return ret;
}
static ParameterNode createNativeInputParameter(const SymbolParameter& parameter)
{
	int io;
	if (parameter.type->isPrimitive())
	{
		io = ParameterNode::VALUE;
	}
	else
	{
		io = ParameterNode::REFERENCE_IN;
	}
	return ParameterNode(io, parameter.type->toJNIType(), "i_" + parameter.name);
}


static std::vector<ParameterNode> createNativeInputStaticParameters(const SymbolMethod& object)
{
	std::vector<ParameterNode> ret;
	for (auto& parameter : object.parameters)
	{
		ret.push_back(createNativeInputParameter(*parameter));
	}
	return ret;
}

static std::vector<ParameterNode> createNativeInputConstructorParameters(const SymbolMethod& object)
{
	return createNativeInputStaticParameters(object);
}

static std::vector<ParameterNode> createNativeInputDestructorParameters()
{
	return createJNIInputParameter();
}

static std::vector<ParameterNode> createNativeInputMemberParameters(const SymbolMethod& object)
{
	std::vector<ParameterNode> ret = createJNIInputParameter();
	auto params = createNativeInputStaticParameters(object);
	ret.insert(ret.end(), params.begin(), params.end());
	return ret;
}


static std::vector<ParameterNode> createNativeInputPropertyParameters(const SymbolProperty& obj)
{
	std::vector<ParameterNode> ret = createJNIInputParameter();
	ret.push_back(
		ParameterNode(
			ParameterNode::VALUE,
			obj.type->toManagedType(),
			"i_value"
		)
	);
	return ret;
}

static std::vector<ParameterNode> createNativeInputPropertyGetterParameters(const SymbolProperty& obj)
{
	std::vector<ParameterNode> ret = createJNIInputParameter();
	return ret;
}
static std::vector<ParameterNode> createNativeInputPropertySetterParameters(const SymbolProperty& obj)
{
	std::vector<ParameterNode> ret = createJNIInputParameter();
	ret.push_back(
		ParameterNode(
			ParameterNode::REFERENCE_IN,
			obj.type->toJNIType(),
			"i_value")
	);
	return ret;
}

static std::vector<ParameterNode> createNativeInputReleaserParameters()
{
	std::vector<ParameterNode> ret;
	ret.push_back(
		ParameterNode(
			ParameterNode::REFERENCE_IN,
			"std::function<void()>",
			"i_releaser"
		)
	);
	return ret;
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeConstructorDefinition(SourceStream& ss, const std::string& prefix, const SymbolClass& clazz, const SymbolMethod& constructor, int number)
{
	{
		std::string method_name{ prefix };
		method_name += createNativeWrapperScope(clazz);
		method_name += "construct";
		if (number != 0)
			method_name += std::to_string(number);

		MethodCXXSourceScopedStream method_scope{
			ss,
			false,
			JNIEXPORT,
			"",
			"jlong",
			{ },
			method_name,
			createNativeConstructorParameters(constructor)
		};

		for (auto& parameter : constructor.parameters)
		{
			createNativeInputParameterChanger(ss, *parameter);
		}
		callNativeConstructor(ss, clazz, constructor);
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::callNativeConstructor(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& constructor)
{
	{
		auto root_namespace = _infDirectory.getRootNamespace();
		ss << "return ";
		CallCXXSourceScopedStream call{
			ss,
			"jlong",
			createInterfaceScope(root_namespace, clazz),
			"construct",
			createNativeInputConstructorParameters(constructor)
		};
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeDestructorDefinition(SourceStream& ss, const std::string& prefix, const SymbolClass& clazz)
{
	{
		std::string method_name{ prefix };
		method_name += createNativeWrapperScope(clazz);
		method_name += "release";

		MethodCXXSourceScopedStream method_scope{
			ss,
			false,
			JNIEXPORT,
			"",
			"void",
			{ },
			method_name,
			createNativeDestructorParameters()
		};

		createNativeHandleChanger(ss, clazz);

		callNativeDesturctor(ss, clazz);

	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::callNativeDesturctor(SourceStream& ss, const SymbolClass& clazz)
{
	{
		auto root_namespace = _infDirectory.getRootNamespace();
		CallCXXSourceScopedStream call{
			ss,
			"",
			createInterfaceScope(root_namespace, clazz),
			"release",
			createNativeInputDestructorParameters()
		};
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeAddReleaserDefinition(SourceStream& ss, const std::string& prefix, const SymbolClass& clazz)
{
	{
		std::string method_name{ prefix };
		method_name += createNativeWrapperScope(clazz);
		method_name += "addReleaser";

		MethodCXXSourceScopedStream method_scope{
			ss,
			false,
			JNIEXPORT,
			"",
			"void",
			{},
			method_name,
			createNativeReleaserParameters()
		};

		createNativeReleaserChanger(ss);
		callNativeAddReleaser(ss, clazz);
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::callNativeAddReleaser(SourceStream& ss, const SymbolClass& clazz)
{
	{
		auto root_namespace = _infDirectory.getRootNamespace();

		CallCXXSourceScopedStream call{
			ss,
			"",
			createInterfaceScope(root_namespace, clazz),
			"addReleaser",
			createNativeInputReleaserParameters()
		};
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeClassMethodDefinition(SourceStream& ss, const std::string& prefix, const SymbolClass& clazz, const SymbolMethod& object, int number)
{
	{
		std::string method_name{ prefix };
		method_name += createNativeWrapperScope(clazz);
		method_name += object.name;
		if (number != 0)
			method_name += std::to_string(number);

		MethodCXXSourceScopedStream method_scope{
			ss,
			false,
			JNIEXPORT,
			"",
			object.type->toJNIType(),
			{ },
			method_name,
			createNativeMemberParameters(object)
		};

		createNativeHandleChanger(ss, clazz);

		for (auto& parameter : object.parameters)
		{
			createNativeInputParameterChanger(ss, *parameter);
		}
		callNativeClassMethod(ss, clazz, object);

		createNativeReturnValueChanger(ss, object);
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::callNativeClassMethod(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object)
{
	if (object.type->getTypeName() != SymbolType::Name::VOID)
		ss << "auto __temp_ret = ";
	{
		auto root_namespace = _infDirectory.getRootNamespace();
		CallCXXSourceScopedStream call{
			ss,
			"",
			createInterfaceScope(root_namespace, clazz),
			object.name,
			createNativeInputMemberParameters(object)
		};
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeStaticMethodDefinition(SourceStream& ss, const std::string& prefix, const SymbolMethod& object, int number)
{
	{
		std::string method_name{ prefix };
		method_name += createNativeWrapperScope(object);
		method_name += object.name;
		if (number != 0)
			method_name += std::to_string(number);

		MethodCXXSourceScopedStream method_scope{
			ss,
			false,
			JNIEXPORT,
			"",
			object.type->toJNIType(),
			{ },
			method_name,
			createNativeStaticParameters(object)
		};

		if (object.name == "initialize")
		{
			ss << "if (!Bn3Monkey::Kotlin::KotlinTypeConverter::initialize(env))\n";
			ss << "\treturn 0L;\n";
		}

		for (auto& parameter : object.parameters)
		{
			createNativeInputParameterChanger(ss, *parameter);
		}
		callNativeStaticMethod(ss, object);
		
		if (object.name == "release")
		{
			ss << "Bn3Monkey::Kotlin::KotlinTypeConverter::release(env);\n";
		}

		createNativeReturnValueChanger(ss, object);
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::callNativeStaticMethod(SourceStream& ss, const SymbolMethod& object)
{
	if (object.type->getTypeName() != SymbolType::Name::VOID)
		ss << "auto __temp_ret = ";
	{
		auto root_namespace = _infDirectory.getRootNamespace();
		CallCXXSourceScopedStream call{
			ss,
			"",
			createInterfaceScope(root_namespace, object),
			object.name,
			createNativeInputStaticParameters(object)
		};
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::findConverter(SourceStream& ss, SymbolType& type)
{
	ss << "Bn3Monkey::Kotlin::";
	switch (type.getTypeName())
	{
	case SymbolType::Name::VOID:
		ss << "KVoid";
		break;
	case SymbolType::Name::BOOL:
		ss << "KBoolean";
		break;
	case SymbolType::Name::INT8:
		ss << "KInt8";
		break;
	case SymbolType::Name::INT16:
		ss << "KInt16";
		break;
	case SymbolType::Name::INT32:
		ss << "KInt32";
		break;
	case SymbolType::Name::INT64:
		ss << "KInt64";
		break;
	case SymbolType::Name::FLOAT:
		ss << "KFloat";
		break;
	case SymbolType::Name::DOUBLE:
		ss << "KDouble";
		break;
	case SymbolType::Name::STRING:
		ss << "KString";
		break;
	case SymbolType::Name::ENUM:
		ss << "KEnum";
		break;
	case SymbolType::Name::OBJECT:	
		ss << "KObject";
		break;
	case SymbolType::Name::CALLBACK:
	{
		ss << "KCallback<";
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
		ss << "KArray<";
		for (auto& type : type.toElementTypes())
		{
			findConverter(ss, *type);
			ss << ", ";
		}
		{
			// ss.pop(2);
			auto* arrayType = reinterpret_cast<SymbolTypeBaseArray*>(&type);
			auto size = arrayType->length();
			std::stringstream tt;
			tt << size;
			ss << tt.str();
			ss << ">";
		}
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
		ss << "KVector<";
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

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeReleaserChanger(SourceStream& ss)
{
	ss << "auto i_releaser = Bn3Monkey::Kotlin::KCallback<Bn3Monkey::Kotlin::KVoid>().toManagedType(env, releaser);\n";
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeHandleChanger(SourceStream& ss, const SymbolClass& clazz)
{
	ss << "auto i_self = Bn3Monkey::Kotlin::KObject().toManagedType(env, self);\n";
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeReturnValueChanger(SourceStream& ss, const SymbolMethod& object)
{
	if (object.type)
	{
		if (object.type->getTypeName() != SymbolType::Name::VOID)
		{
			ss << "auto __ret = ";
			findConverter(ss, *(object.type));
			ss << "().toKotlinType(env, __temp_ret);\n";
			ss << "return __ret;\n";
		}
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeInputParameterChanger(SourceStream& ss, const SymbolParameter& object)
{
	if (object.type)
	{
		ss << "auto i_" << object.name << " = ";
		findConverter(ss, *(object.type));
		ss << "().toManagedType(env, " << object.name << ");\n";
	}
}


std::string LibraryInterfaceGenerator::Implementation::Wrapper::createPropertyName(const SymbolProperty& object)
{
	std::string propertyName = object.name;
	char firstChar = propertyName[0];
	if ('a' <= firstChar && firstChar <= 'z')
		firstChar += ('A' - 'a');
	propertyName[0] = firstChar;
	return propertyName;
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativePropertySetterDeclaration(SourceStream& ss, const std::string& prefix, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object)
{
	{
		std::string method_name{ prefix };
		method_name += createNativeWrapperScope(clazz);
		method_name += "set";
		method_name += propertyName;

		MethodCXXSourceScopedStream method_scope{
			ss,
			false,
			JNIEXPORT,
			"",
			"void",
			{  },
			method_name,
			createNativePropertySetterParameters(object)
		};

		createNativeHandleChanger(ss, clazz);
		createNativeInputPropertyChanger(ss, object);
		callNativePropertySetter(ss, propertyName, clazz, object);
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativePropertyGetterDeclaration(SourceStream& ss, const std::string& prefix, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object)
{
	{
		std::string method_name{ prefix };
		method_name += createNativeWrapperScope(clazz);
		method_name += "get";
		method_name += propertyName;

		MethodCXXSourceScopedStream method_scope{
			ss,
			false,
			JNIEXPORT,
			"",
			object.type->toJNIType(),
			{  },
			method_name,
			createNativePropertyGetterParameters(object)
		};

		createNativeHandleChanger(ss, clazz);
		callNativePropertyGetter(ss, propertyName, clazz, object);
		createNativeOutputPropertyChanger(ss, object);
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::callNativePropertySetter(SourceStream& ss, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object)
{
	{
		auto root_namespace = _infDirectory.getRootNamespace();
		CallCXXSourceScopedStream call(
			ss,
			"",
			createInterfaceScope(root_namespace, clazz),
			"set" + propertyName,
			createNativeInputPropertySetterParameters(object)
		);
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::callNativePropertyGetter(SourceStream& ss, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object)
{
	{
		auto root_namespace = _infDirectory.getRootNamespace();
		ss << "auto __temp_ret = ";
		CallCXXSourceScopedStream call(
			ss,
			"",
			createInterfaceScope(root_namespace, clazz),
			"get" + propertyName,
			createNativeInputPropertyGetterParameters(object)
		);
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativePropertyDefinition(SourceStream& ss, const std::string& prefix, const SymbolClass& clazz, const SymbolProperty& object)
{
	auto propertyName = createPropertyName(object);
	createNativePropertyGetterDeclaration(ss, prefix, propertyName, clazz, object);
	if (object.readonly == false)
	{
		createNativePropertySetterDeclaration(ss, prefix, propertyName, clazz, object);
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeInputPropertyChanger(SourceStream& ss, const SymbolProperty& object)
{
	if (object.type)
	{
		ss << "auto i_value = ";
		findConverter(ss, *(object.type));
		ss << "().toManagedType(env, value);\n";
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeOutputPropertyChanger(SourceStream& ss, const SymbolProperty& object)
{
	if (object.type)
	{
		ss << "auto ret = ";
		findConverter(ss, *(object.type));
		ss << "().toKotlinType(env, __temp_ret);\n";
		ss << "return ret;\n";
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperPackageDeclaration(SourceStream& ss, const SymbolPackage& symbolObject)
{
	for (auto& mod : symbolObject.modules)
	{
		createWrapperModuleDeclaration(ss, *mod);
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperModuleDeclaration(SourceStream& ss, const SymbolModule& mod)
{
	for (auto& global_method : mod.global_methods)
	{
		auto& method = global_method.first;
		auto method_count = global_method.second;
		createWrapperStaticMethodDeclaration(ss, *method, method_count);
	}

	for (auto& clazz : mod.classes)
	{
		createWrapperClassDeclaration(ss, *clazz);
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperClassDeclaration(SourceStream& ss, const SymbolClass& clazz)
{
	for (auto& constructor : clazz.constructors)
	{
		auto& method = constructor.first;
		auto& method_count = constructor.second;
		createWrapperConstructorDeclaration(ss, clazz, *method, method_count);
	}

	createWrapperDestructorDeclaration(ss, clazz);
	createWrapperAddReleaserDeclaration(ss, clazz);

	auto& base_methods = clazz.getBaseMethods();
	for (auto& base_method : base_methods)
	{
		auto& method = base_method.first;
		auto method_count = base_method.second;
		createWrapperClassMethodDeclaration(ss, clazz, *method, method_count);
	}

	auto& member_methods = clazz.methods;
	for (auto& member_method : member_methods)
	{
		auto& method = member_method.first;
		auto method_count = member_method.second;
		createWrapperClassMethodDeclaration(ss, clazz, *method, method_count);
	}

	auto& base_properties = clazz.getBaseProperties();
	for (auto& prop : base_properties)
	{
		createWrapperPropertyDeclaration(ss, clazz, *prop);
	}

	auto& properties = clazz.properties;
	for (auto& prop : properties)
	{
		createWrapperPropertyDeclaration(ss, clazz, *prop);
	}
}

static ParameterNode createWrapperParameter(const SymbolParameter& parameter)
{
	int io;
	if (parameter.type->isPrimitive())
	{
		io = ParameterNode::VALUE;
	}
	else
	{
		io = ParameterNode::REFERENCE_IN;
	}
	return ParameterNode(io, parameter.type->toKotlinWrapperType(), parameter.name);
}

static std::vector<ParameterNode> createWrapperParameters(const SymbolMethod& object)
{
	std::vector<ParameterNode> ret;
	for (auto& parameter : object.parameters)
	{
		ret.push_back(createWrapperParameter(*parameter));
	}
	return ret;
}

static std::vector<ParameterNode> createWrapperHandleParameters()
{
	std::vector<ParameterNode> ret;
	ret.push_back(ParameterNode(ParameterNode::VALUE, "Long", "self"));
	return ret;
}

static std::vector<ParameterNode> createWrapperMemberParameters(const SymbolMethod& object)
{
	auto ret = createWrapperHandleParameters();
	auto parameters = createWrapperParameters(object);
	ret.insert(ret.end(), parameters.begin(), parameters.end());
	return ret;
}

static std::vector<ParameterNode> createWrapperPropertyParameters(const SymbolProperty& obj)
{
	auto ret = createWrapperHandleParameters();
	ret.push_back(
		ParameterNode( ParameterNode::REFERENCE_IN, obj.type->toKotlinWrapperType(), "value")
	);
	return ret;
}

static std::vector<ParameterNode> createWrapperReleaserParameters()
{
	std::vector<ParameterNode> ret;
	ret.push_back(ParameterNode(ParameterNode::VALUE, "(() -> Unit)", "releaser"));
	return ret;
}


void LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperConstructorDeclaration(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& constructor, int number)
{
	{
		std::string method_name{ createKotlinWrapperScope(clazz) };
		method_name += "construct";
		if (number > 0)
		{
			method_name += std::to_string(number);
		}

		MethodKotlinSourceScopedStream method{
			ss,
			false,
			KotlinAccess::EXTERNAL,
			"",
			"",
			"Long",
			method_name,
			createWrapperParameters(constructor)
		};
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperDestructorDeclaration(SourceStream& ss, const SymbolClass& clazz)
{
	{
		std::string method_name{ createKotlinWrapperScope(clazz) };
		method_name += "release";

		MethodKotlinSourceScopedStream method{
			ss,
			false,
			KotlinAccess::EXTERNAL,
			"",
			"",
			"",
			method_name,
			createWrapperHandleParameters()
		};
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperAddReleaserDeclaration(SourceStream& ss, const SymbolClass& clazz)
{
	{
		std::string method_name{ createKotlinWrapperScope(clazz) };
		method_name += "addReleaser";

		MethodKotlinSourceScopedStream method{
			ss,
			false,
			KotlinAccess::EXTERNAL,
			"",
			"",
			"",
			method_name,
			createWrapperReleaserParameters()
		};
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperClassMethodDeclaration(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object, int number)
{
	{
		std::string method_name{ createKotlinWrapperScope(clazz) };
		method_name += object.name;
		if (number > 0)
		{
			method_name += std::to_string(number);
		}

		MethodKotlinSourceScopedStream method{
			ss,
			false,
			KotlinAccess::EXTERNAL,
			"",
			"",
			object.type->toKotlinWrapperType(),
			method_name,
			createWrapperMemberParameters(object)
		};
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperStaticMethodDeclaration(SourceStream& ss, const SymbolMethod& object, int number)
{
	{
		std::string method_name{ createKotlinWrapperScope(object) };
		method_name += object.name;
		if (number > 0)
		{
			method_name += std::to_string(number);
		}

		MethodKotlinSourceScopedStream method{
			ss,
			false,
			KotlinAccess::EXTERNAL,
			"",
			"",
			object.type->toKotlinWrapperType(),
			method_name,
			createWrapperParameters(object)
		};
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperPropertySetterDeclaration(SourceStream& ss, const std::string& property_name, const SymbolClass& clazz, const SymbolProperty& object)
{
	{
		std::string method_name{ createKotlinWrapperScope(clazz) };
		method_name += "set";
		method_name += property_name;

		MethodKotlinSourceScopedStream method(
			ss,
			false,
			KotlinAccess::EXTERNAL,
			"",
			"",
			"",
			method_name,
			createWrapperPropertyParameters(object)
		);
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperPropertyGetterDeclaration(SourceStream& ss, const std::string& property_name, const SymbolClass& clazz, const SymbolProperty& object)
{
	{
		std::string method_name{ createKotlinWrapperScope(clazz) };
		method_name += "get";
		method_name += property_name;

		MethodKotlinSourceScopedStream method{
			ss,
			false,
			KotlinAccess::EXTERNAL,
			"",
			"",
			object.type->toKotlinWrapperType(),
			method_name,
			createWrapperHandleParameters()
		};
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperPropertyDeclaration(SourceStream& ss, const SymbolClass& clazz, const SymbolProperty& object)
{
	auto propertyName = createPropertyName(object);
	createWrapperPropertyGetterDeclaration(ss, propertyName, clazz, object);
	if (object.readonly == false)
	{
		createWrapperPropertySetterDeclaration(ss, propertyName, clazz, object);
	}
}
