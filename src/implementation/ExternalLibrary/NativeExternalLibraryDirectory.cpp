#include "NativeExternalLibraryDirectory.hpp"

#include "Log/Log_cpp.h"
#include "Log/Log_hpp.h"
#include "MemoryPool/MemoryPool_cpp.h"
#include "MemoryPool/MemoryPool_hpp.h"
#include "MemoryPool/MemoryPoolImpl_hpp.h"
#include "Tag/Tag_hpp.h"
#include "ManagedTypeConverter/ManagedTypeConverter_hpp.h"
#include "ManagedTypeConverter/ManagedTypeConverter_cpp.h"
#include "KotlinTypeConverter/KotlinTypeConverter_hpp.h"
#include "KotlinTypeConverter/KotlinTypeConverter_cpp.h"



#ifdef __linux__
static char* delimeter = "/";
#elif _WIN32
static char* delimeter = "\\";
#else
static char* delimeter = "/";
#endif

LibraryInterfaceGenerator::Implementation::NativeExternalLibraryDirectory::NativeExternalLibraryDirectory(
    std::string root_dir_path,
	const char* external_library_dir_name) : _lib_dir_name(external_library_dir_name)
{
	for (auto& check : is_created)
		check = false;
	_lib_dir_path = root_dir_path;
	_lib_dir_path += delimeter;
	_lib_dir_path += _lib_dir_name;
}

static LibraryInterfaceGenerator::Implementation::Result createExternalToolDirectory(const std::string& lib_dir_path, const std::string& tool_name)
{
	std::string dir_path = lib_dir_path;
	dir_path += delimeter;
	dir_path += tool_name;
	auto ret = LibraryInterfaceGenerator::Implementation::FileSystem::createDirectory(dir_path);
	return ret;
}

static LibraryInterfaceGenerator::Implementation::Result createExternalToolInternal
(const std::string& lib_dir_path, const std::string& tool_name, const char* file_name, const char* content)
{
	std::string path = lib_dir_path;
	path += delimeter;
	path += tool_name;
	path += delimeter;
	path += file_name;

	auto ret = LibraryInterfaceGenerator::Implementation::FileSystem::createFile(path, content);
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
			auto ret = createExternalToolDirectory(_lib_dir_path, "Log");
			if (!ret)
				return ret;
		}
		{
			auto ret = createExternalToolInternal(_lib_dir_path, "Log", "Log.hpp", LOG_HPP);
			if (!ret)
				return ret;
		}
		{
			auto ret = createExternalToolInternal(_lib_dir_path, "Log", "Log.cpp", LOG_CPP);
			if (!ret)
				return ret;
		}
		break;
	case ExternalTool::MemoryPool:
		{
			auto ret = createExternalToolDirectory(_lib_dir_path, "MemoryPool");
			if (!ret)
				return ret;
		}
		{
			auto ret = createExternalToolInternal(_lib_dir_path, "MemoryPool", "MemoryPool.hpp", MEMORYPOOL_HPP);
			if (!ret)
				return ret;
		}
		{
			auto ret = createExternalToolInternal(_lib_dir_path, "MemoryPool", "MemoryPool.cpp", MEMORYPOOL_CPP);
			if (!ret)
				return ret;
		}
		{
			auto ret = createExternalToolInternal(_lib_dir_path, "MemoryPool", "MemoryPoolImpl.hpp", MEMORYPOOLIMPL_HPP);
			if (!ret)
				return ret;
		}
		{
			auto ret = createExternalToolDirectory(_lib_dir_path, "Tag");
			if (!ret)
				return ret;
		}
		{
			auto ret = createExternalToolInternal(_lib_dir_path, "Tag", "Tag.hpp", TAG_HPP);
			if (!ret)
				return ret;
		}
		break;
	case ExternalTool::ManagedTypeConverter:
		{
			auto ret = createExternalToolDirectory(_lib_dir_path, "ManagedTypeConverter");
			if (!ret)
				return ret;
		}
		{
			auto ret = createExternalToolInternal(_lib_dir_path, "ManagedTypeConverter", "ManagedTypeConverter.hpp", MANAGEDTYPECONVERTER_HPP);
			if (!ret)
				return ret;
		}
		{
			auto ret = createExternalToolInternal(_lib_dir_path, "ManagedTypeConverter", "ManagedTypeConverter.cpp", MANAGEDTYPECONVERTER_CPP);
			if (!ret)
				return ret;
		}
		break;
	case ExternalTool::KotlinTypeConverter:
		{
			auto ret = createExternalToolDirectory(_lib_dir_path, "KotlinTypeConverter");
			if (!ret)
				return ret;
		}
		{
			auto ret = createExternalToolInternal(_lib_dir_path, "KotlinTypeConverter", "KotlinTypeConverter.hpp", KOTLINTYPECONVERTER_HPP);
			if (!ret)
				return ret;
		}
		{
			auto ret = createExternalToolInternal(_lib_dir_path, "KotlinTypeConverter", "KotlinTypeConverter.cpp", KOTLINTYPECONVERTER_CPP);
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
	path += _lib_dir_name;
	path += "/";
	switch (tool)
	{
	case ExternalTool::Log:
		path += "Log/Log.hpp";
		break;
	case ExternalTool::MemoryPool:
		path += "MemoryPool/MemoryPool.hpp";
		break;
	case ExternalTool::ManagedTypeConverter:
		path += "ManagedTypeConverter/ManagedTypeConverter.hpp";
		break;
	case ExternalTool::KotlinTypeConverter:
		path += "KotlinTypeConverter/KotlinTypeConverter.hpp";
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
	path += _lib_dir_name;
	path += "/";
	switch (tool)
	{
	case ExternalTool::Log:
		path += "Log/Log.hpp";
		break;
	case ExternalTool::MemoryPool:
		path += "MemoryPool/MemoryPool.hpp";
		break;
	case ExternalTool::ManagedTypeConverter:
		path += "ManagedTypeConverter/ManagedTypeConverter.hpp";
		break;
	case ExternalTool::KotlinTypeConverter:
		path += "KotlinTypeConverter/KotlinTypeConverter.hpp";
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
	path += _lib_dir_name;
	path += "/";
	switch (tool)
	{
	case ExternalTool::Log:
		path += "Log/Log.hpp";
		break;
	case ExternalTool::MemoryPool:
		path += "MemoryPool/MemoryPool.hpp";
		break;
	case ExternalTool::ManagedTypeConverter:
		path += "ManagedTypeConverter/ManagedTypeConverter.hpp";
		break;
	case ExternalTool::KotlinTypeConverter:
		path += "KotlinTypeConverter/KotlinTypeConverter.hpp";
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
	path += _lib_dir_name;
	path += "/";
	switch (tool)
	{
	case ExternalTool::Log:
		path += "Log/Log.hpp";
		break;
	case ExternalTool::MemoryPool:
		path += "MemoryPool/MemoryPool.hpp";
		break;
	case ExternalTool::ManagedTypeConverter:
		path += "ManagedTypeConverter/ManagedTypeConverter.hpp";
		break;
	case ExternalTool::KotlinTypeConverter:
		path += "KotlinTypeConverter/KotlinTypeConverter.hpp";
		break;
	}
	return path;
}

