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

		createNativeHandle(ss);
		createReleaser(ss);

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
				createClassMethodDefinition(ss, object, *method, method_number, true);
			}
		}

		for (auto& member_method : object.methods)
		{
			auto& method = member_method.first;
			auto& method_number = member_method.second;

			{
				createKotlinComment(ss, *method);
				createClassMethodDefinition(ss, object, *method, method_number, false);
			}
		}

		auto base_properties = object.getBaseProperties();
		for (auto& base_prop : base_properties)
		{
			createKotlinComment(ss, *base_prop);
			createClassPropertyDefinition(ss, object, *base_prop, true);
		}

		for (auto& prop : object.properties)
		{
			createKotlinComment(ss, *prop);
			createClassPropertyDefinition(ss, object, *prop, false);
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

void LibraryInterfaceGenerator::Implementation::SourceDirectory::createReleaser(SourceStream& ss)
{
	ss << "private val releaser : (() -> Unit) = close()\n";
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
	return ParameterNode(ParameterNode::VALUE, "Any", "this");
}
static std::vector<ParameterNode> createInputConstructorParameter(const SymbolMethod& method)
{
	std::vector<ParameterNode> ret;
	for (auto& param : method.parameters)
	{
		std::string method_name = "i_" + method.name;
		ret.push_back(ParameterNode(ParameterNode::VALUE, param->type->toKotlinType(), method_name));
	}
	return ret;
}
static std::vector<ParameterNode> createInputDestructorParameter()
{
	std::vector<ParameterNode> ret;
	ret.push_back(createInputHandleParameter());
	return ret;
}
static std::vector<ParameterNode> createInputAddReleaserParameter()
{
	std::vector<ParameterNode> ret;
	ret.push_back(ParameterNode(ParameterNode::VALUE, "", "releaser"));
	return ret;
}
static std::vector<ParameterNode> createInputStaticParamter(const SymbolMethod& method)
{
	std::vector<ParameterNode> ret;
	for (auto& param : method.parameters)
	{
		ret.push_back(ParameterNode(ParameterNode::VALUE, param->type->toKotlinType(), param->name));
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
static std::vector<ParameterNode> createInputPropertyParameter()
{
	std::vector<ParameterNode> ret;
	ret.push_back(createInputHandleParameter());
	ret.push_back(ParameterNode(ParameterNode::VALUE, "", "value"));
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

		callConstructor(ss, clazz, constructor, number);

		ss << "require(_nativeHandle != 0L)\n";

		callAddReleaser(ss, clazz);
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
	{
		MethodKotlinSourceScopedStream method{
			ss,
			false,
			KotlinAccess::PUBLIC,
			"override",
			"",
			"",
			"close",
			{}
		};

		ss << "if (_handle != 0L)\n";
		{
			SourceScopedStream if_scope(ss, CodeStyle::Kotlin);

			callDestructor(ss, clazz);

			ss << "_handle = 0L\n";
		}
	}
	{
		MethodKotlinSourceScopedStream method{
			ss,
			true,
			KotlinAccess::PROTECTED,
			"",
			"= close()",
			"",
			"finalize",
			{}
		};
	}
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::createInterfaceMethodDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object)
{
	{
		MethodKotlinSourceScopedStream method{
			ss,
			true,
			KotlinAccess::PUBLIC,
			"",
			"",
			"",
			object.name,
			createParameters(object)
		};
	}
}
void LibraryInterfaceGenerator::Implementation::SourceDirectory::createClassMethodDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object, int number, bool is_overrided)
{	
	{
		MethodKotlinSourceScopedStream method{
			ss,
			false,
			KotlinAccess::PUBLIC,
			(is_overrided ? "override" : ""),
			"",
			object.type->toKotlinType(),
			object.name,
			createParameters(object)
		};

		
		callClassMethod(ss, clazz, object, number);
	}
}
void LibraryInterfaceGenerator::Implementation::SourceDirectory::createStaticMethodDefinition(SourceStream& ss, const SymbolMethod& object, int number)
{
	{
		MethodKotlinSourceScopedStream method{
			ss,
			false,
			KotlinAccess::PUBLIC,
			"",
			"",
			object.type->toKotlinType(),
			object.name,
			createParameters(object)
		};

		
		callStaticMethod(ss, object, number);
		
	}
}

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::createMethodName(const SymbolClass& clazz, const std::string& method_name, int number)
{
	std::stringstream ss;
	ss << _wrapperDirectory.getKotlinWrapperClassName() << ".getInstance()." << createKotlinWrapperScope(clazz) << method_name;
	if (number != 0)
		ss << number;
	return ss.str();
}

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::createMethodName(const SymbolMethod& method, const std::string& method_name, int number)
{
	std::stringstream ss;
	ss << _wrapperDirectory.getKotlinWrapperClassName() << ".getInstance()." << createKotlinWrapperScope(method) << method_name;
	if (number != 0)
		ss << number;
	return ss.str();
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::callConstructor(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object, int number)
{
	{		
		ss << "_nativeHandle = ";
		
		CallKotlinSourceScopedStream call{
			ss,
			"",
			{},
			createMethodName(clazz, "consturctor", number),
			createInputConstructorParameter(object)
		};
	}
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::callDestructor(SourceStream& ss, const SymbolClass& clazz)
{
	{
		CallKotlinSourceScopedStream call{
			ss,
			"",
			{},
			createMethodName(clazz, "release", 0),
			createInputDestructorParameter()
		};
	}
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::callAddReleaser(SourceStream& ss, const SymbolClass& clazz)
{
	{
		CallKotlinSourceScopedStream call{
			ss,
			"",
			{},
			createMethodName(clazz, "addReleaser", 0),
			createInputAddReleaserParameter()
		};
	}
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::callClassMethod(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object, int number)
{
	{
		if (object.type->getTypeName() != SymbolType::Name::VOID)
		{
			ss << "return ";
		}
		CallKotlinSourceScopedStream call{
			ss,
			"",
			{},
			createMethodName(clazz, object.name, number),
			createInputMemberParamter(object)
		};
	}
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::callStaticMethod(SourceStream& ss, const SymbolMethod& object, int number)
{
	{
		if (object.type->getTypeName() != SymbolType::Name::VOID)
		{
			ss << "return ";
		}

		CallKotlinSourceScopedStream call{
			ss,
			"",
			{},
			createMethodName(object, object.name, number),
			createInputStaticParamter(object)
		};
	}
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

void LibraryInterfaceGenerator::Implementation::SourceDirectory::createInterfacePropertyDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolProperty& object)
{
	std::string propertyName = createPropertyName(object);
	{
		PropertyKotlinSourceScopedStream prop{
				ss,
				KotlinAccess::PUBLIC,
				"",
				object.type->toKotlinType(),
				object.name,
				object.readonly
		};
	}
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::createClassPropertyDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolProperty& object, bool is_overrided)
{
	std::string propertyName = createPropertyName(object);
	{
		PropertyKotlinSourceScopedStream prop{
				ss,
				KotlinAccess::PUBLIC,
				is_overrided ? "override" : "",
				object.type->toKotlinType(),
				object.name,
				object.readonly
		};
		{
			auto getter = prop.createGetter();
			callPropertyGetter(ss, propertyName, clazz, object);
		}
		if (!object.readonly)
		{
			auto setter = prop.createSetter();
			callPropertySetter(ss, propertyName, clazz, object);
		}
	}
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::callPropertyGetter(SourceStream& ss, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object)
{
	{
		ss << "return ";
		CallKotlinSourceScopedStream call {
			ss,
			"",
			{},
			createMethodName(clazz, "get" + propertyName, 0),
			{}
		};
	}
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::callPropertySetter(SourceStream& ss, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object)
{
	{
		CallKotlinSourceScopedStream call{
			ss,
			"",
			{},
			createMethodName(clazz, "set" + propertyName, 0),
			createInputPropertyParameter()
		};
	}
}