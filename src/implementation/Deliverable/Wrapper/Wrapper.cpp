#include "Wrapper.hpp"

#ifdef __linux__
static char* delimeter = "/";
#elif _WIN32
static char* delimeter = "\\";
#else
static char* delimeter = "/";
#endif

LibraryInterfaceGenerator::Implementation::Wrapper::Wrapper(Environment environment, const NativeInterface& interfaceDirectory, const SymbolTable& symbolTable, std::string root_dir_path, const char* directory_name)
	: _infDirectory(interfaceDirectory), _symbolTable(symbolTable)
{
	_wrapper_dir_path = root_dir_path;
	_wrapper_dir_path += delimeter;
	_wrapper_dir_path += directory_name;

	auto& package = _symbolTable.getPackage();

	std::string packageName = package.name;
	std::transform(packageName.begin(), packageName.end(), packageName.begin(), ::tolower);

	_kotlin_package_name = "com." + package.author + "." + packageName;
	_kotlin_wrapper_class_name = package.name + "Wrapper";
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
		ExternalIncludeCXXSourceStream exclude{ ss,  "jni.h" };
	}
	{
		ExternalIncludeCXXSourceStream exclude{ ss,  "android/log.h" };
	}
	{
		std::string interface_header_path = symbolObject.name;
		interface_header_path += "Interface.hpp";
		ExternalIncludeCXXSourceStream exclude{ ss, interface_header_path };
	}

	ss << "\n";

	{
		ss << "using namespace " << _infDirectory.getRootNamespace() << ";\n\n";
	}

	{
		createChangerFunction(ss);
		ss << "\n";
	}


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
	int io;
	/*
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
			io = ParameterNode::VALUE;
		}
	}
	*/
	io = ParameterNode::VALUE;
	return ParameterNode(io, parameter.type->toJNIType(), parameter.name);
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

