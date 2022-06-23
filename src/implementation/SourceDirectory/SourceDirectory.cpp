#include "SourceDirectory.hpp"

#ifdef __linux__
static char* delimeter = "/";
#elif _WIN32
static char* delimeter = "\\";
#else
static char* delimeter = "/";
#endif


static const std::vector<std::string> handleCode = {
	"internal constructor(handle : Long) {",
	"\t_handle = handle",
	"\trequire(_handle != 0L)",
	"}",
	"private var _handle : Long",
	"internal fun getNativeHandle() : Long = _handle",
};

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

	auto& global_methods = object.globla_methods;
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

	Result result;
	std::string content;
	result = createInterfaceFileContent(object, content);
	if (!result)
		return result;

	result = FileSystem::createFile(header_path, content);
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

	Result result;
	std::string content;
	result = createClassFileContent(object, content);
	if (!result)
		return result;

	result = FileSystem::createFile(header_path, content);
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

	Result result;
	std::string content;
	result = createEnumFileContent(object, content);
	if (!result)
		return result;

	result = FileSystem::createFile(header_path, content);
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

	Result result;
	std::string content;
	result = createMethodFileContent(object, content);
	if (!result)
		return result;

	result = FileSystem::createFile(header_path, content);
	if (!result)
		return result;

	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createInterfaceFileContent(const SymbolClass& object, std::string& content)
{
	std::stringstream ss;
	std::string indent;

	{
		ss << "package " << _wrapperDirectory.getKotlinPackageName();
		for (size_t i = 1; i < object.parentModules.size(); i++)
			ss << "." << object.parentModules[i];
		ss << "\n";

		ss << "import " << _wrapperDirectory.getKotlinPackageName() << "." << _wrapperDirectory.getKotlinWrapperClassName() << "\n";
		//ss << "import java.lang.AutoCloseable\n";
		addForwardDeclaration(ss, indent, object);

		ss << "interface " << object.name << "\n";
		ss << "{\n";
		{
			indent += "\t";
			/*
			for (auto& constructor : object.constructors)
			{
				{
					Comment comment{ ss, indent };
					comment.add(*constructor);
				}
				auto lines = createConstructorDefinition(object, *constructor);
				for (auto& line : lines)
				{
					ss << indent << line << "\n";
				}
			}

			{
				for (const auto& line : handleCode)
					ss << indent << line << "\n";
			}

			{
				auto lines = createDestructorDefinition(object);
				for (auto& line : lines)
				{
					ss << indent << line << "\n";
				}
			}
			*/

			for (auto& method : object.methods)
			{
				{
					Comment comment{ ss, indent };
					comment.add(*method);
				}
				auto lines = createInterfaceMethodDefinition(object, *method);
				for (auto& line : lines)
				{
					ss << indent << line << "\n";
				}
			}

			for (auto& prop : object.properties)
			{
				{
					Comment comment{ ss, indent };
					comment.add(*prop);
				}
				auto lines = createInterfacePropertyDefinition(object, *prop);
				for (auto& line : lines)
				{
					ss << indent << line << "\n";
				}
			}

			indent.pop_back();
		}
		ss << "}";
	}
	content = ss.str();

	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createClassFileContent(const SymbolClass& object, std::string& content)
{
	std::stringstream ss;
	std::string indent;

	{
		ss << "package " << _wrapperDirectory.getKotlinPackageName();
		for (size_t i = 1; i < object.parentModules.size(); i++)
			ss << "." << object.parentModules[i];
		ss << "\n";

		ss << "import " << _wrapperDirectory.getKotlinPackageName() << "." << _wrapperDirectory.getKotlinWrapperClassName() << "\n";
		ss << "import java.lang.AutoCloseable\n";
		addForwardDeclaration(ss, indent, object);

		ss << "class " << object.name << " : AutoCloseable";

		for (auto& base : object.bases)
		{
			if (auto & pBase = base.lock())
			{
				ss << ", " << pBase->name;
			}
		}

		ss << "\n";
		ss << "{\n";
		{
			indent += "\t";
			for (auto& constructor : object.constructors)
			{
				{
					Comment comment{ ss, indent };
					comment.add(*constructor);
				}
				auto lines = createConstructorDefinition(object, *constructor);
				for (auto& line : lines)
				{
					ss << indent << line << "\n";
				}
			}

			{
				for (const auto& line : handleCode)
					ss << indent << line << "\n";
			}

			{
				auto lines = createDestructorDefinition(object);
				for (auto& line : lines)
				{
					ss << indent << line << "\n";
				}
			}

			auto baseMethods = object.getBaseMethods();
			for (auto& method : baseMethods)
			{
				{
					Comment comment{ ss, indent };
					comment.add(*method);
				}
				auto lines = createDerivedMethodDefinition(object, *method);
				for (auto& line : lines)
				{
					ss << indent << line << "\n";
				}
			}


			for (auto& method : object.methods)
			{
				{
					Comment comment{ ss, indent };
					comment.add(*method);
				}
				auto lines = createClassMethodDefinition(object, *method);
				for (auto& line : lines)
				{
					ss << indent << line << "\n";
				}
			}

			auto baseProperties = object.getBaseProperties();
			for (auto& prop : baseProperties)
			{
				{
					Comment comment{ ss, indent };
					comment.add(*prop);
				}
				auto lines = createDerivedPropertyDefinition(object, *prop);
				for (auto& line : lines)
				{
					ss << indent << line << "\n";
				}
			}

			for (auto& prop : object.properties)
			{
				{
					Comment comment{ ss, indent };
					comment.add(*prop);
				}
				auto lines = createClassPropertyDefinition(object, *prop);
				for (auto& line : lines)
				{
					ss << indent << line << "\n";
				}
			}

			indent.pop_back();
		}
		ss << "}";
	}
	content = ss.str();

	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createEnumFileContent(const SymbolEnum& object, std::string& content)
{
	std::stringstream ss;
	std::string indent;

	{
		ss << "package " << _wrapperDirectory.getKotlinPackageName();
		for (size_t i = 1; i < object.parentModules.size(); i++)
			ss << "." << object.parentModules[i];
		ss << "\n";

		ss << "import " << _wrapperDirectory.getKotlinPackageName() << "." << _wrapperDirectory.getKotlinWrapperClassName() << "\n";


		{
			{
				Comment comment{ ss, indent };
				comment.add(object);
			}

			auto lines = createEnumDefinition(object);
			for (auto& line : lines)
			{
				ss << indent << line << "\n";
			}
		}
	}
	content = ss.str();

	return Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::SourceDirectory::createMethodFileContent(const SymbolModule& object, std::string& content)
{
	std::stringstream ss;
	std::string indent;

	{
		ss << "package " << _wrapperDirectory.getKotlinPackageName();
		for (size_t i = 1; i < object.moduleNames.size(); i++)
			ss << "." << object.moduleNames[i];
		ss << "\n";

		ss << "import " << _wrapperDirectory.getKotlinPackageName() << "." << _wrapperDirectory.getKotlinWrapperClassName() << "\n";
		addForwardDeclaration(ss, indent, object);

		{
			for (auto& method : object.globla_methods)
			{
				{
					Comment comment{ ss, indent };
					comment.add(*method);
				}
				auto lines = createStaticMethodDefinition(*method);
				for (auto& line : lines)
				{
					ss << indent << line << "\n";
				}
			}
		}
	}
	content = ss.str();

	return Result();
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::addForwardDeclaration(std::stringstream& ss, std::string& indent, const SymbolClass& object)
{
	{
		auto baseObjects = object.bases;
		for (auto& wbaseObject : baseObjects)
		{
			if (auto baseObject = wbaseObject.lock())
			{
				std::string objectPath;
				auto& parentModules = baseObject->parentModules;
				for (size_t i = 1; i < parentModules.size(); i++)
				{
					objectPath += ".";
					objectPath += parentModules[i];
				}
				auto& parentObjects = baseObject->parentObjects;
				for (auto& objectName : parentObjects)
				{
					objectPath += ".";
					objectPath += objectName;
				}
				objectPath += ".";
				objectPath += baseObject->name;

				ss << "import " << _wrapperDirectory.getKotlinPackageName() << objectPath << ";\n";
			}
		}
	}
	{
		auto classObjects = object.collectAllClassReference();
		for (auto& wClassObject : classObjects)
		{
			if (auto classObject = wClassObject.lock())
			{
				auto clazz = std::dynamic_pointer_cast<SymbolClass>(classObject);
				std::string objectPath;
				auto& parentModules = clazz->parentModules;
				for (size_t i = 1; i < parentModules.size(); i++)
				{
					objectPath += ".";
					objectPath += parentModules[i];
				}
				auto& parentObjects = clazz->parentObjects;
				for (auto& objectName : parentObjects)
				{
					objectPath += ".";
					objectPath += objectName;
				}
				objectPath += ".";
				objectPath += clazz->name;

				ss << "import " << _wrapperDirectory.getKotlinPackageName() << objectPath << ";\n";
			}
		}
	}

	{
		auto enumObjects = object.collectAllEnumReference();
		for (auto& wEnumObject : enumObjects)
		{
			if (auto enumObject = wEnumObject.lock())
			{
				auto enumm = std::dynamic_pointer_cast<SymbolEnum>(enumObject);
				std::string objectPath;
				auto& parentModules = enumm->parentModules;
				for (size_t i = 1; i < parentModules.size(); i++)
				{
					objectPath += ".";
					objectPath += parentModules[i];
				}
				auto& parentObjects = enumm->parentObjects;
				for (auto& objectName : parentObjects)
				{
					objectPath += ".";
					objectPath += objectName;
				}
				objectPath += ".";
				objectPath += enumm->name;

				ss << "import " << _wrapperDirectory.getKotlinPackageName() << objectPath << ";\n";
			}
		}
	}
	ss << "\n";
}

void LibraryInterfaceGenerator::Implementation::SourceDirectory::addForwardDeclaration(std::stringstream& ss, std::string& indent, const SymbolModule& object)
{
	{
		auto classObjects = object.collectAllClassReference();
		for (auto& wClassObject : classObjects)
		{
			if (auto classObject = wClassObject.lock())
			{
				auto clazz = std::dynamic_pointer_cast<SymbolClass>(classObject);
				std::string objectPath;
				auto& parentModules = clazz->parentModules;
				for (size_t i = 1; i < parentModules.size(); i++)
				{
					objectPath += ".";
					objectPath += parentModules[i];
				}
				auto& parentObjects = clazz->parentObjects;
				for (auto& objectName : parentObjects)
				{
					objectPath += ".";
					objectPath += objectName;
				}
				objectPath += ".";
				objectPath += clazz->name;

				ss << "import " << _wrapperDirectory.getKotlinPackageName() << objectPath << ";\n";
			}
		}
	}

	{
		auto enumObjects = object.collectAllEnumReference();
		for (auto& wEnumObject : enumObjects)
		{
			if (auto enumObject = wEnumObject.lock())
			{
				auto enumm = std::dynamic_pointer_cast<SymbolEnum>(enumObject);
				std::string objectPath;
				auto& parentModules = enumm->parentModules;
				for (size_t i = 1; i < parentModules.size(); i++)
				{
					objectPath += ".";
					objectPath += parentModules[i];
				}
				auto& parentObjects = enumm->parentObjects;
				for (auto& objectName : parentObjects)
				{
					objectPath += ".";
					objectPath += objectName;
				}
				objectPath += ".";
				objectPath += enumm->name;

				ss << "import " << _wrapperDirectory.getKotlinPackageName() << objectPath << ";\n";
			}
		}
	}
	ss << "\n";
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createEnumDefinition(const SymbolEnum& object)
{
	std::vector<std::string> ret;
	{
		ret.push_back("enum class " + object.name + "(val value : Int)");
		ret.push_back("{");
		for (auto& pair : object.values)
		{
			ret.push_back("\t" + pair.first + "( " + pair.second + "),");
		}
		ret.push_back("}");
	}
	return ret;
}


std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createConstructorDefinition(const SymbolClass& clazz, const SymbolMethod& object)
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
			line += "construct(";
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
	}
	lines.push_back("}");
	return lines;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createDestructorDefinition(const SymbolClass& clazz)
{
	std::vector<std::string> lines{};

	{
		lines.push_back("override fun close()");
	}
	lines.push_back("{");
	{
		std::string indent = "\t";
		std::string line;
		line += indent;
		line += _wrapperDirectory.getKotlinWrapperClassName();
		line += ".getInstance().";
		line += createWrapperScope(clazz);
		line += "release(_handle)";
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

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createClassMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object)
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
		lines.push_back(indent + callClassMethod(clazz, object));
		for (auto& parameter : object.parameters)
		{
			if (parameter->io == SymbolParameter::IO::OUT)
				lines.push_back(indent + createOutputParameterChanger(*parameter));
		}
		if (object.type->getTypeName() != SymbolType::Name::VOID)
		{
			lines.push_back(indent + "return __ret;");
		}
	}
	lines.push_back("}");
	return lines;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createDerivedMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object)
{
	auto lines = createClassMethodDefinition(clazz, object);
	lines[0] = "override " + lines[0];
	return lines;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::SourceDirectory::createStaticMethodDefinition(const SymbolMethod& object)
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
		lines.push_back(indent + callStaticMethod(object));
		for (auto& parameter : object.parameters)
		{
			if (parameter->io == SymbolParameter::IO::OUT)
				lines.push_back(indent + createOutputParameterChanger(*parameter));
		}
		if (object.type->getTypeName() != SymbolType::Name::VOID)
		{
			lines.push_back(indent + "return __ret;");
		}
	}
	lines.push_back("}");
	return lines;
}

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::callClassMethod(const SymbolClass& clazz, const SymbolMethod& object)
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

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::callStaticMethod(const SymbolMethod& object)
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
		ret += object.type->toKotlinType();
		ret += ">().find { it.value == __temp_ret[idx]}!! }";
		break;
	case SymbolType::Name::ENUMVECTOR:
		ret = "val __ret = MutableList(__temp_ret.size) { idx -> enumValues<";
		ret += object.type->toKotlinType();
		ret += ">().find { it.value == __temp_ret[idx]}!! }";
		break;
	case SymbolType::Name::OBJECT:
		ret = "val __ret = ";
		ret += object.type->toKotlinType();
		ret += "(__temp_ret)";
		break;
	case SymbolType::Name::OBJECTARRAY:
		ret = "val _value = Array(value.size) {";
		ret += object.type->toKotlinInnerType();
		ret += "(__temp_ret[it]) }";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ret = "val _value = MutableList(value.size) {";
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
		ret += "IntArray(value.size) { ";
		ret += object.name;
		ret += "[it].value }";
		break;
	case SymbolType::Name::ENUMVECTOR:
		ret = "val i_";
		ret += object.name;
		ret += " = ";
		ret += "MutableList(value.size) { ";
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
		ret += "IntArray(value.size) { ";
		ret += object.name;
		ret += "[it].getNativeHandle() }";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ret = "val i_";
		ret += object.name;
		ret += " = ";
		ret += "MutableList(value.size) { ";
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
		ret += " = Array(i_";
		ret += object.name;
		ret += ".size) { idx->enumValues<";
		ret += object.type->toKotlinType();
		ret += ">().find { it.value == i_";
		ret += object.name;
		ret += "[idx] }!! }";
		break;
	case SymbolType::Name::ENUMVECTOR:
		ret = object.name;
		ret += " = MutableList(i_";
		ret += object.name;
		ret += ".size) { idx->enumValues<";
		ret += object.type->toKotlinType();
		ret += ">().find { it.value == i_";
		ret += object.name;
		ret += "[idx] }!! }";
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
		ret += " = Array(value.size) {";
		ret += object.type->toKotlinInnerType();
		ret += "(i_";
		ret += object.name;
		ret += "[it]) }";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ret = object.name;
		ret += " = MutableList(value.size) {";
		ret += object.type->toKotlinInnerType();
		ret += "(i_";
		ret += object.name;
		ret += "[it]) }";
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
		ret += object.type->toKotlinType();
		ret += ">().find { it.value == __temp_ret[idx]}!! }";
		break;
	case SymbolType::Name::ENUMVECTOR:
		ret = "val __ret = MutableList(__temp_ret.size) { idx -> enumValues<";
		ret += object.type->toKotlinType();
		ret += ">().find { it.value == __temp_ret[idx]}!! }";
		break;
	case SymbolType::Name::OBJECT:
		ret = "val __ret = ";
		ret += object.type->toKotlinType();
		ret += "(__temp_ret)";
		break;
	case SymbolType::Name::OBJECTARRAY:
		ret = "val __ret = Array(value.size) {";
		ret += object.type->toKotlinInnerType();
		ret += "(__temp_ret[it]) }";
		break;
	case SymbolType::Name::OBJECTVECTOR:
		ret = "val __ret = MutableList(value.size) {";
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

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::createWrapperScope(const SymbolClass& clazz)
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

std::string LibraryInterfaceGenerator::Implementation::SourceDirectory::createWrapperScope(const SymbolMethod& method)
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
