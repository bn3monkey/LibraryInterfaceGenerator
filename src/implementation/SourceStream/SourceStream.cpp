#include "SourceStream.hpp"

LibraryInterfaceGenerator::Implementation::SourceStream::SourceStream()
{
	_content = new char[MAX_STREAM_SIZE];
	memset(_content, 0, MAX_STREAM_SIZE);
}

LibraryInterfaceGenerator::Implementation::SourceStream::~SourceStream()
{
	if (_content != nullptr)
		delete[] _content;
}

void LibraryInterfaceGenerator::Implementation::SourceStream::pop(size_t end_string_size)
{
	if (_content[_size - 1] == '\n')
	{
		_is_new_line_started = false;
	}
	_size -= end_string_size;
	_content[_size] = 0;
}

void LibraryInterfaceGenerator::Implementation::SourceStream::addIndent()
{
	if (MAX_INDENT > _indent_size)
	{
		_current_indent[_indent_size++] = indent;
	}
}

void LibraryInterfaceGenerator::Implementation::SourceStream::removeIndent()
{
	if (_indent_size > 0)
	{
		_current_indent[--_indent_size] = 0;
	}
}

char*& LibraryInterfaceGenerator::Implementation::SourceStream::str()
{
	return reinterpret_cast<char*&>(_content);
}

void LibraryInterfaceGenerator::Implementation::SourceStream::clear()
{
	memset(_current_indent, 0, _indent_size);
	_indent_size = 0;

	_is_new_line_started = false;

	_size = 0;
	memset(_content, 0, MAX_STREAM_SIZE);
}

LibraryInterfaceGenerator::Implementation::SourceStream& LibraryInterfaceGenerator::Implementation::operator<<(LibraryInterfaceGenerator::Implementation::SourceStream& stream, const char* str)
{
	auto* ptr = str;
	while (*ptr != '\0')
	{
		if (stream._is_new_line_started)
		{
			stream._is_new_line_started = false;
			std::copy(stream._current_indent, stream._current_indent + stream._indent_size, stream._content + stream._size);
			stream._size += stream._indent_size;
		}
		stream._content[stream._size++] = *ptr;
		if (*ptr == '\n')
		{
			stream._is_new_line_started = true;
		}
		ptr++;
	}
	stream._content[stream._size] = 0;
	
	return stream;
}

LibraryInterfaceGenerator::Implementation::SourceStream& LibraryInterfaceGenerator::Implementation::operator<<(LibraryInterfaceGenerator::Implementation::SourceStream& stream, const std::string& str)
{
	stream << str.c_str();
	return stream;
}

LibraryInterfaceGenerator::Implementation::SourceScopedStream::SourceScopedStream(SourceStream& ss, CodeStyle style) : _stream(ss)
{
	using namespace LibraryInterfaceGenerator::Implementation;

	_scopeStart = nullptr;
	_scopeEnd = nullptr;
	switch (style)
	{
	case CodeStyle::Cpp:
	case CodeStyle::Kotlin:
		_scopeStart = "{";
		_scopeEnd = "}";
		break;
	case CodeStyle::CppClass:
		_scopeStart = "{";
		_scopeEnd = "};";
		break;
	case CodeStyle::Python:
	case CodeStyle::None:
		break;
	}

	if (_scopeStart != nullptr)
		ss << _scopeStart << "\n";

	ss.addIndent();
}

LibraryInterfaceGenerator::Implementation::SourceScopedStream::SourceScopedStream(SourceScopedStream& ss, CodeStyle style) :
	SourceScopedStream(ss._stream,  style)
{
}

LibraryInterfaceGenerator::Implementation::SourceScopedStream::SourceScopedStream(UnindentedSourceScopedStream& ss, CodeStyle style) :
	SourceScopedStream(ss.stream(), style)
{
}

LibraryInterfaceGenerator::Implementation::SourceScopedStream::~SourceScopedStream()
{
	_stream.removeIndent();

	if (_scopeEnd != nullptr)
	{
		_stream << _scopeEnd << "\n";
	}
}

void LibraryInterfaceGenerator::Implementation::SourceScopedStream::pop(size_t end_string_size)
{
	_stream.pop(end_string_size);
}



LibraryInterfaceGenerator::Implementation::SourceScopedStream& LibraryInterfaceGenerator::Implementation::operator<<(LibraryInterfaceGenerator::Implementation::SourceScopedStream& stream, const char* str)
{
	stream._stream << str;
	return stream;
}

LibraryInterfaceGenerator::Implementation::SourceScopedStream& LibraryInterfaceGenerator::Implementation::operator<<(LibraryInterfaceGenerator::Implementation::SourceScopedStream& stream, const std::string& str)
{
	stream._stream << str;
	return stream;
}






LibraryInterfaceGenerator::Implementation::UnindentedSourceScopedStream::UnindentedSourceScopedStream(SourceStream& ss, CodeStyle style) : _stream(ss)
{
	_stream.removeIndent();
}

LibraryInterfaceGenerator::Implementation::UnindentedSourceScopedStream::UnindentedSourceScopedStream(SourceScopedStream& ss, CodeStyle style) : UnindentedSourceScopedStream(ss.stream(), style)
{
}

LibraryInterfaceGenerator::Implementation::UnindentedSourceScopedStream::UnindentedSourceScopedStream(UnindentedSourceScopedStream& ss, CodeStyle style) : UnindentedSourceScopedStream(ss.stream(), style)
{
}


LibraryInterfaceGenerator::Implementation::UnindentedSourceScopedStream::~UnindentedSourceScopedStream()
{
	_stream.addIndent();
}

LibraryInterfaceGenerator::Implementation::UnindentedSourceScopedStream& LibraryInterfaceGenerator::Implementation::operator<<(LibraryInterfaceGenerator::Implementation::UnindentedSourceScopedStream& stream, const char* str)
{
	stream._stream << str;
	return stream;
}

LibraryInterfaceGenerator::Implementation::UnindentedSourceScopedStream& LibraryInterfaceGenerator::Implementation::operator<<(LibraryInterfaceGenerator::Implementation::UnindentedSourceScopedStream& stream, const std::string& str)
{
	stream._stream << str;
	return stream;
}
void LibraryInterfaceGenerator::Implementation::UnindentedSourceScopedStream::pop(size_t end_string_size)
{
	_stream.pop(end_string_size);
}
