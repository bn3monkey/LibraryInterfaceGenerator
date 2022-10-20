#include "CXXSourceStream.hpp"
#include <algorithm>

LibraryInterfaceGenerator::Implementation::HeaderGuardCXXSourceScopedStream::HeaderGuardCXXSourceScopedStream(SourceStream& sourceStream, const std::vector<std::string>& module_names, const std::string& name)
	: _stream(sourceStream)
{
	_macro += "__";
	for (auto& module_name : module_names)
	{
		std::string converted_module_name { module_name };
		std::transform(converted_module_name.begin(), converted_module_name.end(), converted_module_name.begin(), toupper);
		_macro += converted_module_name;
		_macro += "_";
	}
	auto guid = makeGUID(module_names);
	std::string converted_name {name};
	std::transform(converted_name.begin(), converted_name.end(), converted_name.begin(), toupper);
	_macro += converted_name;
	_macro += "_";
	_macro += guid;
	_macro += "__";

	_stream << "#if !defined(" << _macro << ")\n";
	_stream << "#define " << _macro << "\n";
	_stream << "\n";
}

LibraryInterfaceGenerator::Implementation::HeaderGuardCXXSourceScopedStream::~HeaderGuardCXXSourceScopedStream()
{
	_stream << "\n";
	_stream << "#endif // " << _macro << "\n";
}

