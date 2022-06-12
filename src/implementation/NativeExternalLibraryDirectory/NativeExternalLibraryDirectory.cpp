#include "NativeExternalLibraryDirectory.hpp"

#include "Log/Log.h"
#include "MemoryPool/MemoryPool.h"

#ifdef __linux__
static char* delimeter = "/";
#elif _WIN32
static char* delimeter = "\\";
#else
static char* delimeter = "/";
#endif

LibraryInterfaceGenerator::Implementation::NativeExternalLibraryDirectory::NativeExternalLibraryDirectory(std::string root_dir_path)
{
	for (auto& check : is_created)
		check = false;
	_lib_dir_path = root_dir_path;
	_lib_dir_path += delimeter;
	_lib_dir_path += DirectoryName;
}

static LibraryInterfaceGenerator::Implementation::Result createExternalToolInternal
(const std::string& lib_dir_path, std::string toolName, const char* header_content, const char* cpp_content)
{
	std::string dir_path = lib_dir_path;
	dir_path += delimeter;
	dir_path += toolName;
	{
		auto ret = LibraryInterfaceGenerator::Implementation::FileSystem::createDirectory(dir_path);
		if (!ret)
			return ret;
	}
	std::string header_path = dir_path;
	header_path += delimeter;
	header_path += toolName;
	header_path += ".hpp";

	{
		auto ret = LibraryInterfaceGenerator::Implementation::FileSystem::createFile(header_path, header_content);
		if (!ret)
			return ret;
	}
	std::string cpp_path = dir_path;
	cpp_path += delimeter;
	cpp_path += toolName;
	cpp_path += ".cpp";
	{
		auto ret = LibraryInterfaceGenerator::Implementation::FileSystem::createFile(cpp_path, cpp_content);
		if (!ret)
			return ret;
	}
	return LibraryInterfaceGenerator::Implementation::Result();
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::NativeExternalLibraryDirectory::createLibraryDirectory()
{
	return LibraryInterfaceGenerator::Implementation::FileSystem::createDirectory(_lib_dir_path);
}

LibraryInterfaceGenerator::Implementation::Result LibraryInterfaceGenerator::Implementation::NativeExternalLibraryDirectory::createExternalTool(ExternalTool tool)
{
	
	switch (tool)
	{
	case ExternalTool::Log :
		{
			auto ret = createExternalToolInternal(_lib_dir_path, "Log", log_header, log_source);
			if (!ret)
				return ret;
			}
		break;
	case ExternalTool::MemoryPool:
		{
			auto ret = createExternalToolInternal(_lib_dir_path, "MemoryPool", memorypool_header, memorypool_src);
			if (!ret)
				return ret;
		}
		break;
	}
	is_created[static_cast<size_t>(tool)] = true;
	return Result();
}

std::string LibraryInterfaceGenerator::Implementation::NativeExternalLibraryDirectory::getRelativeHeaderPath(ExternalTool tool) const
{
	std::string path;
	path += "../";
	path += DirectoryName;
	path += "/";
	switch (tool)
	{
	case ExternalTool::Log:
		path += "Log/Log.hpp";
		break;
	case ExternalTool::MemoryPool:
		path += "MemoryPool/MemoryPool.hpp";
		break;
	}
	return path;
}

std::string LibraryInterfaceGenerator::Implementation::NativeExternalLibraryDirectory::getRelativeHeaderPath(ExternalTool tool, const SymbolModule& modull) const
{
	std::string path;
	for (auto& name : modull.moduleNames)
	{
		path += "../";
	}
	path += DirectoryName;
	path += "/";
	switch (tool)
	{
	case ExternalTool::Log:
		path += "Log/Log.hpp";
		break;
	case ExternalTool::MemoryPool:
		path += "MemoryPool/MemoryPool.hpp";
		break;
	}
	return path;
}

std::string LibraryInterfaceGenerator::Implementation::NativeExternalLibraryDirectory::getRelativeHeaderPath(ExternalTool tool, const SymbolEnum& enumm) const
{
	std::string path;
	for (auto& name : enumm.parentModules)
	{
		path += "../";
	}
	path += DirectoryName;
	path += "/";
	switch (tool)
	{
	case ExternalTool::Log:
		path += "Log/Log.hpp";
		break;
	case ExternalTool::MemoryPool:
		path += "MemoryPool/MemoryPool.hpp";
		break;
	}
	return path;
}

std::string LibraryInterfaceGenerator::Implementation::NativeExternalLibraryDirectory::getRelativeHeaderPath(ExternalTool tool, const SymbolClass& clazz) const
{
	std::string path;
	for (auto& name : clazz.parentModules)
	{
		path += "../";
	}
	path += DirectoryName;
	path += "/";
	switch (tool)
	{
	case ExternalTool::Log:
		path += "Log/Log.hpp";
		break;
	case ExternalTool::MemoryPool:
		path += "MemoryPool/MemoryPool.hpp";
		break;
	}
	return path;
}
