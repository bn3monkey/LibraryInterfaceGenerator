#include "FileSystem.hpp"

using namespace LibraryInterfaceGenerator::Implementation;

Result FileSystem::createDirectory(const std::string& path)
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
Result FileSystem::createDirectories(const std::string& path)
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
Result FileSystem::createFile(const std::string& path, const std::string& content)
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
            ofstream.write(ptr, writing_size);
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

static Result copyFile(const std::string& src_path, const std::string& dest_path)
{
    {
        std::ifstream ifs(src_path, std::ios::binary);
        if (!ifs) {
            return Result(Result::Code::FILE_OPEN_FAIL, "File cannot be opened (%s)", src_path.c_str());
        }

        std::ofstream ofs(dest_path, std::ios::binary);
        if (!ofs)
        {
            return Result(Result::Code::FILE_OPEN_FAIL, "File cannot be opened (%s)", dest_path.c_str());
        }

        ofs << ifs.rdbuf();

        ifs.close();
        ofs.close();
    }
    return Result();
}
static Result copyDirectory(const std::string& src_path ,const std::string& dest_path)
{
    Result result;
    auto src_directory = std::filesystem::directory_iterator{ src_path };

    for (const auto iter : src_directory)
    //for (auto& name : names)
    {
        auto child_name = iter.path().filename().string();
        //auto child_name = name;
        auto child_src_path = src_path + "\\" + child_name;
        auto child_dest_path = dest_path + "\\" + child_name;

        if (iter.is_directory())
        //if (name.find_first_of('.', 0) != name.length())
        {
            if (!std::filesystem::exists(child_dest_path))
                std::filesystem::create_directories(child_dest_path);
            result = copyDirectory(child_src_path, child_dest_path);
        }
        else {
            result = copyFile(child_src_path, child_dest_path);
        }

        if (!result) {
            return result;
        }
    }
    return result;
}
Result LibraryInterfaceGenerator::Implementation::FileSystem::copyDirectories(const std::string& src_path, const std::string& dest_path)
{
    if (!std::filesystem::exists(dest_path))
    {
        std::filesystem::create_directories(dest_path);
    }
    return copyDirectory(src_path, dest_path);
}

Result LibraryInterfaceGenerator::Implementation::FileSystem::removeDirectory(const std::string& path)
{
    std::filesystem::remove_all(path);
    return Result();
}
