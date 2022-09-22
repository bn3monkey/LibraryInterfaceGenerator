#include "NativeSourceStream.hpp"
#include "UIDGenerator.hpp"

LibraryInterfaceGenerator::Implementation::HeaderGuardNativeSourceScopedStream::HeaderGuardNativeSourceScopedStream(SourceStream& sourceStream, const std::vector<std::string>& module_names, const std::string& name)
{
}

LibraryInterfaceGenerator::Implementation::HeaderGuardNativeSourceScopedStream::~HeaderGuardNativeSourceScopedStream()
{
}

std::string LibraryInterfaceGenerator::Implementation::HeaderGuardNativeSourceScopedStream::makeGUID(const std::vector<std::string>& strs)
{
	constexpr static char characters[] = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	constexpr size_t length = sizeof(characters);

	size_t slots[12] = { 0 };
	size_t idx = 0;

	constexpr size_t upper_length = 'A' - 'Z' + 1;
	constexpr size_t lower_length = 'z' - 'a' + 1;

	for (auto& str : strs)
	{
		for (auto& ch : str)
		{
			auto& slot = slots[idx++];
			if ('a' <= ch && ch <= 'z')
			{
				slot += ch - 'a' + upper_length;
			}
			else if ('A' <= ch && ch <= 'Z')
			{
				slot += ch - 'A';
			}
			else if ('0' <= ch && ch <= '9')
			{
				slot += ch - '0' + lower_length + upper_length;
			}
		}
	}

	for (auto& slot : slots)
	{
		slot %= length;
	}

	std::string ret{ "" };
	ret += slots[0];
	ret += slots[1];
	ret += slots[2];
	ret += slots[3];
	ret += "_";
	ret += slots[4];
	ret += slots[5];
	ret += slots[6];
	ret += slots[7];
	ret += "_";
	ret += slots[8];
	ret += slots[9];
	ret += slots[10];
	ret += slots[11];
	return ret;
}


