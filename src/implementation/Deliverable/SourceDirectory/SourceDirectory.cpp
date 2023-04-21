#include "SourceDirectory.hpp"

using namespace LibraryInterfaceGenerator::Implementation;
using namespace LibraryInterfaceGenerator::Implementation::Definition;

#ifdef __linux__
static char* delimeter = "/";
#elif _WIN32
static char* delimeter = "\\";
#else
static char* delimeter = "/";
#endif


LibraryInterfaceGenerator::Implementation::SourceDirectory::SourceDirectory(Environment environment, const Wrapper& wrapperDirectory, const SymbolTable& symbolTable, std::string root_dir_path, const char* dir_name)
	: _wrapperDirectory(wrapperDirectory), _symbolTable(symbolTable)
{
	_src_dir_path = root_dir_path;
	_src_dir_path += delimeter;
	_src_dir_path += dir_name;
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::make()
{
	auto& package = _symbolTable.getPackage();
	auto& modules = package.modules;

	Result result{ Result::Code::SUCCESS };

	result = FileSystem::createDirectory(_src_dir_path);
	if (!result)
		return result;

	for (auto& submodule : modules)
	{
		result = createModule(*submodule, _src_dir_path);
		if (!result)
			return result;
	}

	return result;
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createModule(const SymbolModule& object, std::string& parent_path)
{
	std::string module_path{ parent_path };
	module_path += delimeter;
	module_path += object.name;

	Result result;
	result = FileSystem::createDirectory(module_path);
	if (!result)
		return result;

	auto& submodules = object.submodules;
	for (auto& submodule : submodules)
	{
		auto result = createModule(*submodule, module_path);
		if (!result)
			return result;
	}

	auto& interfaces = object.interfaces;
	for (auto& interfaze : interfaces)
	{
		auto result = createInterfaceFile(*interfaze, module_path);
		if (!result)
			return result;
	}

	auto& classes = object.classes;
	for (auto& clazz : classes)
	{
		auto result = createClassFile(*clazz, module_path);
		if (!result)
			return result;
	}

	auto& enums = object.enums;
	for (auto& enumm : enums)
	{
		auto result = createEnumFile(*enumm, module_path);
		if (!result)
			return result;
	}

	auto& global_methods = object.global_methods;
	if (!global_methods.empty())
	{
		auto result = createMethodFile(object, module_path);
		if (!result)
			return result;
	}

	return result;
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createInterfaceFile(const SymbolClass& object, std::string& parent_path)
{
	std::string header_path{ parent_path };
	header_path += delimeter;
	header_path += object.name;
	header_path += ".kt";
	
	auto content = createInterfaceFileContent(object);
	auto result = FileSystem::createFile(header_path, content.str());
	if (!result)
		return result;

	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createClassFile(const SymbolClass& object, std::string& parent_path)
{
	std::string header_path{ parent_path };
	header_path += delimeter;
	header_path += object.name;
	header_path += ".kt";

	auto content = createClassFileContent(object);
	auto result = FileSystem::createFile(header_path, content.str());
	if (!result)
		return result;

	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createEnumFile(const SymbolEnum& object, std::string& parent_path)
{
	std::string header_path{ parent_path };
	header_path += delimeter;
	header_path += object.name;
	header_path += ".kt";
		
	auto content = createEnumFileContent(object);
	auto result = FileSystem::createFile(header_path, content.str());
	if (!result)
		return result;

	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createMethodFile(const SymbolModule& object, std::string& parent_path)
{
	std::string header_path{ parent_path };
	header_path += delimeter;
	header_path += object.name;
	header_path += ".kt";

	auto content = createMethodFileContent(object);
	auto result = FileSystem::createFile(header_path, content.str());
	if (!result)
		return result;

	return Result();
}

SourceStream LibraryInterfaceGenerator::Implementation::SourceDirectory::createInterfaceFileContent(const SymbolClass& object)
{
	SourceStream ss;

	{
		PackageKotlinSourceStream package(ss, _wrapperDirectory.getKotlinPackageName(), object.parentModules);
	}
	{
		ImportKotlinSourceStream wrapperImport(ss, _wrapperDirectory.getKotlinPackageName(), { _wrapperDirectory.getKotlinWrapperClassName() });
		createForwardDeclaration(ss, object);
	}

	{
		createKotlinComment(ss, object);
		InterfaceKotlinSourceScopedStream interfaze(ss, object.name);
		
		for (auto& member_method : object.methods)
		{
			auto method = member_method.first;
			auto method_count = member_method.second;

			{
				createKotlinComment(ss, *method);
				createInterfaceMethodDefinition(ss, object, *method);
			}
		}

		for (auto& prop : object.properties)
		{
			{
				createKotlinComment(ss, *prop);
				createInterfacePropertyDefinition(ss, object, *prop);
			}
		}

	}

	return ss;
}

SourceStream LibraryInterfaceGenerator::Implementation::SourceDirectory::createClassFileContent(const SymbolClass& object)
{
	SourceStream ss;

	{
		PackageKotlinSourceStream package(ss, _wrapperDirectory.getKotlinPackageName(), object.parentModules);
	}
	{
		ImportKotlinSourceStream wrapperImport(ss, _wrapperDirectory.getKotlinPackageName(), { _wrapperDirectory.getKotlinWrapperClassName() });
		ImportKotlinSourceStream autoClosableImport(ss, "", { "java", "lang", "AutoCloseable" });
		createForwardDeclaration(ss, object);
	}
		

	{
		createKotlinComment(ss, object);
		
		std::vector<std::string> base_classes = { "AutoClosable" };
		for (auto& base : object.bases)
		{
			if (auto& pBase = base.lock())
			{
				base_classes.push_back(pBase->name);
			}
		}		
		ClassKotlinSourceScopedStream clazz(ss, object.name, base_classes);

		createNativeHandle(ss, true);

		for (auto& constructor : object.constructors)
		{
			auto method = constructor.first;
			auto method_number = constructor.second;
			{
				createKotlinComment(ss, *method);
				createConstructorDefinition(ss, object, *method, method_number);
			}
		}

		createDestructorDefinition(ss, object);
		
		auto base_methods = object.getBaseMethods();
		for (auto& base_method : base_methods)
		{
			auto& method = base_method.first;
			auto& method_number = base_method.second;

			{
				createKotlinComment(ss, *method);
				createDerivedMethodDefinition(ss, object, *method, method_number);
			}
		}

		for (auto& member_method : object.methods)
		{
			auto& method = member_method.first;
			auto& method_number = member_method.second;

			{
				createKotlinComment(ss, *method);
				createClassMethodDefinition(ss, object, *method, method_number);
			}
		}

		auto base_properties = object.getBaseProperties();
		for (auto& base_prop : base_properties)
		{
			createKotlinComment(ss, *base_prop);
			createDerivedPropertyDefinition(ss, object, *base_prop);
		}

		for (auto& prop : object.properties)
		{
			createKotlinComment(ss, *prop);
			createClassPropertyDefinition(ss, object, *prop);
		}
	}

	return ss;
}

SourceStream LibraryInterfaceGenerator::Implementation::SourceDirectory::createEnumFileContent(const SymbolEnum& object)
{
	SourceStream ss;

	{
		PackageKotlinSourceStream package(ss, _wrapperDirectory.getKotlinPackageName(), object.parentModules);
	}
	{
		ImportKotlinSourceStream wrapperImport(ss, _wrapperDirectory.getKotlinPackageName(), { _wrapperDirectory.getKotlinWrapperClassName() });
	}
	{
		createKotlinComment(ss, object);
		createEnumDefinition(ss, object);
	}

	return ss;
}

SourceStream LibraryInterfaceGenerator::Implementation::SourceDirectory::createMethodFileContent(const SymbolModule& object)
{
	SourceStream ss;

	{
		PackageKotlinSourceStream package(ss, _wrapperDirectory.getKotlinPackageName(), object.moduleNames);
	}
	{
		ImportKotlinSourceStream wrapperImport(ss, _wrapperDirectory.getKotlinPackageName(), { _wrapperDirectory.getKotlinWrapperClassName() });
		createForwardDeclaration(ss, object);
	}

	for (auto& global_method : object.global_methods)
	{
		auto& method = global_method.first;
		auto& method_count = global_method.second;

		createStaticMethodDefinition(ss, *method, method_count);
	}

	return ss;
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::createForwardDeclaration(SourceStream& ss, const SymbolClass& object)
{
	{
		auto base_classes = object.bases;
		for (auto& wbase_class : base_classes)
		{
			if (auto& pbase_class = wbase_class.lock())
			{
				std::vector<std::string> paths;
				{
					auto& parent_modules = pbase_class->parentModules;
					paths.insert(paths.end(), parent_modules.begin(), parent_modules.end());
				}
				{
					auto& parent_objects = pbase_class->parentObjects;
					paths.insert(paths.end(), parent_objects.begin(), parent_objects.end());
				}
				paths.push_back(pbase_class->name);

				ImportKotlinSourceStream importClass{
					ss, _wrapperDirectory.getKotlinPackageName(), paths
				};
			}
		}
	}
	{
		auto used_classes = object.collectAllClassReference();
		for (auto& wclass : used_classes)
		{
			if (auto& pclass = wclass.lock())
			{
				auto clazz = std::dynamic_pointer_cast<SymbolClass>(pclass);
				std::vector<std::string> paths;
				{
					auto& parent_modules = clazz->parentModules;
					paths.insert(paths.end(), parent_modules.begin(), parent_modules.end());
				}
				{
					auto& parent_objects = clazz->parentObjects;
					paths.insert(paths.end(), parent_objects.begin(), parent_objects.end());
				}
				paths.push_back(clazz->name);

				ImportKotlinSourceStream importClass{
					ss, _wrapperDirectory.getKotlinPackageName(), paths
				};
			}
		}
	}
	{
		auto used_enums = object.collectAllEnumReference();
		for (auto& wenum : used_enums)
		{
			if (auto& penum = wenum.lock())
			{
				auto enumm = std::dynamic_pointer_cast<SymbolEnum>(penum);
				std::vector<std::string> paths;
				{
					auto& parent_modules = enumm->parentModules;
					paths.insert(paths.end(), parent_modules.begin(), parent_modules.end());
				}
				{
					auto& parent_objects = enumm->parentObjects;
					paths.insert(paths.end(), parent_objects.begin(), parent_objects.end());
				}
				paths.push_back(enumm->name);

				ImportKotlinSourceStream importClass{
					ss, _wrapperDirectory.getKotlinPackageName(), paths
				};
			}
		}
	}
	ss << "\n";
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::createForwardDeclaration(SourceStream& ss, const SymbolModule& object)
{
	{
		auto used_classes = object.collectAllClassReference();
		for (auto& wclass : used_classes)
		{
			if (auto& pclass = wclass.lock())
			{
				auto clazz = std::dynamic_pointer_cast<SymbolClass>(pclass);
				std::vector<std::string> paths;
				{
					auto& parent_modules = clazz->parentModules;
					paths.insert(paths.end(), parent_modules.begin(), parent_modules.end());
				}
				{
					auto& parent_objects = clazz->parentObjects;
					paths.insert(paths.end(), parent_objects.begin(), parent_objects.end());
				}
				paths.push_back(clazz->name);

				ImportKotlinSourceStream importClass{
					ss, _wrapperDirectory.getKotlinPackageName(), paths
				};
			}
		}
	}
	{
		auto used_enums = object.collectAllEnumReference();
		for (auto& wenum : used_enums)
		{
			if (auto& penum = wenum.lock())
			{
				auto enumm = std::dynamic_pointer_cast<SymbolEnum>(penum);
				std::vector<std::string> paths;
				{
					auto& parent_modules = enumm->parentModules;
					paths.insert(paths.end(), parent_modules.begin(), parent_modules.end());
				}
				{
					auto& parent_objects = enumm->parentObjects;
					paths.insert(paths.end(), parent_objects.begin(), parent_objects.end());
				}
				paths.push_back(enumm->name);

				ImportKotlinSourceStream importClass{
					ss, _wrapperDirectory.getKotlinPackageName(), paths
				};
			}
		}
	}
	ss << "\n";
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::createEnumDefinition(SourceStream& ss, const SymbolEnum& object)
{
	if (object.keys_to_names.empty())
	{	
		EnumKotlinSourceScopedStream s{
			ss, object.name
		};
		for (auto& element : object.keys_to_values)
		{
			auto key = element.first;
			auto value = element.second;

			s.addElement(key, value);
		}
	}
	else
	{
		AdvancedEnumKotlinSourceScopedStream s{
			ss, object.name
		};
		for (auto& element : object.keys_to_values)
		{
			auto key = element.first;
			auto value = element.second;
			auto name = object.keys_to_names.at(key);

			s.addElement(key, value, name);
		}
	}
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::createNativeHandle(SourceStream& ss)
{
	{
		ss << "private var _nativeHandle : Long = 0\n";
		PropertyKotlinSourceScopedStream nativeHandle{
			ss,
			KotlinAccess::INTERNAL,
			"",
			"Long",
			"nativeHandle",
			true
		};
		{
			auto getter = nativeHandle.createGetter();
			ss << "return _nativeHandle\n";
		}
	}
}

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
	return ParameterNode(io, parameter.type->toKotlinType(), parameter.name);
}
static std::vector<ParameterNode> createParameters(const SymbolMethod& method)
{
	std::vector<ParameterNode> ret;
	for (auto& param : method.parameters)
	{
		ret.push_back(createParameter(*param));
	}
	return ret;
}
static ParameterNode createHandleParameter()
{
	return ParameterNode(ParameterNode::VALUE, "Long", "handle");
}

static ParameterNode createInputHandleParameter()
{
	return ParameterNode(ParameterNode::VALUE, "Long", "_nativeHandle");
}
static std::vector<ParameterNode> createInputConstructorParameter(const SymbolMethod& method)
{
	std::vector<ParameterNode> ret = createInputStaticParamter(method);
	return ret;
}
static std::vector<ParameterNode> createInputDestructorParameter(const SymbolMethod& method)
{
	std::vector<ParameterNode> ret;
	ret.push_back(createInputHandleParameter());
	return ret;
}
static std::vector<ParameterNode> createInputAddReleaserParameter(const SymbolMethod& method)
{
	std::vector<ParameterNode> ret;
	for (auto& param : method.parameters)
	{
		ret.push_back(ParameterNode(ParameterNode::VALUE, "", "this"));
		ret.push_back(ParameterNode(ParameterNode::VALUE, "", "\"close\""));
	}
	return ret;
}
static std::vector<ParameterNode> createInputStaticParamter(const SymbolMethod& method)
{
	std::vector<ParameterNode> ret;
	for (auto& param : method.parameters)
	{
		std::string method_name = "i_" + method.name;
		ret.push_back(ParameterNode(ParameterNode::VALUE, param->type->toKotlinType(), method_name));
	}
	return ret;
}
static std::vector<ParameterNode> createInputMemberParamter(const SymbolMethod& method)
{
	std::vector<ParameterNode> ret;
	ret.push_back(createInputHandleParameter());
	auto params = createInputStaticParamter(method);
	ret.insert(ret.end(), params.begin(), params.end());
	return ret;
}
static std::vector<ParameterNode> createInputPropertyParameter(const SymbolProperty& method)
{
	std::vector<ParameterNode> ret;
	ret.push_back(createInputHandleParameter());
	ret.push_back(ParameterNode(ParameterNode::VALUE, "", "i_value"));
	return ret;
}


void LibraryInterfaceGenerator::Implementation::SourceDirectory::createConstructorDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& constructor, int number)
{
	{
		MethodKotlinSourceScopedStream method{
			ss,
			false,
			KotlinAccess::PUBLIC,
			"",
			"",
			"",
			"constructor",
			createParameters(constructor)
		};

		for (auto& parameter : constructor.parameters)
		{
			createInputParameterChanger(ss, *parameter);
		}

		callConstructor(ss, clazz, constructor, number);

		ss << "require(_nativeHandle != 0L)\n";

		callAddReleaser(ss, clazz);

		/*
		std::string wrapper_constructor = _wrapperDirectory.getKotlinWrapperClassName();
		wrapper_constructor += ".getInsance().";
		wrapper_constructor += createKotlinWrapperScope(clazz);
		wrapper_constructor += "construct";

		ss << "_nativeHandle = ";
		*/
	}

	{
		MethodKotlinSourceScopedStream method{
			ss,
			false,
			KotlinAccess::INTERNAL,
			"",
			"",
			"",
			"constructor",
			{createHandleParameter()}
		};

		ss << "_nativeHandle = handle\n";
		ss << "require(_nativeHandle != 0L)\n";
		callAddReleaser(ss, clazz);
	}
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::createDestructorDefinition(SourceStream& ss, const SymbolClass& clazz)
{
}

/*

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createConstructorDefinition(const SymbolClass& clazz, const SymbolMethod& object, int number)
{
	std::vector<std::string> lines{};

	{
		std::string line = "constructor(";
		line += createParametersDefinition(object);
		line += ")";
		lines.push_back(line);
	}
	lines.push_back("{");
	{
		std::string indent = "\t";
		for (auto& parameter : object.parameters)
		{
			lines.push_back(indent + createInputParameterChanger(*parameter));
		}
		{
			std::string line;
			line += indent;
			line += "_handle = ";
			line += _wrapperDirectory.getKotlinWrapperClassName();
			line += ".getInstance().";
			line += createWrapperScope(clazz);
			line += "construct";
			if (number > 0)
			{
				line += std::to_string(number);
			}
			line += "(";
			if (!object.parameters.empty())
			{
				for (auto& parameter : object.parameters)
				{
					line += "i_";
					line += parameter->name;
					line += ", ";
				}
				line.pop_back();
				line.pop_back();
			}
			line += ")";
			lines.push_back(line);
		}
		
		lines.push_back(indent + "require(_handle != 0L)");

		{
			std::string line = indent;
			line += _wrapperDirectory.getKotlinWrapperClassName();
			line += ".getInstance().";
			line += createWrapperScope(clazz);
			line += "addReleaser(this)";
			lines.push_back(line);
		}
	}
	lines.push_back("}");

	lines.push_back("internal constructor(handle : Long) { ");
	lines.push_back("\t_handle = handle");
	lines.push_back("\trequire(_handle != 0L)");
	{
		std::string line = "\t";
		line += _wrapperDirectory.getKotlinWrapperClassName();
		line += ".getInstance().";
		line += createWrapperScope(clazz);
		line += "addReleaser(this)";
		lines.push_back(line);
	}
	lines.push_back("}");
	lines.push_back("private var _handle : Long");
	{
		std::string line;
		if (clazz.bases.empty())
			line = "internal ";
		else
			line = "override ";
		line += "fun getNativeHandle() : Long = _handle";
		lines.push_back(line);
	}
	return lines;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createDestructorDefinition(const SymbolClass& clazz)
{
	std::vector<std::string> lines{};

	{
		lines.push_back("override fun close()");
	}
	lines.push_back("{");
	std::string indent = "\t";
	{
		std::string line{ indent };
		line += "if (_handle != 0L)";
		lines.push_back(line);
	}
	{
		std::string line{ indent };
		line += "{";
		lines.push_back(line);
	}
	{
		std::string line{indent + indent};
		line += _wrapperDirectory.getKotlinWrapperClassName();
		line += ".getInstance().";
		line += createWrapperScope(clazz);
		line += "release(_handle)";
		lines.push_back(line);
	}
	{
		std::string line{ indent + indent };
		line += "_handle = 0L";
		lines.push_back(line);
	}
	{
		std::string line{ indent };
		line += "}";
		lines.push_back(line);
	}

	lines.push_back("}");
	lines.push_back("protected fun finalize() = close()");
	return lines;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createInterfaceMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object)
{
	std::vector<std::string> lines{};

	{
		std::string line = "fun ";
		line += object.name;
		line += "(";
		line += createParametersDefinition(object);
		line += ") : ";
		line += object.type->toKotlinType();
		lines.push_back(line);
	}
	return lines;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createClassMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object, int number)
{
	std::vector<std::string> lines{};

	{
		std::string line = "fun ";
		line += object.name;
		line += "(";
		line += createParametersDefinition(object);
		line += ") : ";
		line += object.type->toKotlinType();
		lines.push_back(line);
	}
	lines.push_back("{");
	{
		std::string indent = "\t";
		for (auto& parameter : object.parameters)
		{
			lines.push_back(indent + createInputParameterChanger(*parameter));
		}
		lines.push_back(indent + callClassMethod(clazz, object, number));
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

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createDerivedMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object, int number)
{
	auto lines = createClassMethodDefinition(clazz, object, number);
	lines[0] = "override " + lines[0];
	return lines;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createStaticMethodDefinition(const SymbolMethod& object, int number)
{
	std::vector<std::string> lines{};

	{
		std::string line = "fun ";
		line += object.name;
		line += "(";
		line += createParametersDefinition(object);
		line += ") : ";
		line += object.type->toKotlinType();
		lines.push_back(line);
	}
	lines.push_back("{");
	{
		std::string indent = "\t";
		for (auto& parameter : object.parameters)
		{
			lines.push_back(indent + createInputParameterChanger(*parameter));
		}
		lines.push_back(indent + callStaticMethod(object, number));
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

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::callClassMethod(const SymbolClass& clazz, const SymbolMethod& object, int number)
{
	std::string ret;
	if (object.type->getTypeName() != SymbolType::Name::VOID)
	{
		ret += "val __temp_ret = ";
	}
	ret += _wrapperDirectory.getKotlinWrapperClassName();
	ret += ".getInstance().";
	ret += createWrapperScope(clazz);
	ret += object.name;
	if (number > 0)
	{
		ret += std::to_string(number);
	}
	ret += "(_handle";
	if (!object.parameters.empty())
	{
		ret += ", ";
		for (auto& parameter : object.parameters)
		{
			ret += "i_";
			ret += parameter->name;
			ret += ", ";
		}
		ret.pop_back();
		ret.pop_back();
	}
	ret += ")";
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::callStaticMethod(const SymbolMethod& object, int number)
{
	std::string ret;
	if (object.type->getTypeName() != SymbolType::Name::VOID)
	{
		ret += "val __temp_ret = ";
	}
	ret += _wrapperDirectory.getKotlinWrapperClassName();
	ret += ".getInstance().";
	ret += createWrapperScope(object);
	ret += object.name;
	if (number > 0)
	{
		ret += std::to_string(number);
	}
	ret += "(";
	if (!object.parameters.empty())
	{
		for (auto& parameter : object.parameters)
		{
			ret += "i_";
			ret += parameter->name;
			ret += ", ";
		}
		ret.pop_back();
		ret.pop_back();
	}
	ret += ")";
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::createReturnValueChanger(const SymbolMethod& object)
{
	std::string ret{};

	switch (object.type->getTypeName())
	{
	case SymbolType::Name::ENUM:
		ret = "val __ret = enumValues<";
		ret += object.type->toKotlinType();
		ret += ">().find { it.value == __temp_ret}!!";
		break;
	case SymbolType::Name::ENUMARRAY:
		ret = "val __ret = Array(__temp_ret.size) { idx -> enumValues<";
		ret += object.type->toKotlinInnerType();
		ret += ">().find { it.value == __temp_ret[idx]}!! }";
		break;
	case SymbolType::Name::ENUMVECTOR:
		ret = "val __ret = MutableList(__temp_ret.size) { idx -> enumValues<";
		ret += object.type->toKotlinInnerType();
		ret += ">().find { it.value == __temp_ret[idx]}!! }";
		break;
	case SymbolType::Name::OBJECT:
		ret = "val __ret = ";
		ret += object.type->toKotlinType();
		ret += "(__temp_ret)";
		break;
	case SymbolType::Name::OBJECTARRAY:
		ret = "val _value = Array(__temp_ret.size) {";
		ret += object.type->toKotlinInnerType();
		ret += "(__temp_ret[it]) }";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ret = "val _value = MutableList(__temp_ret.size) {";
		ret += object.type->toKotlinInnerType();
		ret += "(__temp_ret[it]) }";
		break;
	default:
		ret = "val __ret = __temp_ret";
		break;
	}

	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::createInputParameterChanger(const SymbolParameter& object)
{
	std::string ret{};

	switch (object.type->getTypeName())
	{
	case SymbolType::Name::ENUM:
		ret = "val i_";
		ret += object.name;
		ret += " = ";
		ret += object.name;
		ret += ".value";
		break;
	case SymbolType::Name::ENUMARRAY:
		ret = "val i_";
		ret += object.name;
		ret += " = ";
		ret += "IntArray(";
		ret += object.name;
		".size) { ";
		ret += object.name;
		ret += "[it].value }";
		break;
	case SymbolType::Name::ENUMVECTOR:
		ret = "val i_";
		ret += object.name;
		ret += " = ";
		ret += "MutableList(";
		ret += object.name;
		ret += ".size) { ";
		ret += object.name;
		ret += "[it].value }";
		break;
	case SymbolType::Name::OBJECT:
		ret = "val i_";
		ret += object.name;
		ret += " = ";
		ret += object.name;
		ret += ".getNativeHandle()";
		break;
	case SymbolType::Name::OBJECTARRAY:
		ret = "val i_";
		ret += object.name;
		ret += " = ";
		ret += "IntArray(";
		ret += object.name;
		".size) { ";
		ret += object.name;
		ret += "[it].getNativeHandle() }";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ret = "val i_";
		ret += object.name;
		ret += " = ";
		ret += "MutableList(";
		ret += object.name;
		ret += ".size) { ";
		ret += object.name;
		ret += "[it].getNativeHandle() }";
		break;
	default:
		ret += "val i_";
		ret += object.name;
		ret += " = ";
		ret += object.name;
		break;
	}

	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::createOutputParameterChanger(const SymbolParameter& object)
{
	std::string ret{};

	switch (object.type->getTypeName())
	{
	case SymbolType::Name::ENUM:
		ret += object.name;
		ret += " = enumValues<";
		ret += object.type->toKotlinType();
		ret += ">().find { it.value == i_";
		ret += object.name;
		ret += "}!!";
		break;
	case SymbolType::Name::ENUMARRAY:
		ret = object.name;
		ret += ".clear(); ";

		ret += "for (value in i_" + object.name;
		ret += ") {";
		ret += object.name;
		ret += ".add(enumValues<";
		ret += object.type->toKotlinType();
		ret += ">().find { it.value == value }";
		ret += ")";
		ret += " }";

		break;
	case SymbolType::Name::ENUMVECTOR:
		ret = object.name;
		ret += ".clear(); ";

		ret += "for (value in i_" + object.name;
		ret += ") {";
		ret += object.name;
		ret += ".add(enumValues<";
		ret += object.type->toKotlinType();
		ret += ">().find { it.value == value }";
		ret += ")";
		ret += " }";

		break;
	case SymbolType::Name::OBJECT:
		ret = object.name;
		ret += " = ";
		ret += object.type->toKotlinType();
		ret += "(i_";
		ret += object.name;
		ret += ")";
		break;
	case SymbolType::Name::OBJECTARRAY:
		ret = object.name;
		ret += ".clear(); ";

		ret += "for (value in i_" + object.name;
		ret += ") {";
		ret += object.name;
		ret += ".add(";
		ret += object.type->toKotlinInnerType();
		ret += "(value)";
		ret += ")";
		ret += " }";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ret = object.name;
		ret += ".clear(); ";

		ret += "for (value in i_" + object.name;
		ret += ") {";
		ret += object.name;
		ret += ".add(";
		ret += object.type->toKotlinInnerType();
		ret += "(value)";
		ret += ")";
		ret += " }";
		break;
	default:
		ret = object.name;
		ret += " = i_";
		ret += object.name;
	}

	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::createParametersDefinition(const SymbolMethod& object)
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

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::createParameterDefinition(const SymbolParameter& object)
{
	std::string ret;
	ret = object.name;
	ret += " : ";
	ret += object.type->toKotlinType();
	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::createInputPropertyChanger(const SymbolProperty& object)
{
	std::string ret{};

	switch (object.type->getTypeName())
	{
	case SymbolType::Name::ENUM:
		ret = "val __value = value.value";
		break;
	case SymbolType::Name::ENUMARRAY:
		ret = "val __value = IntArray(value.size) {value[it].value }";
		break;
	case SymbolType::Name::ENUMVECTOR:
		ret = "val __value = MutableList(value.size) {value[it].value}";
		break;
	case SymbolType::Name::OBJECT:
		ret = "val __value = value.getNativeHandle()";
		break;
	case SymbolType::Name::OBJECTARRAY:
		ret = "val __value = LongArray(value.size) {value[it].getNativeHandle()}";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ret = "val __value = MutableList(value.size) {value[it].getNativeHandle()}";
		break;
	default:
		ret = "val __value = value";
		break;
	}

	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::createOutputPropertyChanger(const SymbolProperty& object)
{
	std::string ret{};

	switch (object.type->getTypeName())
	{
	case SymbolType::Name::ENUM:
		ret = "val __ret = enumValues<";
		ret += object.type->toKotlinType();
		ret += ">().find { it.value == __temp_ret}!!";
		break;
	case SymbolType::Name::ENUMARRAY:
		ret = "val __ret = Array(__temp_ret.size) { idx -> enumValues<";
		ret += object.type->toKotlinInnerType();
		ret += ">().find { it.value == __temp_ret[idx]}!! }";
		break;
	case SymbolType::Name::ENUMVECTOR:
		ret = "val __ret = MutableList(__temp_ret.size) { idx -> enumValues<";
		ret += object.type->toKotlinInnerType();
		ret += ">().find { it.value == __temp_ret[idx]}!! }";
		break;
	case SymbolType::Name::OBJECT:
		ret = "val __ret = ";
		ret += object.type->toKotlinType();
		ret += "(__temp_ret)";
		break;
	case SymbolType::Name::OBJECTARRAY:
		ret = "val __ret = Array(__temp_ret.size) {";
		ret += object.type->toKotlinInnerType();
		ret += "(__temp_ret[it]) }";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ret = "val __ret = MutableList(__temp_ret.size) {";
		ret += object.type->toKotlinInnerType();
		ret += "(__temp_ret[it]) }";
		break;
	default:
		ret = "val __ret = __temp_ret";
		break;
	}

	return ret;
}

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::createPropertyName(const SymbolProperty& object)
{
	std::string propertyName = object.name;
	char firstChar = propertyName[0];
	if ('a' <= firstChar && firstChar <= 'z')
		firstChar += ('A' - 'a');
	propertyName[0] = firstChar;
	return propertyName;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createPropertySetterDefinition(const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object)
{
	std::vector<std::string> lines{};

	lines.push_back("\tset(value) {");
	{
		lines.push_back("\t\t" + createInputPropertyChanger(object));
	}
	{
		std::string line = "\t\t";
		line += _wrapperDirectory.getKotlinWrapperClassName();
		line += ".getInstance().";
		line += createWrapperScope(clazz);
		line += "set";
		line += propertyName;
		line += "(_handle, __value)";
		lines.push_back(line);
	}
	lines.push_back("\t}");

	return lines;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createPropertyGetterDefinition(const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object)
{
	std::vector<std::string> lines{};
	lines.push_back("\tget() {");
	{
		std::string line = "\t\tval __temp_ret = ";
		line += _wrapperDirectory.getKotlinWrapperClassName();
		line += ".getInstance().";
		line += createWrapperScope(clazz);
		line += "get";
		line += propertyName;
		line += "(_handle)";
		lines.push_back(line);
	}
	{
		lines.push_back("\t\t" + createOutputPropertyChanger(object));
	}
	lines.push_back("\t\treturn __ret");
	lines.push_back("\t}");
	return lines;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createInterfacePropertyDefinition(const SymbolClass& clazz, const SymbolProperty& object)
{
	std::vector<std::string> lines{};
	std::string propertyName = createPropertyName(object);
	if (object.readonly)
	{
		{
			std::string line = "val ";
			line += object.name;
			line += " : ";
			line += object.type->toKotlinType();
			lines.push_back(line);
		}
	}
	else
	{
		{
			std::string line = "var ";
			line += object.name;
			line += " : ";
			line += object.type->toKotlinType();
			lines.push_back(line);
		}
	}
	return lines;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createClassPropertyDefinition(const SymbolClass& clazz, const SymbolProperty& object)
{
	std::vector<std::string> lines{};
	std::string propertyName = createPropertyName(object);

	if (object.readonly)
	{
		{
			std::string line = "val ";
			line += object.name;
			line += " : ";
			line += object.type->toKotlinType();
			lines.push_back(line);
		}
		{
			auto inner_lines = createPropertyGetterDefinition(propertyName, clazz, object);
			for (auto& line : inner_lines)
			{
				lines.push_back(line);
			}
		}
	}
	else
	{
		{
			std::string line = "var ";
			line += object.name;
			line += " : ";
			line += object.type->toKotlinType();
			lines.push_back(line);
		}
		{
			auto inner_lines = createPropertyGetterDefinition(propertyName, clazz, object);
			for (auto& line : inner_lines)
			{
				lines.push_back(line);
			}
		}
		{
			auto inner_lines = createPropertySetterDefinition(propertyName, clazz, object);
			for (auto& line : inner_lines)
			{
				lines.push_back(line);
			}
		}
	}
	return lines;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createDerivedPropertyDefinition(const SymbolClass& clazz, const SymbolProperty& object)
{
	std::vector<std::string> lines = createClassPropertyDefinition(clazz, object);
	lines[0] = "override " + lines[0];
	return lines;
}
*/