#include "FileSystem.hpp"

using namespace LibraryInterfaceGenerator::Implementation;

Result FileSystem::createDirectory(std::string& path)
{
    auto path_obj = std::filesystem::path{path};
    bool ret = std::filesystem::create_directory(path_obj);
    if (!ret)
    {
        if (std::filesystem::exists(path_obj))
        {
            std::filesystem::remove_all(path_obj);
            ret = std::filesystem::create_directory(path_obj);
            if(!ret)
            {
                return Result(Result::Code::DIRECTORY_CREATION_FAIL, "Directory cannot be created (%s)", path.c_str());
            }
        }
        else
        {
            return Result(Result::Code::DIRECTORY_CREATION_FAIL, "Directory cannot be created (%s)", path.c_str());
        }
    }
    return Result(Result::Code::SUCCESS);
}
Result FileSystem::createDirectories(std::string& path)
{
    auto path_obj = std::filesystem::path{ path };
    bool ret = std::filesystem::create_directories(path_obj);
    if (!ret)
    {
        if (std::filesystem::exists(path_obj))
        {
            std::filesystem::remove_all(path_obj);
            ret = std::filesystem::create_directories(path_obj);
            if (!ret)
            {
                return Result(Result::Code::DIRECTORY_CREATION_FAIL, "Directory cannot be created (%s)", path.c_str());
            }
        }
        else
        {
            return Result(Result::Code::DIRECTORY_CREATION_FAIL, "Directory cannot be created (%s)", path.c_str());
        }
    }
    return Result(Result::Code::SUCCESS);
}
Result FileSystem::createFile(std::string& path, std::string& content)
{
    {
        std::ofstream ofstream{path};
        if (!ofstream.is_open())
        {
            return Result(Result::Code::FILE_CREATION_FAIL, "File cannot be created (%s)", path.c_str());
        }
    
        constexpr size_t block_size {4096};
        size_t size = content.size();
        const char* data = content.data();
        size_t written_size = 0;

        for (const char* ptr = data; written_size < size; ptr += block_size)
        {
            size_t writing_size = size - written_size >= block_size ? block_size : size - written_size;
            ofstream.write(data, writing_size);
            written_size += writing_size;
        }
    }
    
    return Result(Result::Code::SUCCESS);
}

Result LibraryInterfaceGenerator::Implementation::FileSystem::findAllFilePath(const std::string& path, std::vector<std::string>& file_paths, const std::vector<std::string>& postfixes)
{
    auto original_path = std::filesystem::current_path();
    {
        std::filesystem::current_path(std::filesystem::path{ path });
        auto new_path = std::filesystem::current_path();

        auto root_directory = std::filesystem::recursive_directory_iterator{ "." };
        for (const auto& iter : root_directory)
        {
            if (!iter.is_directory())
            {
                auto& filepath = iter.path();
                auto& temp_path_str = filepath.string();
                std::string path_str = std::string(temp_path_str.begin() + 2, temp_path_str.end());

                for (auto& postfix : postfixes)
                {
                    if (hasPostfix(path_str, postfix))
                    {
                        file_paths.push_back(path_str);
                        break;
                    }
                }
            }
        }
    }
    std::filesystem::current_path(original_path);
    return Result();
}
