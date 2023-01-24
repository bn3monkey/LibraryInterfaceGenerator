#include "KotlinSourceStream.hpp"

LibraryInterfaceGenerator::Implementation::PackageKotlinSourceStream::PackageKotlinSourceStream(SourceStream& sourceStream, const std::string& root_package_name, const std::vector<std::string>& modules)
	: _stream(sourceStream)
{
	_stream << "package " << root_package_name;
	for (auto& mod : modules)
	{
		_stream << "." << mod;
	}
}

LibraryInterfaceGenerator::Implementation::PackageKotlinSourceStream::~PackageKotlinSourceStream()
{
	_stream << "\n";
}

LibraryInterfaceGenerator::Implementation::ImportKotlinSourceStream::ImportKotlinSourceStream(SourceStream& sourceStream, const std::string& root_package_name, const std::vector<std::string>& scopes)
	: _stream(sourceStream)
{
	_stream << "import " << root_package_name;
	for (auto& scope : scopes)
	{
		_stream << "." << scope;
	}
}

LibraryInterfaceGenerator::Implementation::ImportKotlinSourceStream::ImportKotlinSourceStream(SourceStream& sourceStream, const std::string& external_class_name)
	: _stream(sourceStream)
{
	_stream << "import " << external_class_name;
}

LibraryInterfaceGenerator::Implementation::ImportKotlinSourceStream::~ImportKotlinSourceStream()
{
	_stream << "\n";
}

LibraryInterfaceGenerator::Implementation::ClassKotlinSourceScopedStream::ClassKotlinSourceScopedStream(SourceStream& sourceStream, const std::string& name, const std::vector<std::string>& base_names)
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

	sourceStream << "\n";
	_stream = new SourceScopedStream(sourceStream, CodeStyle::Kotlin);
	
}

LibraryInterfaceGenerator::Implementation::ClassKotlinSourceScopedStream::~ClassKotlinSourceScopedStream()
{
	if (_stream)
		delete _stream;
}

LibraryInterfaceGenerator::Implementation::CompanionObjectKotlinSourceScopedStream::CompanionObjectKotlinSourceScopedStream(SourceStream& sourceStream)
{
	sourceStream << "companion object\n";
	_stream = new SourceScopedStream(sourceStream, CodeStyle::Kotlin);
}

LibraryInterfaceGenerator::Implementation::CompanionObjectKotlinSourceScopedStream::~CompanionObjectKotlinSourceScopedStream()
{
	if (_stream)
		delete _stream;
}

void LibraryInterfaceGenerator::Implementation::CompanionObjectKotlinSourceScopedStream::addSingleTon(const std::string& class_name)
{
	*_stream << "@Volatile private var instance : " << class_name << " ? = null\n";
	*_stream << "@JvmStatic fun getInstance() : " << class_name << " =\n";
	*_stream << "	instance ? : synchronized(this) { instance ? : " << class_name << "().also{instance = it} }\n";
}

LibraryInterfaceGenerator::Implementation::CompanionObjectKotlinSourceScopedStream::Init::Init(SourceStream& sourceStream)
{
	sourceStream << "init\n";
	_stream = new SourceScopedStream(sourceStream, CodeStyle::Kotlin);
}

LibraryInterfaceGenerator::Implementation::CompanionObjectKotlinSourceScopedStream::Init::~Init()
{
	if (_stream)
		delete _stream;
}

void LibraryInterfaceGenerator::Implementation::CompanionObjectKotlinSourceScopedStream::Init::loadLibrary(const std::string& library_name)
{
	*_stream << "System.loadLibrary(\"" << library_name << "\")\n";
}

LibraryInterfaceGenerator::Implementation::EnumKotlinSourceScopedStream::EnumKotlinSourceScopedStream(SourceStream& sourceStream, const std::string& name)
{
	sourceStream << "enum class " << name << "(val value : Int)\n";
	_stream = new SourceScopedStream(sourceStream, CodeStyle::Kotlin);
}

LibraryInterfaceGenerator::Implementation::EnumKotlinSourceScopedStream::~EnumKotlinSourceScopedStream()
{
	if (_stream)
		delete _stream;
}