static ParameterNode createNativeHandleParameters()
{
	return ParameterNode(ParameterNode::VALUE, "jlong", "handle");
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

static std::vector<ParameterNode> createNativeDestructorParameters(const SymbolMethod& object)
{
	auto ret = createJNIParameters();
	ret.push_back(createNativeHandleParameters());
	return ret;
}

static std::vector<ParameterNode> createNativeMemberParameters(const SymbolMethod& object)
{
	auto ret = createJNIParameters();
	ret.push_back(createNativeHandleParameters());
	auto parameters = createNativeParameters(object);
	ret.insert(ret.end(), parameters.begin(), parameters.end());
	return ret;
}

static std::vector<ParameterNode> createNativeCallbackParameters()
{
	std::vector<ParameterNode> ret;
	ret.push_back(ParameterNode(ParameterNode::VALUE, "JNIEnv*", "env"));
	ret.push_back(ParameterNode(ParameterNode::VALUE, "jobject", "thiz"));
	ret.push_back(ParameterNode(ParameterNode::VALUE, "jobject", "instance"));
	ret.push_back(ParameterNode(ParameterNode::VALUE, "jstring", "method_name"));
	return ret;
}

static std::vector<ParameterNode> createNativePropertyGetterParameters(const SymbolProperty& obj)
{
	auto ret = createJNIParameters();
	ret.push_back(createNativeHandleParameters());
	return ret;
}
static std::vector<ParameterNode> createNativePropertySetterParameters(const SymbolProperty& obj)
{
	auto ret = createJNIParameters();
	ret.push_back(createNativeHandleParameters());
	ret.push_back(
		ParameterNode(
			ParameterNode::REFERENCE_IN,
			obj.type->toJNIType(),
			"value")
	);
	return ret;
}

static std::vector<ParameterNode> createJNIInputParameter()
{
	std::vector<ParameterNode> ret;
	ret.push_back(ParameterNode(
		ParameterNode::VALUE, "void*", "i_handle"
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
		if (parameter.io == SymbolParameter::IO::OUT)
		{
			io = ParameterNode::REFERENCE_OUT;
		}
		else
		{
			io = ParameterNode::REFERENCE_IN;
		}
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


static std::vector<ParameterNode> createNativeInputCallbackParameters()
{
	std::vector<ParameterNode> ret;
	ret.push_back(ParameterNode(ParameterNode::VALUE, "void *", "callback"));
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
			createNativeMemberParameters(constructor)
		};

		createNativeHandleChanger(ss);

		for (auto& parameter : constructor.parameters)
		{
			createNativeInputParameterChanger(ss, *parameter);
		}
		callNativeConstructor(ss, clazz, constructor);
		for (auto& parameter : constructor.parameters)
		{
			createNativeOutputParameterChanger(ss, *parameter);
		}
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
			constructor.name,
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
			
		};

		createNativeHandleChanger(ss);

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
			createNativeCallbackParameters()
		};

		createNativeCallbackChanger(ss);
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
			createNativeInputCallbackParameters()
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

		createNativeHandleChanger(ss);

		for (auto& parameter : object.parameters)
		{
			createNativeInputParameterChanger(ss, *parameter);
		}
		callNativeClassMethod(ss, clazz, object);
		for (auto& parameter : object.parameters)
		{
			createNativeOutputParameterChanger(ss, *parameter);
		}
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

		for (auto& parameter : object.parameters)
		{
			createNativeInputParameterChanger(ss, *parameter);
		}
		callNativeStaticMethod(ss, object);
		for (auto& parameter : object.parameters)
		{
			createNativeOutputParameterChanger(ss, *parameter);
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

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeCallbackChanger(SourceStream& ss)
{
	ss << "auto temp = createNativeCallback(env, instance, method_name);\n";
	ss << "auto* callback = (void *)&temp;\n";
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeHandleChanger(SourceStream& ss)
{
	ss << "auto i_handle = (void *)handle;\n";
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeReturnValueChanger(SourceStream& ss, const SymbolMethod& object)
{
	switch (object.type->getTypeName())
	{
	case SymbolType::Name::VOID:
		return;
	case SymbolType::Name::BOOL:
	case SymbolType::Name::INT8:
	case SymbolType::Name::INT16:
	case SymbolType::Name::INT32:
	case SymbolType::Name::INT64:
	case SymbolType::Name::FLOAT:
	case SymbolType::Name::DOUBLE:
	case SymbolType::Name::ENUM:
		ss << "auto __ret = static_cast<" << object.type->toJNIType() << ">(__temp_ret);\n";
		break;
	case SymbolType::Name::OBJECT:
		ss << "auto __ret = (" << object.type->toJNIType() << ")__temp_ret\n";
		break;
	case SymbolType::Name::STRING:
		ss << "auto __ret = createWrapperString(env, __temp_ret);\n";
		break;
	case SymbolType::Name::BOOLARRAY:
		ss << "auto __ret = createWrapperBooleanArray(env, __temp_ret);\n";
		break;
	case SymbolType::Name::INT8ARRAY:
		ss << "auto __ret = createWrapperInt8Array(env, __temp_ret);\n";
		break;
	case SymbolType::Name::INT16ARRAY:
		ss << "auto __ret = createWrapperInt16Array(env, __temp_ret);\n";
		break;
	case SymbolType::Name::INT32ARRAY:
		ss << "auto __ret = createWrapperInt32Array(env, __temp_ret);\n";
		break;
	case SymbolType::Name::INT64ARRAY:
		ss << "auto __ret = createWrapperInt64Array(env, __temp_ret);\n";
		break;
	case SymbolType::Name::FLOATARRAY:
		ss << "auto __ret = createWrapperFloatArray(env, __temp_ret);\n";
		break;
	case SymbolType::Name::DOUBLEARRAY:
		ss << "auto __ret = createWrapperDoubleArray(env, __temp_ret);\n";
		break;
	case SymbolType::Name::STRINGARRAY:
		ss << "auto __ret = createWrapperStringArray(env, __temp_ret);\n";
		break;
	case SymbolType::Name::ENUMARRAY:
		ss << "auto __ret = createWrapperEnumArray(env, __temp_ret);\n";
		break;
	case SymbolType::Name::OBJECTARRAY:
		ss << "auto __ret = createWrapperObjectArray(env, __temp_ret);\n";
		break;
	case SymbolType::Name::BOOLVECTOR:
		ss << "auto __ret = createWrapperBooleanVector(env, __temp_ret);\n";
		break;
	case SymbolType::Name::INT8VECTOR:
		ss << "auto __ret = createWrapperInt8Vector(env, __temp_ret);\n";
		break;
	case SymbolType::Name::INT16VECTOR:
		ss << "auto __ret = createWrapperInt16Vector(env, __temp_ret);\n";
		break;
	case SymbolType::Name::INT32VECTOR:
		ss << "auto __ret = createWrapperInt32Vector(env, __temp_ret);\n";
		break;
	case SymbolType::Name::INT64VECTOR:
		ss << "auto __ret = createWrapperInt64Vector(env, __temp_ret);\n";
		break;
	case SymbolType::Name::FLOATVECTOR:
		ss << "auto __ret = createWrapperFloatVector(env, __temp_ret);\n";
		break;
	case SymbolType::Name::DOUBLEVECTOR:
		ss << "auto __ret = createWrapperDoubleVector(env, __temp_ret);\n";
		break;
	case SymbolType::Name::STRINGVECTOR:
		ss << "auto __ret = createWrapperStringVector(env, __temp_ret);\n";
		break;
	case SymbolType::Name::ENUMVECTOR:
		ss << "auto __ret = createWrapperEnumVector(env, __temp_ret);\n";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ss << "auto __ret = createWrapperObjectVector(env, __temp_ret);\n";
		break;
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeInputParameterChanger(SourceStream& ss, const SymbolParameter& object)
{
	switch (object.type->getTypeName())
	{
	case SymbolType::Name::VOID:
		break;
	case SymbolType::Name::BOOL:
	case SymbolType::Name::INT8:
	case SymbolType::Name::INT16:
	case SymbolType::Name::INT32:
	case SymbolType::Name::INT64:
	case SymbolType::Name::FLOAT:
	case SymbolType::Name::DOUBLE:
	case SymbolType::Name::ENUM:
		ss << "auto i_" << object.name << " = static_cast<" << object.type->toManagedType() << ">(" << object.name << ");\n";
		break;
	case SymbolType::Name::OBJECT:
		ss << "auto i_" << object.name << " = (const " <<  object.type->toManagedType() <<  ")" <<  object.name <<  ";\n";
		break;
	case SymbolType::Name::STRING:
		ss << "auto i_" << object.name << " = createNativeString(env, " << object.name << ");\n";
		break;
	case SymbolType::Name::BOOLARRAY:
		ss << "auto i_" << object.name << " = createNativeBooleanArray(env, " << object.name << ");\n";
		break;
	case SymbolType::Name::INT8ARRAY:
		ss << "auto i_" << object.name << " = createNativeInt8Array(env, " << object.name << ");\n";
		break;
	case SymbolType::Name::INT16ARRAY:
		ss << "auto i_" << object.name << " = createNativeInt16Array(env, " << object.name << ");\n"; 
		break;
	case SymbolType::Name::INT32ARRAY:
		ss << "auto i_" << object.name << " = createNativeInt32Array(env, " << object.name << ");\n";
		break;
	case SymbolType::Name::INT64ARRAY:
		ss << "auto i_" << object.name << " = createNativeInt64Array(env, " << object.name << ");\n";
		break;
	case SymbolType::Name::FLOATARRAY:
		ss << "auto i_" << object.name << " = createNativeFloatArray(env, " << object.name << ");\n";
		break;
	case SymbolType::Name::DOUBLEARRAY:
		ss << "auto i_" << object.name << " = createNativeDoubleArray(env, " << object.name << ");\n";
		break;
	case SymbolType::Name::STRINGARRAY:
		ss << "auto i_" << object.name << " = createNativeStringArray(env, " << object.name << ");\n";
		break;
	case SymbolType::Name::ENUMARRAY:
		ss << "auto i_" << object.name << " = createNativeEnumArray(env, " << object.name << ");\n";
		break;
	case SymbolType::Name::OBJECTARRAY:
		ss << "auto i_" << object.name << " = createNativeObjectArray(env, " << object.name << ");\n";
		break;
	case SymbolType::Name::BOOLVECTOR:
		ss << "auto i_" << object.name << " = createNativeBooleanVector(env, " << object.name << ");\n";
		break;
	case SymbolType::Name::INT8VECTOR:
		ss << "auto i_" << object.name << " = createNativeInt8Vector(env, " << object.name << ");\n";
		break;
	case SymbolType::Name::INT16VECTOR:
		ss << "auto i_" << object.name << " = createNativeInt16Vector(env, " << object.name << ");\n"; 
		break;
	case SymbolType::Name::INT32VECTOR:
		ss << "auto i_" << object.name << " = createNativeInt32Vector(env, " << object.name << ");\n";
		break;
	case SymbolType::Name::INT64VECTOR:
		ss << "auto i_" << object.name << " = createNativeInt64Vector(env, " << object.name << ");\n";
		break;
	case SymbolType::Name::FLOATVECTOR:
		ss << "auto i_" << object.name << " = createNativeFloatVector(env, " << object.name << ");\n";
		break;
	case SymbolType::Name::DOUBLEVECTOR:
		ss << "auto i_" << object.name << " = createNativeDoubleVector(env, " << object.name << ");\n";
		break;
	case SymbolType::Name::STRINGVECTOR:
		ss << "auto i_" << object.name << " = createNativeStringVector(env, " << object.name << ");\n";
		break;
	case SymbolType::Name::ENUMVECTOR:
		ss << "auto i_" << object.name << " = createNativeEnumVector(env, " << object.name << ");\n";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ss << "auto i_" << object.name << " = createNativeObjectVector(env, " << object.name << ");\n";
		break;
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeOutputParameterChanger(SourceStream& ss, const SymbolParameter& object)
{
	switch (object.type->getTypeName())
	{
	case SymbolType::Name::VOID:
		break;
	case SymbolType::Name::BOOL:
	case SymbolType::Name::INT8:
	case SymbolType::Name::INT16:
	case SymbolType::Name::INT32:
	case SymbolType::Name::INT64:
	case SymbolType::Name::FLOAT:
	case SymbolType::Name::DOUBLE:
	case SymbolType::Name::ENUM:
		ss << object.name << " = static_cast<" << object.type->toJNIType() << ">(i_" << object.name << "); // NOT AVAILABLE\n";
		break;
	case SymbolType::Name::OBJECT:
		ss << object.name << " = (" << object.type->toJNIType() << ")i_" << object.name << "; // NOT AVAILABLE\n";
		break;
	case SymbolType::Name::STRING:
		ss << object.name << " = createWrapperString(env, i_" << object.name << "); // NOT AVAILABLE\n";
		break;
	case SymbolType::Name::BOOLARRAY:
		ss << "copyBooleanArray(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::INT8ARRAY:
		ss << "copyInt8Array(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::INT16ARRAY:
		ss << "copyInt16Array(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::INT32ARRAY:
		ss << "copyInt32Array(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::INT64ARRAY:
		ss << "copyInt64Array(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::FLOATARRAY:
		ss << "copyFloatArray(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::DOUBLEARRAY:
		ss << "copyDoubleArray(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::STRINGARRAY:
		ss << "copyStringArray(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::ENUMARRAY:
		ss << "copyEnumArray(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::OBJECTARRAY:
		ss << "copyObjectArray(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::BOOLVECTOR:
		ss << "copyBooleanVector(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::INT8VECTOR:
		ss << "copyInt8Vector(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::INT16VECTOR:
		ss << "copyInt16Vector(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::INT32VECTOR:
		ss << "copyInt32Vector(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::INT64VECTOR:
		ss << "copyInt64Vector(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::FLOATVECTOR:
		ss << "copyFloatVector(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::DOUBLEVECTOR:
		ss << "copyDoubleVector(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::STRINGVECTOR:
		ss << "copyStringVector(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::ENUMVECTOR:
		ss << "copyEnumVector(env, i_" << object.name << "," << object.name << ");\n";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ss << "copyObjectVector(env, i_" << object.name << "," << object.name << ");\n";
		break;
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

		createNativeHandleChanger(ss);
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

		createNativeHandleChanger(ss);
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
	switch (object.type->getTypeName())
	{
	case SymbolType::Name::BOOL:
	case SymbolType::Name::INT8:
	case SymbolType::Name::INT16:
	case SymbolType::Name::INT32:
	case SymbolType::Name::INT64:
	case SymbolType::Name::FLOAT:
	case SymbolType::Name::DOUBLE:
	case SymbolType::Name::ENUM:
		ss << "auto i_value = static_cast<" << object.type->toManagedType() << ">(value);\n";
		break;
	case SymbolType::Name::OBJECT:
		ss << "auto i_value = (const " << object.type->toManagedType() << ")value;\n";
		break;
	case SymbolType::Name::STRING:
		ss << "auto i_value = createNativeString(env, value);\n";
		break;
	case SymbolType::Name::BOOLARRAY:
		ss << "auto i_value = createNativeBooleanArray(env, value);\n";
		break;
	case SymbolType::Name::INT8ARRAY:
		ss << "auto i_value = createNativeInt8Array(env, value);\n";
		break;
	case SymbolType::Name::INT16ARRAY:
		ss << "auto i_value = createNativeInt16Array(env, value);\n";
		break;
	case SymbolType::Name::INT32ARRAY:
		ss << "auto i_value = createNativeInt32Array(env, value);\n";
		break;
	case SymbolType::Name::INT64ARRAY:
		ss << "auto i_value = createNativeInt64Array(env, value);\n";
		break;
	case SymbolType::Name::FLOATARRAY:
		ss << "auto i_value = createNativeFloatArray(env, value);\n";
		break;
	case SymbolType::Name::DOUBLEARRAY:
		ss << "auto i_value = createNativeDoubleArray(env, value);\n";
		break;
	case SymbolType::Name::STRINGARRAY:
		ss << "auto i_value = createNativeStringArray(env, value);\n";
		break;
	case SymbolType::Name::ENUMARRAY:
		ss << "auto i_value = createNativeEnumArray(env, value);\n";
		break;
	case SymbolType::Name::OBJECTARRAY:
		ss << "auto i_value = createNativeObjectArray(env, value);\n";
		break;
	case SymbolType::Name::BOOLVECTOR:
		ss << "auto i_value = createNativeBooleanVector(env, value);\n";
		break;
	case SymbolType::Name::INT8VECTOR:
		ss << "auto i_value = createNativeInt8Vector(env, value);\n";
		break;
	case SymbolType::Name::INT16VECTOR:
		ss << "auto i_value = createNativeInt16Vector(env, value);\n";
		break;
	case SymbolType::Name::INT32VECTOR:
		ss << "auto i_value = createNativeInt32Vector(env, value);\n";
		break;
	case SymbolType::Name::INT64VECTOR:
		ss << "auto i_value = createNativeInt64Vector(env, value);\n";
		break;
	case SymbolType::Name::FLOATVECTOR:
		ss << "auto i_value = createNativeFloatVector(env, value);\n";
		break;
	case SymbolType::Name::DOUBLEVECTOR:
		ss << "auto i_value = createNativeDoubleVector(env, value);\n";
		break;
	case SymbolType::Name::STRINGVECTOR:
		ss << "auto i_value = createNativeStringVector(env, value);\n";
		break;
	case SymbolType::Name::ENUMVECTOR:
		ss << "auto i_value = createNativeEnumVector(env, value);\n";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ss << "auto i_value = createNativeObjectVector(env, value);\n";
		break;
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createNativeOutputPropertyChanger(SourceStream& ss, const SymbolProperty& object)
{
	switch (object.type->getTypeName())
	{
	case SymbolType::Name::BOOL:
	case SymbolType::Name::INT8:
	case SymbolType::Name::INT16:
	case SymbolType::Name::INT32:
	case SymbolType::Name::INT64:
	case SymbolType::Name::FLOAT:
	case SymbolType::Name::DOUBLE:
	case SymbolType::Name::ENUM:
		ss << "auto __ret = static_cast<" << object.type->toJNIType() << ">(__temp_ret);\n";
		break;
	case SymbolType::Name::OBJECT:
		ss << "auto __ret = (" << object.type->toJNIType() << ")__temp_ret;\n";
		break;
	case SymbolType::Name::STRING:
		ss << "auto __ret = createWrapperString(env, __temp_ret);\n";
		break;
	case SymbolType::Name::BOOLARRAY:
		ss << "auto __ret = createWrapperBooleanArray(env, __temp_ret);\n";
		break;
	case SymbolType::Name::INT8ARRAY:
		ss << "auto __ret = createWrapperInt8Array(env, __temp_ret);\n";
		break;
	case SymbolType::Name::INT16ARRAY:
		ss << "auto __ret = createWrapperInt16Array(env, __temp_ret);\n";
		break;
	case SymbolType::Name::INT32ARRAY:
		ss << "auto __ret = createWrapperInt32Array(env, __temp_ret);\n";
		break;
	case SymbolType::Name::INT64ARRAY:
		ss << "auto __ret = createWrapperInt64Array(env, __temp_ret);\n";
		break;
	case SymbolType::Name::FLOATARRAY:
		ss << "auto __ret = createWrapperFloatArray(env, __temp_ret);\n";
		break;
	case SymbolType::Name::DOUBLEARRAY:
		ss << "auto __ret = createWrapperDoubleArray(env, __temp_ret);\n";
		break;
	case SymbolType::Name::STRINGARRAY:
		ss << "auto __ret = createWrapperStringArray(env, __temp_ret);\n";
		break;
	case SymbolType::Name::ENUMARRAY:
		ss << "auto __ret = createWrapperEnumArray(env, __temp_ret);\n";
		break;
	case SymbolType::Name::OBJECTARRAY:
		ss << "auto __ret = createWrapperObjectArray(env, __temp_ret);\n";
		break;
	case SymbolType::Name::BOOLVECTOR:
		ss << "auto __ret = createWrapperBooleanVector(env, __temp_ret);\n";
		break;
	case SymbolType::Name::INT8VECTOR:
		ss << "auto __ret = createWrapperInt8Vector(env, __temp_ret);\n";
		break;
	case SymbolType::Name::INT16VECTOR:
		ss << "auto __ret = createWrapperInt16Vector(env, __temp_ret);\n";
		break;
	case SymbolType::Name::INT32VECTOR:
		ss << "auto __ret = createWrapperInt32Vector(env, __temp_ret);\n";
		break;
	case SymbolType::Name::INT64VECTOR:
		ss << "auto __ret = createWrapperInt64Vector(env, __temp_ret);\n";
		break;
	case SymbolType::Name::FLOATVECTOR:
		ss << "auto __ret = createWrapperFloatVector(env, __temp_ret);\n";
		break;
	case SymbolType::Name::DOUBLEVECTOR:
		ss << "auto __ret = createWrapperDoubleVector(env, __temp_ret);\n";
		break;
	case SymbolType::Name::STRINGVECTOR:
		ss << "auto __ret = createWrapperStringVector(env, __temp_ret);\n";
		break;
	case SymbolType::Name::ENUMVECTOR:
		ss << "auto __ret = createWrapperEnumVector(env, __temp_ret);\n";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ss << "auto __ret = createWrapperObjectVector(env, __temp_ret);\n";
		break;
	}
	ss << "return __ret;\n";
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createChangerFunction(SourceStream& ss)
{
	// bool : bool <-> jboolean
	// int8 : int8_t <-> jbyte
	// int16 : int16_t <-> jshort
	// int32 : int32_t <-> jint
	// int64 : int64_t <-> jlong
	// float : float <-> jfloat
	// double : double <-> jdouble
	// string : std::string <-> jstring
	// enum : int <-> jint
	// object : void* <-> jlong
	// 
	// array<bool>   : std::vector<bool> <-> jbooleanArray
	// array<int8>   : std::vector<int8_t> <-> jbyteArray
	// array<int16>  : std::vector<int16_t> <-> jshortArray
	// array<int32>  : std::vector<int32_t> <-> jintArray
	// array<int64>  : std::vector<int64_t> <-> jlongArray
	// array<float>  : std::vector<float> <-> jfloatArray
	// array<double> : std::vector<double> <-> jdoubleArray
	// array<string> : std::vector<std::string> <-> jobjectArray
	// array<enum>   : std::vector<int> <-> jintArray
	// array<object> : std::vector<void*> <-> jlongArray
	// 
	// vector<bool>   : std::vector<bool> <-> java/util/ArrayList & java/lang/Boolean
	// vector<int8>   : std::vector<int8_t> <-> java/util/ArrayList & java/lang/Byte
	// vector<int16>  : std::vector<int16_t> <-> java/util/ArrayList & java/lang/Short
	// vector<int32>  : std::vector<int32_t> <-> java/util/ArrayList & java/lang/Integer
	// vector<int64>  : std::vector<int64_t> <-> java/util/ArrayList & java/lang/Long
	// vector<float>  : std::vector<float> <-> java/util/ArrayList & java/lang/Float
	// vector<double> : std::vector<double> <-> java/util/ArrayList & java/lang/Double
	// vector<string> : std::vector<std::string> <-> java/util/ArrayList & java/lang/String
	// vector<enum>   : std::vector<int> <-> java/util/ArrayList & java/lang/Integer
	// vector<object> : std::vector<void*> <-> java/util/ArrayList & java/lang/Long 

	ss << "";
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
		if (parameter.io == SymbolParameter::IO::OUT)
		{
			io = ParameterNode::REFERENCE_OUT;
		}
		else
		{
			io = ParameterNode::REFERENCE_IN;
		}
	}
	return ParameterNode(io, parameter.type->toKotlinType(), parameter.name);
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
	ret.push_back(ParameterNode(ParameterNode::VALUE, "Long", "handle"));
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
		ParameterNode( ParameterNode::REFERENCE_IN, obj.type->toKotlinType(), "value")
	);
	return ret;
}

static std::vector<ParameterNode> createWrapperCallbackParameters()
{
	std::vector<ParameterNode> ret;
	ret.push_back(ParameterNode(ParameterNode::VALUE, "Any", "instance"));
	ret.push_back(ParameterNode(ParameterNode::VALUE, "String", "method_name"));
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
			"Long",
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
			createWrapperCallbackParameters()
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
			object.type->toKotlinType(),
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
			object.type->toKotlinType(),
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
			object.type->toKotlinType(),
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