/*
#include "NativeSourceStream.hpp"

LibraryInterfaceGenerator::Implementation::DefineOnce::DefineOnce(std::stringstream& ss, const std::vector<std::string>& moduleNames, const std::string& name, std::string& indent)
	: _ss(ss), _indent(indent)
{
	macro = "__";
	for (auto& moduleName : moduleNames)
	{
		macro += moduleName;
		macro += "_";
	}
	macro += name;
	macro += "_";
	macro += makeGUID();
	macro += "__";

	_ss << "#if !defined(" << macro << ")\n";
	_ss << "#define " << macro << "\n";
	_ss << "\n";
}

LibraryInterfaceGenerator::Implementation::DefineOnce::~DefineOnce()
{
	_ss << "\n";
	_ss << "#endif // " << macro << "\n";
}

std::string LibraryInterfaceGenerator::Implementation::DefineOnce::makeGUID()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	const char* characters = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::uniform_int_distribution<int> dis(0, strlen(characters) - 1);

	std::string ret{ "" };

	ret += characters[dis(gen)];
	ret += characters[dis(gen)];
	ret += characters[dis(gen)];
	ret += characters[dis(gen)];
	ret += "_";
	ret += characters[dis(gen)];
	ret += characters[dis(gen)];
	ret += characters[dis(gen)];
	ret += characters[dis(gen)];
	ret += "_";
	ret += characters[dis(gen)];
	ret += characters[dis(gen)];
	ret += characters[dis(gen)];
	ret += characters[dis(gen)];
	return ret;
}

LibraryInterfaceGenerator::Implementation::DefineInclude::DefineInclude(std::stringstream& ss, std::string& indent)
	: _ss(ss), _indent(indent)
{

}

void LibraryInterfaceGenerator::Implementation::DefineInclude::addExternal(const std::string& header_path)
{
	std::string path{ header_path };
	std::replace(path.begin(), path.end(), '\\', '/');
	_ss << "#include <" << path << ">\n";
}

void LibraryInterfaceGenerator::Implementation::DefineInclude::addInternal(const std::string& header_path)
{
	std::string path{ header_path };
	std::replace(path.begin(), path.end(), '\\', '/');
	_ss << "#include \"" << path << "\"\n";
}


LibraryInterfaceGenerator::Implementation::DefineNamespace::DefineNamespace(std::stringstream& ss, const std::vector<std::string>& modules, std::string& indent)
:	_ss(ss), _indent(indent)
{
	for (_depth = 0; _depth < modules.size(); _depth++)
	{
		_ss << _indent << "namespace " << modules[_depth] << "\n";
		_ss << _indent << "{\n";
		_indent += new_indent;
	}
}

LibraryInterfaceGenerator::Implementation::DefineNamespace::DefineNamespace(std::stringstream&ss, const std::string& name, std::string& indent)
:	_ss(ss), _indent(indent)
{
	_depth += 1;
	_ss << _indent << "namespace " << name << "\n";
	_ss << _indent << "{\n";
	_indent += new_indent;
}

LibraryInterfaceGenerator::Implementation::DefineNamespace::~DefineNamespace()
{
	while(_depth--)
	{
		_indent.erase(_indent.end() - indent_size, _indent.end());
		_ss << _indent << "}\n";
	}
}

void LibraryInterfaceGenerator::Implementation::DefineNamespace::addLine(const std::string& line)
{
	_ss << _indent << line << "\n";
}


LibraryInterfaceGenerator::Implementation::DefineObject::DefineObject(std::stringstream& ss, const std::string& object, std::string& indent)
	: _ss(ss), _indent(indent)
{
	_ss << _indent << object << "\n";
	_ss << _indent << "{\n";
	_indent += new_indent;
}

LibraryInterfaceGenerator::Implementation::DefineObject::~DefineObject()
{
	_indent.erase(_indent.end() - indent_size, _indent.end());
	_ss << _indent << "};\n";
}

void LibraryInterfaceGenerator::Implementation::DefineObject::addLine(const std::string& line)
{
	_ss << _indent << line << "\n";
}

LibraryInterfaceGenerator::Implementation::Comment::Comment(std::stringstream& ss, std::string& indent)
	: _ss(ss), _indent(indent)
{
	_ss << _indent << "\/*" << "\n";
}

LibraryInterfaceGenerator::Implementation::Comment::~Comment()
{
	_ss << _indent << "*\/" << "\n";
}

void LibraryInterfaceGenerator::Implementation::Comment::add(const SymbolPackage& obj)
{
	_ss << _indent << "* Library Name : " << obj.name << "\n";
	_ss << _indent << "* Author : " << obj.author << "\n";

	auto descriptions = tokenize(obj.description);
	_ss << _indent << "* Description :\n";
	for (auto& line : descriptions)
	{
		_ss << _indent << "*     " << line << "\n";
	}
}

void LibraryInterfaceGenerator::Implementation::Comment::add(const SymbolModule& obj)
{
	_ss << _indent << "* Module Name : " << obj.name << "\n";

	auto descriptions = tokenize(obj.description);
	_ss << _indent << "* Description :\n";
	for (auto& line : descriptions)
	{
		_ss << _indent << "*     " << line << "\n";
	}
}

void LibraryInterfaceGenerator::Implementation::Comment::add(const SymbolClass& obj)
{
	auto descriptions = tokenize(obj.description);
	_ss << _indent << "* \\brief \n";
	for (auto& line : descriptions)
	{
		_ss << _indent << "*     " << line << "\n";
	}
}
void LibraryInterfaceGenerator::Implementation::Comment::add(const SymbolEnum& obj)
{
	auto descriptions = tokenize(obj.description);
	_ss << _indent << "* \\brief \n";
	for (auto& line : descriptions)
	{
		_ss << _indent << "*     " << line << "\n";
	}
}

void LibraryInterfaceGenerator::Implementation::Comment::add(const SymbolMethod& obj)
{
	auto descriptions = tokenize(obj.description);
	_ss << _indent << "* \\brief \n";
	for (auto& line : descriptions)
	{
		_ss << _indent << "*     " << line << "\n";
	}

	for (auto& parameter : obj.parameters)
	{
		add(*parameter);
	}

	_ss << _indent << "* @return " << obj.return_description << "\n";
}

void LibraryInterfaceGenerator::Implementation::Comment::add(const SymbolParameter& obj)
{
	_ss << _indent << "* @param[" << (obj.io == SymbolParameter::IO::IN ? "in" : "out") << "] " << obj.name << "  " << obj.description << "\n";
}

void LibraryInterfaceGenerator::Implementation::Comment::add(const SymbolProperty& obj)
{
	_ss << _indent << "* property name : " << obj.name << "\n";
	auto descriptions = tokenize(obj.description);
	_ss << _indent << "* \\brief \n";
	for (auto& line : descriptions)
	{
		_ss << _indent << "*     " << line << "\n";
	}
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::Comment::tokenize(const std::string& description)
{
	std::vector<std::string> ret;
	std::istringstream ss{ description };
	for (std::string line; std::getline(ss, line);)
	{
		ret.push_back(line);
	}
	return ret;
}


*/