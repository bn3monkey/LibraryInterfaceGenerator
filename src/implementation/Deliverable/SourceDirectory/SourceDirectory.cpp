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

	std::string package_path{ _src_dir_path };
	package_path += delimeter;
	package_path += package.name;

	result = FileSystem::createDirectory(package_path);
	if (!result)
		return result;

	for (auto& submodule : modules)
	{
		result = createModule(*submodule, package_path);
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

	auto& callbacks = object.callbacks;
	for (auto& callback : callbacks)
	{
		auto result = createCallbackFile(*callback, module_path);
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

Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createCallbackFile(const SymbolCallback& object, std::string& parent_path)
{
	std::string header_path{ parent_path };
	header_path += delimeter;
	header_path += object.name;
	header_path += ".kt";

	Result result;

	auto ss = createCallbackFileContent(object);
	auto header_content = ss.str();

	result = FileSystem::createFile(header_path, header_content);
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
		ImportKotlinSourceStream functionImport(ss, _wrapperDirectory.getKotlinPackageName(), {"*"});
	}
	{
		ImportKotlinSourceStream wrapperImport(ss, _wrapperDirectory.getKotlinPackageName(), { _wrapperDirectory.getKotlinWrapperClassName() });
	}
	{
		createForwardDeclaration(ss, object);
	}

	{
		createKotlinComment(ss, object);
		InterfaceKotlinSourceScopedStream interfaze(ss, object.name, {"WrapperTypeObject"});
		
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
		ImportKotlinSourceStream functionImport(ss, _wrapperDirectory.getKotlinPackageName(), { "*" });
	}
	{
		ImportKotlinSourceStream wrapperImport(ss, _wrapperDirectory.getKotlinPackageName(), { _wrapperDirectory.getKotlinWrapperClassName() });
	}
	{
		createForwardDeclaration(ss, object);
	}
		

	{
		createKotlinComment(ss, object);
		
		std::vector<std::string> base_classes = { "WrapperTypeObjectImpl" };
		for (auto& base : object.bases)
		{
			if (auto& pBase = base.lock())
			{
				base_classes.push_back(pBase->name);
			}
		}		
		ClassKotlinSourceScopedStream clazz(ss, object.name, base_classes);

		createPrimaryConstructorDefinition(ss, object);
		for (auto& constructor : object.constructors)
		{
			auto method = constructor.first;
			auto method_number = constructor.second;
			{
				createSecondaryConstructorDefinition(ss, object, *method);
			}
		}
		
		{
			CompanionObjectKotlinSourceScopedStream co{ss};
			for (auto& constructor : object.constructors)
			{
				auto method = constructor.first;
				auto method_number = constructor.second;
				{
					createKotlinComment(ss, *method);
					createConstructorDefinition(ss, object, *method, method_number);
				}
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
		ImportKotlinSourceStream functionImport(ss, _wrapperDirectory.getKotlinPackageName(), { "*" });
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
		ImportKotlinSourceStream functionImport(ss, _wrapperDirectory.getKotlinPackageName(), { "*" });
	}
	{
		ImportKotlinSourceStream wrapperImport(ss, _wrapperDirectory.getKotlinPackageName(), { _wrapperDirectory.getKotlinWrapperClassName() });
	}
	{
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

SourceStream LibraryInterfaceGenerator::Implementation::SourceDirectory::createCallbackFileContent(const SymbolCallback& callback)
{
	SourceStream ss;

	{
		PackageKotlinSourceStream package(ss, _wrapperDirectory.getKotlinPackageName(), callback.parentModules);
	}
	{
		createForwardDeclaration(ss, callback);
	}
	{		
		createKotlinComment(ss, callback);
		createCallbackDefinition(ss, callback);
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

void LibraryInterfaceGenerator::Implementation::SourceDirectory::createForwardDeclaration(SourceStream& ss, const SymbolCallback& object)
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

	{
		auto callback_objects = object.collectAllCallbackReference();
		for (auto& w_callback_object : callback_objects)
		{
			if (auto callback_object = w_callback_object.lock())
			{
				auto callback = std::dynamic_pointer_cast<SymbolCallback>(callback_object);
				std::vector<std::string> paths;
				{
					auto& parent_modules = callback->parentModules;
					paths.insert(paths.end(), parent_modules.begin(), parent_modules.end());
				}
				paths.push_back(callback->name);

				ImportKotlinSourceStream importClass{
					ss, _wrapperDirectory.getKotlinPackageName(), paths
				};
			}
		}
	}
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::createEnumDefinition(SourceStream& ss, const SymbolEnum& object)
{
	if (object.keys_to_names.empty())
	{	
		EnumKotlinSourceScopedStream s{
			ss, object.name, {"WrapperTypeEnum"}
		};
		for (auto& element : object.keys_to_values)
		{
			auto key = element.first;
			auto value = element.second;

			s.addElement(key, value);
		}
		ss.pop(2);
		ss << ";\n\n";


		{
			MethodKotlinSourceScopedStream toWrapperType{
				ss,
				false,
				KotlinAccess::PUBLIC,
				"override",
				"",
				"Int",
				"toWrapperType",
				{}
			};
			ss << "return value\n";
		}
	}
	else
	{
		AdvancedEnumKotlinSourceScopedStream s{
			ss, object.name, {"WrapperTypeEnum"}
		};
		for (auto& element : object.keys_to_values)
		{
			auto key = element.first;
			auto value = element.second;
			auto name = object.keys_to_names.at(key);

			s.addElement(key, value, name);
		}
		ss.pop(2);
		ss << ";\n\n";

		{
			MethodKotlinSourceScopedStream toWrapperType{
				ss,
				false,
				KotlinAccess::PUBLIC,
				"override",
				"",
				"Int",
				"toWrapperType",
				{}
			};
			ss << "return value\n";
		}
	}
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::createCallbackDefinition(SourceStream& ss, const SymbolCallback& callback)
{
	std::vector<std::string> param_types;
	for (auto& param : callback.parameters)
	{
		param_types.push_back(param->type->toKotlinType());
	}

	{
		CallbackKotlinSourceStream callback_scope{
			ss,
			callback.name,
			callback.type->toKotlinType(),
			param_types
		};
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
		io = ParameterNode::REFERENCE_IN;
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

static ParameterNode createInputHandleParameter()
{
	return ParameterNode(ParameterNode::VALUE, "Long", "self");
}

static std::vector<ParameterNode> createPrimaryConstructorParameters() {
	return { ParameterNode(ParameterNode::VALUE, "NativeHandle", "handle") };
}
static std::vector<ParameterNode> createInputPrimaryConstructorParameters() {
	return { ParameterNode(ParameterNode::VALUE, "Long", "handle.get()") };
}

static std::vector<ParameterNode> createConstructorParameter(const SymbolMethod& method)
{
	std::vector<ParameterNode> ret;
	for (auto& param : method.parameters)
	{
		ret.push_back(ParameterNode(ParameterNode::VALUE, param->type->toKotlinType(), param->name));
	}
	return ret;
}
static std::vector<ParameterNode> createInputConstructorParameter(const SymbolMethod& method)
{
	std::vector<ParameterNode> ret;
	for (auto& param : method.parameters)
	{
		std::string param_name = "i_" + param->name;
		ret.push_back(ParameterNode(ParameterNode::VALUE, param->type->toKotlinType(), param_name));
	}
	return ret;
}

static std::vector<ParameterNode> createDestructorParameter()
{
	return { ParameterNode(ParameterNode::VALUE, "NativeHandle", "handle") };
}
static std::vector<ParameterNode> createInputDestructorParameter()
{
	return { ParameterNode(ParameterNode::VALUE, "Long", "handle.get()") };
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
		ret.push_back(ParameterNode(ParameterNode::VALUE, param->type->toKotlinType(), "i_" + param->name));
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
	ret.push_back(ParameterNode(ParameterNode::VALUE, "", "i_value"));
	return ret;
}


void LibraryInterfaceGenerator::Implementation::SourceDirectory::createPrimaryConstructorDefinition(SourceStream& ss, const SymbolClass& clazz)
{
	{
		MethodKotlinSourceScopedStream method{
			ss,
			false,
			KotlinAccess::INTERNAL,
			"",
			": super(handle)",
			"",
			"constructor",
			createPrimaryConstructorParameters()
		};

		callAddReleaser(ss, clazz);
	}
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::createSecondaryConstructorDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& constructor)
{
	{
		MethodKotlinSourceScopedStream method{
			ss,
			true,
			KotlinAccess::PUBLIC,
			"",
			"",
			"",
			"constructor",
			createParameters(constructor)
		};
		ss << " : this(";
		CallKotlinSourceScopedStream construct {
			ss,
			"",
			{},
			"construct",
			createParameters(constructor)
		};
		ss.pop(1);
		ss << ")\n";
	}
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
			"NativeHandle",
			"construct",
			createConstructorParameter(constructor)
		};

		for (auto& parameter : constructor.parameters)
		{
			createInputParameterChanger(ss, *parameter);
		}
		callConstructor(ss, clazz, constructor, number);

		ss << "return NativeHandle(ret)\n";
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
			"release",
			{ParameterNode(ParameterNode::VALUE, "NativeHandle", "handle")}
		};

		callDestructor(ss, clazz);
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
			object.type->toKotlinType(),
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

		for (auto& parameter : object.parameters)
		{
			createInputParameterChanger(ss, *parameter);
		}
		callClassMethod(ss, clazz, object, number);

		createReturnValueChanger(ss, object);
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



		for (auto& parameter : object.parameters)
		{
			createInputParameterChanger(ss, *parameter);
		}
		callStaticMethod(ss, object, number);

		createReturnValueChanger(ss, object);
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
		ss << "val ret = ";
		CallKotlinSourceScopedStream call{
			ss,
			"",
			{},
			createMethodName(clazz, "construct", number),
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
			ss << "val temp_ret = ";
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
			ss << "val temp_ret = ";
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

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::findConverter(SymbolType& type)
{
	switch (type.getTypeName())
	{
		case SymbolType::Name::VOID:
		case SymbolType::Name::BOOL:
		case SymbolType::Name::INT8:
		case SymbolType::Name::INT16:
		case SymbolType::Name::INT32:
		case SymbolType::Name::INT64:
		case SymbolType::Name::FLOAT:
		case SymbolType::Name::DOUBLE:
		case SymbolType::Name::STRING:
		case SymbolType::Name::ENUM:
		case SymbolType::Name::OBJECT:
		case SymbolType::Name::CALLBACK:
			return type.toKotlinType();
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
		{
			auto element_types = type.toElementTypes();
			auto element_type = element_types[0];
			return element_type->toKotlinType();
		}
	}
	return "";
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::createReturnValueChanger(SourceStream& ss, const SymbolMethod& object)
{
	if (object.type)
	{
		if (object.type->getTypeName() != SymbolType::Name::VOID)
		{
			ss << "val ret = toKotlinType<" << findConverter(*object.type) << ">(temp_ret)\n";
			ss << "return ret;\n";
		}
	}
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::createInputParameterChanger(SourceStream& ss, const SymbolParameter& object)
{
	if (object.type)
	{
		ss << "val i_" << object.name << " = toKotlinWrapperType(" << object.name << ")\n";
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
			createOutputPropertyChanger(ss, object);
		}
		if (!object.readonly)
		{
			auto setter = prop.createSetter();
			createInputPropertyChanger(ss, object);
			callPropertySetter(ss, propertyName, clazz, object);
		}
	}
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::createInputPropertyChanger(SourceStream& ss, const SymbolProperty& object)
{
	if (object.type)
	{
		ss << "val i_value = toKotlinWrapperType(value)\n";
	}
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::createOutputPropertyChanger(SourceStream& ss, const SymbolProperty& object)
{
	if (object.type)
	{
		ss << "val ret = toKotlinType<" << findConverter(*object.type) << ">(temp_ret)\n";
		ss << "return ret\n";
	} 
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::callPropertyGetter(SourceStream& ss, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object)
{
	{
		ss << "val temp_ret = ";
		CallKotlinSourceScopedStream call {
			ss,
			"",
			{},
			createMethodName(clazz, "get" + propertyName, 0),
			{
				createInputHandleParameter()
			}
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