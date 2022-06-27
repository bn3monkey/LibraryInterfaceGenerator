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
	std::string native_content;
	std::string target_content;
	result = createWrapperContent(symbolObject, native_content, target_content);
	if (!result)
		return result;

	result = FileSystem::createFile(native_path, native_content);
	if (!result)
		return result;

	result = FileSystem::createFile(target_path, target_content);
	if (!result)
		return result;

	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperContent(const SymbolPackage& symbolObject, std::string& header_content, std::string& cpp_content)
{
	std::stringstream ss{""};
	std::string indent{ "" };

	{
		DefineInclude defineInclude{ ss, indent };
		defineInclude.addExternal("string");
		defineInclude.addExternal("vector");
		defineInclude.addExternal("jni.h");
		defineInclude.addExternal("android/log.h");

		std::string interfaceHeaderPath = symbolObject.name;
		interfaceHeaderPath += "Interface.hpp";
		defineInclude.addExternal(interfaceHeaderPath);
	}

	ss << "\n";

	{
		ss << "using namespace " << _infDirectory.getRootNamespace() << ";\n\n";
	}
	{
		ss << createChangerFunction() << "\n\n";
	}
	createPackageDefinition(symbolObject, ss);
	header_content = ss.str();



	ss.str("");

	{
		ss << "package " << _kotlin_package_name << ";\n\n";
	}

	{
		std::string classStart = "class " + _kotlin_wrapper_class_name;
		DefineObject defineObject(ss, classStart, indent);
		{
			std::string companionObject{ "companion object" };
			DefineObject defineObject(ss, companionObject, indent);

			{
				std::string init{ "init" };
				DefineObject defineObject(ss, init, indent);

				defineObject.addLine("System.loadLibrary(\"" + _kotlin_wrapper_class_name + "\")");

			}
			
			defineObject.addLine("@Volatile private var instance: " + _kotlin_wrapper_class_name + "? = null");
			defineObject.addLine("@JvmStatic fun getInstance() : " + _kotlin_wrapper_class_name + "=");
			defineObject.addLine("	instance ?: synchronized(this) { instance ?: " + _kotlin_wrapper_class_name + "().also { instance = it} }");
		}
		createWrapperPackageDeclaration(symbolObject, ss, indent);
	}
	
	
	cpp_content = ss.str();

	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::Wrapper::createPackageDefinition(const SymbolPackage& symbolObject, std::stringstream& ss)
{
	std::string packageName = symbolObject.name;
	std::transform(packageName.begin(), packageName.end(), packageName.begin(), ::tolower);

	std::string prefix = "Java_com_";
	prefix += symbolObject.author;
	prefix += "_";
	prefix += packageName;
	prefix += "_";
	prefix += _kotlin_wrapper_class_name;
	prefix += "_";

	for (auto& mod : symbolObject.modules)
	{
		createModuleDefinition(prefix, *mod, ss);
	}	
	
	return Result();
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createModuleDefinition(const std::string& prefix, const SymbolModule& mod, std::stringstream& ss)
{
	for (auto& method : mod.globla_methods)
	{
		auto lines = createStaticMethodDefinition(prefix, *method);
		for (auto& line : lines)
		{
			ss << line << "\n";
		}
	}
	/*
	for (auto& inf : mod.interfaces)
	{
		createClassDefinition(prefix, *inf, ss);
	}
	*/
	for (auto& clazz : mod.classes)
	{
		createClassDefinition(prefix, *clazz, ss);
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createClassDefinition(const std::string& prefix, const SymbolClass& clazz, std::stringstream& ss)
{
	{
		for (auto& constructor : clazz.constructors)
		{
			auto lines = createConstructorDefinition(prefix, clazz, *constructor);
			for (auto& line : lines)
				ss << line << "\n";
		}

		{
			auto lines = createDestructorDefinition(prefix, clazz);
			for (auto& line : lines)
				ss << line << "\n";
		}

		auto& base_methods = clazz.getBaseMethods();
		for (auto& method : base_methods)
		{
			auto lines = createClassMethodDefinition(prefix, clazz, *method);
			for (auto& line : lines)
				ss << line << "\n";
		}

		auto& methods = clazz.methods;
		for (auto& method : methods)
		{
			auto lines = createClassMethodDefinition(prefix, clazz, *method);
			for (auto& line : lines)
				ss << line << "\n";
		}

		auto& base_properties = clazz.getBaseProperties();
		for (auto& prop : base_properties)
		{
			auto lines = createPropertyDefinition(prefix, clazz, *prop);
			for (auto& line : lines)
				ss << line << "\n";
		}

		auto& properties = clazz.properties;
		for (auto& prop : properties)
		{
			auto lines = createPropertyDefinition(prefix, clazz, *prop);
			for (auto& line : lines)
				ss << line << "\n";
		}
	}
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::createConstructorDefinition(const std::string& prefix, const SymbolClass& clazz, const SymbolMethod& constructor)
{
	std::vector<std::string> lines;
	{
		std::string line = "extern \"C\" JNIEXPORT jlong ";
		line += prefix;
		line += createScope(clazz);
		line += "construct(JNIEnv* env, jobject thiz";
		if (!constructor.parameters.empty())
		{
			line += ", ";
			line += createParametersDefinition(constructor);
		}
		line += ")";
		lines.push_back(line);
	}
	lines.push_back("{");
	{
		std::string indent = "    ";
		for (auto& parameter : constructor.parameters)
		{
			lines.push_back(indent + createInputParameterChanger(*parameter));
		}
		lines.push_back(indent + callConstructor(clazz, constructor));
	}
	lines.push_back("}");
	return lines;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::callConstructor(const SymbolClass& clazz, const SymbolMethod& constructor)
{
	std::string line;
	line += "return (jlong)";
	line += createNativeScope(clazz);
	line += "construct(";
	if (!constructor.parameters.empty())
	{
		std::string parameters;
		for (const auto& parameter : constructor.parameters)
		{
			parameters += "i_";
			parameters += parameter->name;
			parameters += ", ";
		}
		parameters.pop_back();
		parameters.pop_back();
		line += parameters;
	}
	line += ");";
	return line;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::createDestructorDefinition(const std::string& prefix, const SymbolClass& clazz)
{
	std::vector<std::string> lines;
	{
		std::string line = "extern \"C\" JNIEXPORT void ";
		line += prefix;
		line += createScope(clazz);
		line += "release(JNIEnv* env, jobject thiz, jlong handle)";
		lines.push_back(line);
	}
	lines.push_back("{");
	{
		std::string indent = "    ";
		lines.push_back(indent + callDestructor(clazz));
	}
	lines.push_back("}");
	return lines;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::callDestructor(const SymbolClass& clazz)
{
	std::string line;
	line += createNativeScope(clazz);
	line += "release((void *)handle);";
	return line;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::createClassMethodDefinition(const std::string& prefix, const SymbolClass& clazz, const SymbolMethod& object)
{
	std::vector<std::string> lines;

	{
		std::string line = "extern \"C\" JNIEXPORT ";
		line += object.type->toJNIType();
		line += " ";
		line += prefix;
		line += createScope(clazz);
		line += object.name;
		line += "(JNIEnv* env, jobject thiz, jlong handle";
		if (!object.parameters.empty())
		{
			line += ", ";
			line += createParametersDefinition(object);
		}
		line += ")";
		lines.push_back(line);
	}
	lines.push_back("{");
	{
		std::string indent = "    ";
		for (auto& parameter : object.parameters)
		{
			lines.push_back(indent + createInputParameterChanger(*parameter));
		}
		lines.push_back(indent + callClassMethod(clazz, object));
		for (auto& parameter : object.parameters)
		{
			if (parameter->io == SymbolParameter::IO::OUT)
				lines.push_back(indent + createOutputParameterChanger(*parameter));
		}
		if (object.type->getTypeName() != SymbolType::Name::VOID)
		{
			lines.push_back(indent + createReturnValueChanger(object));
			lines.push_back(indent + "return __ret;");
		}
	}
	lines.push_back("}");
	return lines;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::callClassMethod(const SymbolClass& clazz, const SymbolMethod& object)
{
	std::string line;
	if (object.type->getTypeName() != SymbolType::Name::VOID)
	{
		line = "auto __temp_ret = ";
	}
	line += createNativeScope(clazz);
	line += object.name;
	line += "((void *)handle";
	if (!object.parameters.empty())
	{
		line += ", ";
		std::string parameters;
		for (const auto& parameter : object.parameters)
		{
			parameters += "i_";
			parameters += parameter->name;
			parameters += ", ";
		}
		parameters.pop_back();
		parameters.pop_back();
		line += parameters;
	}
	line += ");";
	return line;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::createStaticMethodDefinition(const std::string& prefix, const SymbolMethod& object)
{
	std::vector<std::string> lines;

	{
		std::string line = "extern \"C\" JNIEXPORT ";
		line += object.type->toJNIType();
		line += " ";
		line += prefix;
		line += createScope(object);
		line += object.name;
		line += "(JNIEnv* env, jobject thiz";
		if (!object.parameters.empty())
		{
			line += ", ";
			line += createParametersDefinition(object);
		}
		line += ")";
		lines.push_back(line);
	}
	lines.push_back("{");
	std::string indent = "    ";
	{
		for (auto& parameter : object.parameters)
		{
			lines.push_back(indent + createInputParameterChanger(*parameter));
		}
		lines.push_back(indent + callStaticMethod(object));
		for (auto& parameter : object.parameters)
		{
			if (parameter->io == SymbolParameter::IO::OUT)
				lines.push_back(indent + createOutputParameterChanger(*parameter));
		}
		if (object.type->getTypeName() != SymbolType::Name::VOID)
		{
			lines.push_back(indent + createReturnValueChanger(object));
			lines.push_back(indent + "return __ret;");
		}
	}
	lines.push_back("}");
	return lines;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::callStaticMethod(const SymbolMethod& object)
{
	std::string line;
	if (object.type->getTypeName() != SymbolType::Name::VOID)
	{
		line = "auto __temp_ret = ";
	}
	line += createNativeScope(object);
	line += object.name;
	line += "(";
	if (!object.parameters.empty())
	{
		std::string parameters;
		for (const auto& parameter : object.parameters)
		{
			parameters += "i_";
			parameters += parameter->name;
			parameters += ", ";
		}
		parameters.pop_back();
		parameters.pop_back();
		line += parameters;
	}
	line += ");";
	return line;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createParametersDefinition(const SymbolMethod& object)
{
	std::string ret;
	if (!object.parameters.empty())
	{
		for (auto& parameter : object.parameters)
		{
			ret += createParameterDefinition(*parameter);
			ret += ", ";
		}
		ret.pop_back();
		ret.pop_back();
	}
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createParameterDefinition(const SymbolParameter& object)
{
	std::string ret;
	ret = object.type->toJNIType();
	ret += " ";
	ret += object.name;
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createReturnValueChanger(const SymbolMethod& object)
{
	std::string ret;

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
		ret = "auto __ret = static_cast<";
		ret += object.type->toJNIType();
		ret += ">(__temp_ret);";
		break;
	case SymbolType::Name::OBJECT:
		ret = "auto __ret = (";
		ret += object.type->toJNIType();
		ret += ")__temp_ret;";
		break;
	case SymbolType::Name::STRING:
		ret = "auto __ret = createWrapperString(env, __temp_ret);";
		break;
	case SymbolType::Name::BOOLARRAY:
		ret = "auto __ret = createWrapperBooleanArray(env, __temp_ret);";
		break;
	case SymbolType::Name::INT8ARRAY:
		ret = "auto __ret = createWrapperInt8Array(env, __temp_ret);";
		break;
	case SymbolType::Name::INT16ARRAY:
		ret = "auto __ret = createWrapperInt16Array(env, __temp_ret);";
		break;
	case SymbolType::Name::INT32ARRAY:
		ret = "auto __ret = createWrapperInt32Array(env, __temp_ret);";
		break;
	case SymbolType::Name::INT64ARRAY:
		ret = "auto __ret = createWrapperInt64Array(env, __temp_ret);";
		break;
	case SymbolType::Name::FLOATARRAY:
		ret = "auto __ret = createWrapperFloatArray(env, __temp_ret);";
		break;
	case SymbolType::Name::DOUBLEARRAY:
		ret = "auto __ret = createWrapperDoubleArray(env, __temp_ret);";
		break;
	case SymbolType::Name::STRINGARRAY:
		ret = "auto __ret = createWrapperStringArray(env, __temp_ret);";
		break;
	case SymbolType::Name::ENUMARRAY:
		ret = "auto __ret = createWrapperEnumArray(env, __temp_ret);";
		break;
	case SymbolType::Name::OBJECTARRAY:
		ret = "auto __ret = createWrapperObjectArray(env, __temp_ret);";
		break;
	case SymbolType::Name::BOOLVECTOR:
		ret = "auto __ret = createWrapperBooleanVector(env, __temp_ret);";
		break;
	case SymbolType::Name::INT8VECTOR:
		ret = "auto __ret = createWrapperInt8Vector(env, __temp_ret);";
		break;
	case SymbolType::Name::INT16VECTOR:
		ret = "auto __ret = createWrapperInt16Vector(env, __temp_ret);";
		break;
	case SymbolType::Name::INT32VECTOR:
		ret = "auto __ret = createWrapperInt32Vector(env, __temp_ret);";
		break;
	case SymbolType::Name::INT64VECTOR:
		ret = "auto __ret = createWrapperInt64Vector(env, __temp_ret);";
		break;
	case SymbolType::Name::FLOATVECTOR:
		ret = "auto __ret = createWrapperFloatVector(env, __temp_ret);";
		break;
	case SymbolType::Name::DOUBLEVECTOR:
		ret = "auto __ret = createWrapperDoubleVector(env, __temp_ret);";
		break;
	case SymbolType::Name::STRINGVECTOR:
		ret = "auto __ret = createWrapperStringVector(env, __temp_ret);";
		break;
	case SymbolType::Name::ENUMVECTOR:
		ret = "auto __ret = createWrapperEnumVector(env, __temp_ret);";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ret = "auto __ret = createWrapperObjectVector(env, __temp_ret);";
		break;
	}

	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createInputParameterChanger(const SymbolParameter& object)
{
	std::string ret;

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
		ret = "auto i_";
		ret += object.name;
		ret += " = static_cast<";
		ret += object.type->toCppInterfaceType();
		ret += ">(";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::OBJECT:
		ret = "auto i_";
		ret += object.name;
		ret += " = (const ";
		ret += object.type->toCppInterfaceInnerType();
		ret += ")";
		ret += object.name;
		ret += ";";
		break;
	case SymbolType::Name::STRING:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeString(env, ";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::BOOLARRAY:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeBooleanArray(env, ";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::INT8ARRAY:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeInt8Array(env, ";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::INT16ARRAY:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeInt16Array(env, ";
		ret += object.name;
		ret += ");"; break;
	case SymbolType::Name::INT32ARRAY:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeInt32Array(env, ";
		ret += object.name;
		ret += ");"; 
		break;
	case SymbolType::Name::INT64ARRAY:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeInt64Array(env, ";
		ret += object.name;
		ret += ");"; 
		break;
	case SymbolType::Name::FLOATARRAY:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeFloatArray(env, ";
		ret += object.name;
		ret += ");"; 
		break;
	case SymbolType::Name::DOUBLEARRAY:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeDoubleArray(env, ";
		ret += object.name;
		ret += ");"; 
		break;
	case SymbolType::Name::STRINGARRAY:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeStringArray(env, ";
		ret += object.name;
		ret += ");"; 
		break;
	case SymbolType::Name::ENUMARRAY:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeEnumArray(env, ";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::OBJECTARRAY:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeObjectArray(env, ";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::BOOLVECTOR:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeBooleanVector(env, ";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::INT8VECTOR:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeInt8Vector(env, ";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::INT16VECTOR:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeInt16Vector(env, ";
		ret += object.name;
		ret += ");"; break;
	case SymbolType::Name::INT32VECTOR:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeInt32Vector(env, ";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::INT64VECTOR:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeInt64Vector(env, ";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::FLOATVECTOR:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeFloatVector(env, ";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::DOUBLEVECTOR:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeDoubleVector(env, ";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::STRINGVECTOR:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeStringVector(env, ";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::ENUMVECTOR:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeEnumVector(env, ";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ret = "auto i_";
		ret += object.name;
		ret += " = createNativeObjectVector(env, ";
		ret += object.name;
		ret += ");";
		break;
	}

	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createOutputParameterChanger(const SymbolParameter& object)
{
	std::string ret;

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
		ret += object.name;
		ret += " = static_cast<";
		ret += object.type->toJNIType();
		ret += ">(i_";
		ret += object.name;
		ret += "); // NOT AVAILABLE";
		break;
	case SymbolType::Name::OBJECT:
		ret = object.name;
		ret += " = (";
		ret += object.type->toJNIType();
		ret += ")i_";
		ret += object.name;
		ret += "; // NOT AVAILABLE";
		break;
	case SymbolType::Name::STRING:
		ret = "auto ";
		ret += object.name;
		ret += " = createWrapperString(env, i_";
		ret += object.name;
		ret += "); // NOT AVAILABLE";
		break;
	case SymbolType::Name::BOOLARRAY:
		ret = "copyBooleanArray(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::INT8ARRAY:
		ret = "copyInt8Array(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::INT16ARRAY:
		ret = "copyInt16Array(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");"; 
		break;
	case SymbolType::Name::INT32ARRAY:
		ret = "copyInt32Array(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::INT64ARRAY:
		ret = "copyInt64Array(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::FLOATARRAY:
		ret = "copyFloatArray(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::DOUBLEARRAY:
		ret = "copyDoubleArray(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::STRINGARRAY:
		ret = "copyStringArray(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::ENUMARRAY:
		ret = "copyEnumArray(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::OBJECTARRAY:
		ret = "copyObjectArray(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::BOOLVECTOR:
		ret = "copyBooleanVector(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::INT8VECTOR:
		ret = "copyInt8Vector(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::INT16VECTOR:
		ret = "copyInt16Vector(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::INT32VECTOR:
		ret = "copyInt32Vector(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::INT64VECTOR:
		ret = "copyInt64Vector(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::FLOATVECTOR:
		ret = "copyFloatVector(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::DOUBLEVECTOR:
		ret = "copyDoubleVector(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::STRINGVECTOR:
		ret = "copyStringVector(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::ENUMVECTOR:
		ret = "copyEnumVector(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ret = "copyObjectVector(env, i_";
		ret += object.name;
		ret += ",";
		ret += object.name;
		ret += ");";
		break;
	}

	return ret;
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

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createPropertySetterDeclaration(const std::string& prefix, const std::string& scope, const std::string& propertyName, const SymbolProperty& object)
{

	std::string line = "extern \"C\" JNIEXPORT void ";
	line += prefix;
	line += scope;
	line += "set";
	line += propertyName;
	line += "(JNIEnv* env, jobject thiz, jlong handle, ";
	line += object.type->toJNIType();
	line += " value)";
	return line;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createPropertyGetterDeclaration(const std::string& prefix, const std::string& scope, const std::string& propertyName, const SymbolProperty& object)
{
	std::string line = "extern \"C\" JNIEXPORT ";
	line += object.type->toJNIType();
	line += " ";
	line += prefix;
	line += scope;
	line += "get";
	line += propertyName;
	line += "(JNIEnv* env, jobject thiz, jlong handle)";
	return line;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::callPropertySetter(const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object)
{
	std::string line;
	line += createNativeScope(clazz);
	line += "set";
	line += propertyName;
	line += "((void *)handle, i_value);";
	return line;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::callPropertyGetter(const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object)
{
	std::string line;
	line += "auto __temp_ret = ";
	line += createNativeScope(clazz);
	line += "get";
	line += propertyName;
	line += "((void *)handle);";
	return line;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::createPropertyDefinition(const std::string& prefix, const SymbolClass& clazz, const SymbolProperty& object)
{
	std::vector<std::string> ret;
	std::string indent = "    ";
	std::string scope = createScope(clazz);
	std::string propertyName = createPropertyName(object);

	
	ret.push_back(createPropertyGetterDeclaration(prefix, scope, propertyName, object));
	ret.push_back("{");
	{
		ret.push_back(indent + callPropertyGetter(propertyName, clazz, object));
	}
	ret.push_back(indent + createOutputPropertyChanger(object));
	ret.push_back(indent + "return __ret;");
	ret.push_back("}");

	if (object.readonly == false)
	{
		ret.push_back(createPropertySetterDeclaration(prefix, scope, propertyName, object));
		ret.push_back("{");
		ret.push_back(indent + createInputPropertyChanger(object));
		{
			ret.push_back(indent + callPropertySetter(propertyName, clazz, object));
		}
		ret.push_back("}");
	}
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createInputPropertyChanger(const SymbolProperty& object)
{
	std::string ret;

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
		ret = "auto i_value = static_cast<";
		ret += object.type->toCppInterfaceType();
		ret += ">(value);";
		break;
	case SymbolType::Name::OBJECT:
		ret = "auto i_value = (const ";
		ret += object.type->toCppInterfaceInnerType();
		ret += ")value;";
		break;
	case SymbolType::Name::STRING:
		ret = "auto i_value = createNativeString(env, value);";
		break;
	case SymbolType::Name::BOOLARRAY:
		ret = "auto i_value = createNativeBooleanArray(env, value);";
		break;
	case SymbolType::Name::INT8ARRAY:
		ret = "auto i_value = createNativeInt8Array(env, value);";
		break;
	case SymbolType::Name::INT16ARRAY:
		ret = "auto i_value = createNativeInt16Array(env, value);";
		break;
	case SymbolType::Name::INT32ARRAY:
		ret = "auto i_value = createNativeInt32Array(env, value);";
		break;
	case SymbolType::Name::INT64ARRAY:
		ret = "auto i_value = createNativeInt64Array(env, value);";
		break;
	case SymbolType::Name::FLOATARRAY:
		ret = "auto i_value = createNativeFloatArray(env, value);";
		break;
	case SymbolType::Name::DOUBLEARRAY:
		ret = "auto i_value = createNativeDoubleArray(env, value);";
		break;
	case SymbolType::Name::STRINGARRAY:
		ret = "auto i_value = createNativeStringArray(env, value);";
		break;
	case SymbolType::Name::ENUMARRAY:
		ret = "auto i_value = createNativeEnumArray(env, value);";
		break;
	case SymbolType::Name::OBJECTARRAY:
		ret = "auto i_value = createNativeObjectArray(env, value);";
		break;
	case SymbolType::Name::BOOLVECTOR:
		ret = "auto i_value = createNativeBooleanVector(env, value);";
		break;
	case SymbolType::Name::INT8VECTOR:
		ret = "auto i_value = createNativeInt8Vector(env, value);";
		break;
	case SymbolType::Name::INT16VECTOR:
		ret = "auto i_value = createNativeInt16Vector(env, value);";
		break;
	case SymbolType::Name::INT32VECTOR:
		ret = "auto i_value = createNativeInt32Vector(env, value);";
		break;
	case SymbolType::Name::INT64VECTOR:
		ret = "auto i_value = createNativeInt64Vector(env, value);";
		break;
	case SymbolType::Name::FLOATVECTOR:
		ret = "auto i_value = createNativeFloatVector(env, value);";
		break;
	case SymbolType::Name::DOUBLEVECTOR:
		ret = "auto i_value = createNativeDoubleVector(env, value);";
		break;
	case SymbolType::Name::STRINGVECTOR:
		ret = "auto i_value = createNativeStringVector(env, value);";
		break;
	case SymbolType::Name::ENUMVECTOR:
		ret = "auto i_value = createNativeEnumVector(env, value);";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ret = "auto i_value = createNativeObjectVector(env, value);";
		break;
	}

	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createOutputPropertyChanger(const SymbolProperty& object)
{
	// __temp_ret -> __ret
	std::string ret;

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
		ret = "auto __ret = static_cast<";
		ret += object.type->toJNIType();
		ret += ">(__temp_ret);";
		break;
	case SymbolType::Name::OBJECT:
		ret = "auto __ret = (";
		ret += object.type->toJNIType();
		ret += ")__temp_ret;";
		break;
	case SymbolType::Name::STRING :
		ret = "auto __ret = createWrapperString(env, __temp_ret);";
		break;
	case SymbolType::Name::BOOLARRAY:
		ret = "auto __ret = createWrapperBooleanArray(env, __temp_ret);";
		break;
	case SymbolType::Name::INT8ARRAY:
		ret = "auto __ret = createWrapperInt8Array(env, __temp_ret);";
		break;
	case SymbolType::Name::INT16ARRAY:
		ret = "auto __ret = createWrapperInt16Array(env, __temp_ret);";
		break;
	case SymbolType::Name::INT32ARRAY:
		ret = "auto __ret = createWrapperInt32Array(env, __temp_ret);";
		break;
	case SymbolType::Name::INT64ARRAY:
		ret = "auto __ret = createWrapperInt64Array(env, __temp_ret);";
		break;
	case SymbolType::Name::FLOATARRAY:
		ret = "auto __ret = createWrapperFloatArray(env, __temp_ret);";
		break;
	case SymbolType::Name::DOUBLEARRAY:
		ret = "auto __ret = createWrapperDoubleArray(env, __temp_ret);";
		break;
	case SymbolType::Name::STRINGARRAY:
		ret = "auto __ret = createWrapperStringArray(env, __temp_ret);";
		break;
	case SymbolType::Name::ENUMARRAY:
		ret = "auto __ret = createWrapperEnumArray(env, __temp_ret);";
		break;
	case SymbolType::Name::OBJECTARRAY:
		ret = "auto __ret = createWrapperObjectArray(env, __temp_ret);";
		break;
	case SymbolType::Name::BOOLVECTOR:
		ret = "auto __ret = createWrapperBooleanVector(env, __temp_ret);";
		break;
	case SymbolType::Name::INT8VECTOR:
		ret = "auto __ret = createWrapperInt8Vector(env, __temp_ret);";
		break;
	case SymbolType::Name::INT16VECTOR:
		ret = "auto __ret = createWrapperInt16Vector(env, __temp_ret);";
		break;
	case SymbolType::Name::INT32VECTOR:
		ret = "auto __ret = createWrapperInt32Vector(env, __temp_ret);";
		break;
	case SymbolType::Name::INT64VECTOR:
		ret = "auto __ret = createWrapperInt64Vector(env, __temp_ret);";
		break;
	case SymbolType::Name::FLOATVECTOR:
		ret = "auto __ret = createWrapperFloatVector(env, __temp_ret);";
		break;
	case SymbolType::Name::DOUBLEVECTOR:
		ret = "auto __ret = createWrapperDoubleVector(env, __temp_ret);";
		break;
	case SymbolType::Name::STRINGVECTOR:
		ret = "auto __ret = createWrapperStringVector(env, __temp_ret);";
		break;
	case SymbolType::Name::ENUMVECTOR:
		ret = "auto __ret = createWrapperEnumVector(env, __temp_ret);";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ret = "auto __ret = createWrapperObjectVector(env, __temp_ret);";
		break;
	}

	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createScope(const SymbolClass& clazz)
{
	std::string scope;
	auto& moduleNames = clazz.parentModules;
	auto& objectNames = clazz.parentObjects;
	for (auto& moduleName : moduleNames)
	{
		scope += moduleName;
		scope += "_1";
	}
	for (auto& objectName : objectNames)
	{
		scope += objectName;
		scope += "_1";
	}
	scope += clazz.name;
	scope += "_1";

	return scope;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createScope(const SymbolMethod& method)
{
	std::string scope;
	auto& moduleNames = method.parentModules;
	for (auto& moduleName : moduleNames)
	{
		scope += moduleName;
		scope += "_1";
	}
	return scope;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createNativeScope(const SymbolClass& clazz)
{
	std::string scope;
	auto& moduleNames = clazz.parentModules;
	auto& objectNames = clazz.parentObjects;
	
	for (size_t i=1;i<moduleNames.size();i++)
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

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createNativeScope(const SymbolMethod& method)
{
	std::string scope;
	auto& moduleNames = method.parentModules;
	for (size_t i = 1; i < moduleNames.size(); i++)
	{
		scope += moduleNames[i];
		scope += "::";
	}
	return scope;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createChangerFunction()
{

	// bool : bool <-> jboolean
	// int8 : int8_t <-> jbyte
	// int16 : int16_t <-> jshort
	// int32 : int32_t <-> jint
	// int64 : int64_t <-> jlong
	// float : float <-> jfloat
	// double : double <-> jdouble
	// string : std::string <-> jstring (�Լ� �ʿ�)
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

	return kotlinWrapperConverter;
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperPackageDeclaration(const SymbolPackage& symbolObject, std::stringstream& ss, std::string indent)
{
	for (auto& mod : symbolObject.modules)
	{
		createWrapperModuleDeclaration(*mod, ss, indent);
	}

	return Result();
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperModuleDeclaration(const SymbolModule& mod, std::stringstream& ss, std::string indent)
{
	for (auto& method : mod.globla_methods)
	{
		ss << indent << createWrapperStaticMethodDeclaration(*method) << "\n";
	}
	/*
	for (auto& inf : mod.interfaces)
	{
		createWrapperClassDeclaration(*inf, ss, indent);
	}
	*/
	for (auto& clazz : mod.classes)
	{
		createWrapperClassDeclaration(*clazz, ss, indent);
	}
}

void LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperClassDeclaration(const SymbolClass& clazz, std::stringstream& ss, std::string indent)
{
	{
		for (auto& constructor : clazz.constructors)
		{
			ss << indent << createWrapperConstructorDeclaration(clazz, *constructor) << "\n";
		}

		{
			ss << indent << createWrapperDestructorDeclaration(clazz) << "\n";
		}

		auto& base_methods = clazz.getBaseMethods();
		for (auto& method : base_methods)
		{
			ss << indent << createWrapperClassMethodDeclaration(clazz, *method) << "\n";
		}

		auto& methods = clazz.methods;
		for (auto& method : methods)
		{
			ss << indent << createWrapperClassMethodDeclaration(clazz, *method) << "\n";
		}

		auto& base_properties = clazz.getBaseProperties();
		for (auto& prop : base_properties)
		{
			auto lines = createWrapperPropertyDeclaration(clazz, *prop);
			for (auto& line : lines)
				ss << indent << line << "\n";
		}

		auto& properties = clazz.properties;
		for (auto& prop : properties)
		{
			auto lines = createWrapperPropertyDeclaration(clazz, *prop);
			for (auto& line : lines)
				ss << indent << line << "\n";
		}
	}
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperConstructorDeclaration(const SymbolClass& clazz, const SymbolMethod& constructor)
{
	std::string ret = { "external fun " };
	ret += createWrapperScope(clazz);
	ret += "construct(";
	if (!constructor.parameters.empty())
	{
		ret += createWrapperParametersDeclaration(constructor);
	}
	ret += ") : Long";
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperDestructorDeclaration(const SymbolClass& clazz)
{
	std::string ret = { "external fun " };
	ret += createWrapperScope(clazz);
	ret += "release(handle : Long)";
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperClassMethodDeclaration(const SymbolClass& clazz, const SymbolMethod& object)
{
	std::string ret = { "external fun " };
	ret += createWrapperScope(clazz);
	ret += object.name;
	ret += "(handle : Long";
	if (!object.parameters.empty())
	{
		ret += ", ";
		ret += createWrapperParametersDeclaration(object);
	}
	ret += ") : ";
	ret += object.type->toKotlinWrapperType();
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperStaticMethodDeclaration(const SymbolMethod& object)
{
	std::string ret = { "external fun " };
	ret += createWrapperScope(object);
	ret += object.name;
	ret += "(";
	if (!object.parameters.empty())
	{
		ret += createWrapperParametersDeclaration(object);
	}
	ret += ") : ";
	ret += object.type->toKotlinWrapperType();
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperParametersDeclaration(const SymbolMethod& object)
{
	std::string ret;
	if (!object.parameters.empty())
	{
		for (auto& parameter : object.parameters)
		{
			ret += createWrapperParameterDeclaration(*parameter);
			ret += ", ";
		}
		ret.pop_back();
		ret.pop_back();
	}
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperParameterDeclaration(const SymbolParameter& object)
{
	std::string ret = object.name;
	ret += " : ";
	ret += object.type->toKotlinWrapperType();
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperPropertySetterDeclaration(const std::string& scope, const std::string& propertyName, const SymbolProperty& object)
{
	std::string ret = { "external fun " };
	ret += scope;
	ret += "set";
	ret += propertyName;
	ret += "(handle : Long, value : ";
	ret += object.type->toKotlinWrapperType();
	ret += ")";
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperPropertyGetterDeclaration(const std::string& scope, const std::string& propertyName, const SymbolProperty& object)
{
	std::string ret = { "external fun " };
	ret += scope;
	ret += "get";
	ret += propertyName;
	ret += "(handle : Long) : ";
	ret += object.type->toKotlinWrapperType();
	return ret;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperPropertyDeclaration(const SymbolClass& clazz, const SymbolProperty& object)
{
	std::vector<std::string> ret;

	std::string scope = createWrapperScope(clazz);
	std::string propertyName = createPropertyName(object);

	ret.push_back(createWrapperPropertyGetterDeclaration(scope, propertyName, object));

	if (object.readonly == false)
	{
		ret.push_back(createWrapperPropertySetterDeclaration(scope, propertyName, object));
	}

	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperScope(const SymbolClass& clazz)
{
	std::string scope;
	auto& moduleNames = clazz.parentModules;
	auto& objectNames = clazz.parentObjects;
	for (auto& moduleName : moduleNames)
	{
		scope += moduleName;
		scope += "_";
	}
	for (auto& objectName : objectNames)
	{
		scope += objectName;
		scope += "_";
	}
	scope += clazz.name;
	scope += "_";

	return scope;
}

std::string LibraryInterfaceGenerator::Implementation::Wrapper::createWrapperScope(const SymbolMethod& method)
{
	std::string scope;
	auto& moduleNames = method.parentModules;
	for (auto& moduleName : moduleNames)
	{
		scope += moduleName;
		scope += "_";
	}
	return scope;
}