void LibraryInterfaceGenerator::Implementation::EnumKotlinSourceScopedStream::addElement(const std::string& key, const std::string& value)
{
	*_stream << key << "(" << value << ".toInt()),\n";
}

LibraryInterfaceGenerator::Implementation::AdvancedEnumKotlinSourceScopedStream::AdvancedEnumKotlinSourceScopedStream(SourceStream& sourceStream, const std::string& name)
{
	sourceStream << "enum class " << name << "(val value : Int, val fullname : String)\n";
	_stream = new SourceScopedStream(sourceStream, CodeStyle::Kotlin);
}

LibraryInterfaceGenerator::Implementation::AdvancedEnumKotlinSourceScopedStream::~AdvancedEnumKotlinSourceScopedStream()
{
	if (_stream)
		delete _stream;
}

void LibraryInterfaceGenerator::Implementation::AdvancedEnumKotlinSourceScopedStream::addElement(const std::string& key, const std::string& value, const std::string& fullname)
{
	*_stream << key << "(" << value << ".toInt(), \"" << fullname << "\"),\n";
}

LibraryInterfaceGenerator::Implementation::MethodKotlinSourceScopedStream::MethodKotlinSourceScopedStream(
	SourceStream& sourceStream,
	Access access,
	const std::string& prefix,
	const std::string& postfix,
	const std::string& type,
	const std::string& name,
	const std::vector<Parameter>& parameters) : _stream(nullptr)
{
	std::string accessor{ "" };
	switch (access)
	{
	case Access::PUBLIC: accessor = "";
		break;
	case Access::PROTECTED:
		accessor = "public";
		break;
	case Access::PRIVATE:
		accessor = "private";
		break;
	case Access::INTERNAL:
		accessor = "internal";
		break;
	case Access::EXTERNAL:
		accessor = "external";
		break;
	}
	sourceStream << accessor << " " << prefix << " fun " << name << "(";

	if (!parameters.empty())
	{
		for (auto& parameter : parameters)
		{
			sourceStream << parameter.name << " : " << parameter.type << ", ";
		}
		sourceStream.pop(2);
	}

	sourceStream << ")";

	if (access != Access::EXTERNAL)
		_stream = new SourceScopedStream(sourceStream, CodeStyle::Kotlin);
}

LibraryInterfaceGenerator::Implementation::MethodKotlinSourceScopedStream::~MethodKotlinSourceScopedStream()
{
	if (_stream)
		delete _stream;
}

LibraryInterfaceGenerator::Implementation::CommentKotlinSourceStream::CommentKotlinSourceStream(SourceStream& sourceStream) : _stream(sourceStream)
{
	_stream << "/*\n";
}

LibraryInterfaceGenerator::Implementation::CommentKotlinSourceStream::~CommentKotlinSourceStream()
{
	_stream << "*/\n";
}

void LibraryInterfaceGenerator::Implementation::CommentKotlinSourceStream::addName(const std::string& name)
{
	_stream << " * Name : " << name << "\n";
}
void LibraryInterfaceGenerator::Implementation::CommentKotlinSourceStream::addDescription(const std::string& description)
{
	_stream << " * Description :\n";
	auto descriptions = tokenize(description);
	for (auto& line : descriptions)
	{
		_stream << " * " << line << "\n";
	}
}
void LibraryInterfaceGenerator::Implementation::CommentKotlinSourceStream::addBrief(const std::string& brief)
{
	_stream << " * \\brief : " << brief << "\n";
	auto descriptions = tokenize(brief);
	for (auto& line : descriptions)
	{
		_stream << " * " << line << "\n";
	}
}
void LibraryInterfaceGenerator::Implementation::CommentKotlinSourceStream::addReturn(const std::string& return_value)
{
	_stream << " * @return : " << return_value << "\n";
}

void LibraryInterfaceGenerator::Implementation::CommentKotlinSourceStream::addParameter(bool isInputParameter, const std::string& name, const std::string& description)
{
	_stream << " * @param[" << (isInputParameter ? "in" : "out") << "]  " << name << "  " << description << "\n";
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::CommentKotlinSourceStream::tokenize(const std::string& description)
{
	std::vector<std::string> ret;
	std::istringstream ss{ description };
	for (std::string line; std::getline(ss, line);)
	{
		ret.push_back(line);
	}
	return ret;
}