std::string LibraryInterfaceGenerator::Implementation::HeaderGuardCXXSourceScopedStream::makeGUID(const std::vector<std::string>& strs)
{
	constexpr static char characters[] = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	constexpr size_t length = sizeof(characters) - 1;

	size_t slots[12] = { 0 };
	size_t idx = 0;

	constexpr size_t upper_length = 'Z' - 'A' + 1;
	constexpr size_t lower_length = 'z' - 'a' + 1;

	for (auto& str : strs)
	{
		for (auto& ch : str)
		{
			auto& slot = slots[idx];
			idx = (idx + 1) % 12;
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
	ret += characters[slots[0]];
	ret += characters[slots[1]];
	ret += characters[slots[2]];
	ret += characters[slots[3]];
	ret += "_";
	ret += characters[slots[4]];
	ret += characters[slots[5]];
	ret += characters[slots[6]];
	ret += characters[slots[7]];
	ret += "_";
	ret += characters[slots[8]];
	ret += characters[slots[9]];
	ret += characters[slots[10]];
	ret += characters[slots[11]];
	return ret;
}

LibraryInterfaceGenerator::Implementation::InternalIncludeCXXSourceStream::InternalIncludeCXXSourceStream(SourceStream& sourceStream, const std::string& header_path)
	: _stream(sourceStream)
{
	std::string path{ header_path };
	std::replace(path.begin(), path.end(), '\\', '/');
	_stream << "#include \"" << path << "\"\n";
}

LibraryInterfaceGenerator::Implementation::InternalIncludeCXXSourceStream::~InternalIncludeCXXSourceStream()
{
	// None
}

LibraryInterfaceGenerator::Implementation::ExternalIncludeCXXSourceStream::ExternalIncludeCXXSourceStream(SourceStream& sourceStream, const std::string& header_path)
	: _stream(sourceStream)
{
	std::string path{ header_path };
	std::replace(path.begin(), path.end(), '\\', '/');
	_stream << "#include <" << path << ">\n";
}

LibraryInterfaceGenerator::Implementation::ExternalIncludeCXXSourceStream::~ExternalIncludeCXXSourceStream()
{
	// None
}

LibraryInterfaceGenerator::Implementation::NamespaceCXXSourceScopedStream::NamespaceCXXSourceScopedStream(SourceStream& sourceStream, const std::string& namespace_name)
{
	sourceStream << "namespace " << namespace_name << "\n";
	_stream = new SourceScopedStream(sourceStream, CodeStyle::Cpp);

#if _DEBUG
	_namespace_name = namespace_name;
	printf("Consturctor : %s, Address : %p\n", _namespace_name.c_str(), this);
#endif
}

LibraryInterfaceGenerator::Implementation::NamespaceCXXSourceScopedStream::NamespaceCXXSourceScopedStream(SourceStream& sourceStream, const std::vector<std::string>& namespace_names)
	: _stream(nullptr)
{
#if _DEBUG
	for (auto& namespace_name : namespace_names)
	{
		_namespace_name += namespace_name;
		_namespace_name += "/";
	}
	printf("Consturctor : %s, Address : %p\n", _namespace_name.c_str(), this);
#endif
	_stack.reserve(namespace_names.size());
	for (auto& namespace_name : namespace_names)
	{
		_stack.emplace_back(sourceStream, namespace_name);
	}
}

LibraryInterfaceGenerator::Implementation::NamespaceCXXSourceScopedStream::~NamespaceCXXSourceScopedStream()
{
#if _DEBUG
	printf("Destructor : %s, Address : %p\n", _namespace_name.c_str(), this);
#endif

	while (!_stack.empty())
		_stack.pop_back();
	
	if (_stream)
	{
		delete _stream;
		_stream = nullptr;
	}
}

LibraryInterfaceGenerator::Implementation::NamespaceCXXSourceScopedStream::NamespaceCXXSourceScopedStream(NamespaceCXXSourceScopedStream&& other)
{
	this->_stack = std::move(other._stack);
	this->_stream = other._stream;
	this->_namespace_name = other._namespace_name;
	other._stream = nullptr;

#if _DEBUG
	printf("Consturctor : %s, Address : %p Other Address : %p\n", _namespace_name.c_str(), this, &other);
#endif
}

LibraryInterfaceGenerator::Implementation::ClassCXXSourceScopedStream::ClassCXXSourceScopedStream(SourceStream& sourceStream, bool isDeclaration, const std::string& name, const std::vector<std::string>& base_names)
{
	sourceStream << "class " << name;
	if (!base_names.empty())
	{
		sourceStream << " : ";
		for (auto& base_name : base_names)
		{
			sourceStream << base_name << ", ";
		}
		sourceStream.pop(sizeof(", ") - 1);
	}

	if (isDeclaration)
	{
		sourceStream << ";";
	}

	sourceStream << "\n";

	if (!isDeclaration)
	{
		_stream = new SourceScopedStream(sourceStream, CodeStyle::CppClass);
	}
}

LibraryInterfaceGenerator::Implementation::ClassCXXSourceScopedStream::~ClassCXXSourceScopedStream()
{
	if (_stream)
		delete _stream;
}

LibraryInterfaceGenerator::Implementation::AccessCXXSourceScopedStream::AccessCXXSourceScopedStream(SourceStream& sourceStream, const Specifier& specifier)
{
	_outer_stream = new UnindentedSourceScopedStream(sourceStream, CodeStyle::None);
	switch (specifier)
	{
	case Specifier::Public :
		sourceStream << "public : \n";
		break;
	case Specifier::Private:
		sourceStream << "private : \n";
		break;
	case Specifier::Protected:
		sourceStream << "protected : \n";
		break;
	}
	_inner_stream = new SourceScopedStream(sourceStream, CodeStyle::None);
}

LibraryInterfaceGenerator::Implementation::AccessCXXSourceScopedStream::~AccessCXXSourceScopedStream()
{
	if (_inner_stream)
		delete _inner_stream;
	if (_outer_stream)
		delete _outer_stream;
}

LibraryInterfaceGenerator::Implementation::EnumCXXSourceScopedStream::EnumCXXSourceScopedStream(SourceStream& sourceStream, bool isDeclaration, const std::string& name)
{
	sourceStream << "enum class " << name;
	if (isDeclaration)
	{
		sourceStream << ";";
	}

	sourceStream << "\n";

	if (!isDeclaration)
	{
		_stream = new SourceScopedStream(sourceStream, CodeStyle::CppClass);
	}
}

LibraryInterfaceGenerator::Implementation::EnumCXXSourceScopedStream::~EnumCXXSourceScopedStream()
{
	if (_stream)
		delete _stream;
}

void LibraryInterfaceGenerator::Implementation::EnumCXXSourceScopedStream::addElement(const std::string& key, const std::string& value)
{
	*_stream << key << " = " << value << ", \n";
}

LibraryInterfaceGenerator::Implementation::MethodCXXSourceScopedStream::MethodCXXSourceScopedStream(
	SourceStream& sourceStream,
	bool isDeclaration,
	const std::string& prefix,
	const std::string& postfix,
	const std::string& type,
	const std::vector<std::string>& scopes,
	const std::string& name,
	const std::vector<Parameter>& parameters)
{
	if (prefix != "")
	{
		sourceStream << prefix << " ";
	}
	sourceStream << type << " ";
	if (!scopes.empty())
	{
		for (auto& scope : scopes)
		{
			sourceStream << scope;
			sourceStream << "::";
		}
	}

	sourceStream << name;
	sourceStream << "(";

	if (!parameters.empty())
	{
		for (auto& parameter : parameters)
		{
			if (parameter.io == Parameter::REFERENCE_IN)
			{
				sourceStream << "const ";
			}
			sourceStream << parameter.type;
			if (parameter.io == Parameter::REFERENCE_IN || parameter.io == Parameter::REFERENCE_OUT)
			{
				sourceStream << "&";
			}
			sourceStream << " " << parameter.name << ", ";
		}
		sourceStream.pop(sizeof(", ") - 1);
	}
	
	sourceStream << ")";

	if (postfix != "")
	{
		sourceStream << " " << postfix;
	}

	if (isDeclaration)
	{
		sourceStream << ";";
	}
	sourceStream << "\n";
	if (!isDeclaration)
	{
		_stream = new SourceScopedStream(sourceStream, CodeStyle::Cpp);
	}
}

LibraryInterfaceGenerator::Implementation::MethodCXXSourceScopedStream::~MethodCXXSourceScopedStream()
{
	if (_stream)
		delete _stream;
}


LibraryInterfaceGenerator::Implementation::CommentCXXSourceStream::CommentCXXSourceStream(SourceStream& sourceStream)
	: _stream(sourceStream)
{
	_stream << "/*\n";
}

LibraryInterfaceGenerator::Implementation::CommentCXXSourceStream::~CommentCXXSourceStream()
{
	_stream << "*/\n";
}

void LibraryInterfaceGenerator::Implementation::CommentCXXSourceStream::addName(const std::string& name)
{
	_stream << " * Name : " << name << "\n";
}

void LibraryInterfaceGenerator::Implementation::CommentCXXSourceStream::addDescription(const std::string& description)
{
	_stream << " * Description :\n";
	auto descriptions = tokenize(description);
	for (auto& line : descriptions)
	{
		_stream << " * " << line << "\n";
	}
}

void LibraryInterfaceGenerator::Implementation::CommentCXXSourceStream::addBrief(const std::string& brief)
{
	_stream << " * \\brief : " << brief << "\n";
	auto descriptions = tokenize(brief);
	for (auto& line : descriptions)
	{
		_stream << " * " << line << "\n";
	}
}

void LibraryInterfaceGenerator::Implementation::CommentCXXSourceStream::addReturn(const std::string& return_value)
{
	_stream << " * @return : " << return_value << "\n";
}

void LibraryInterfaceGenerator::Implementation::CommentCXXSourceStream::addParameter(bool isInputParameter, const std::string& name, const std::string& description)
{
	_stream << " * @param[" << (isInputParameter ? "in" : "out") << "]  " << name << "  " << description << "\n";
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::CommentCXXSourceStream::tokenize(const std::string& description)
{
	std::vector<std::string> ret;
	std::istringstream ss{ description };
	for (std::string line; std::getline(ss, line);)
	{
		ret.push_back(line);
	}
	return ret;
}
