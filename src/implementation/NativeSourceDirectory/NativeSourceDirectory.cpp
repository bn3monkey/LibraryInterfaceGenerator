#include "NativeSourceDirectory.hpp"

using namespace LibraryInterfaceGenerator::Implementation;
using namespace LibraryInterfaceGenerator::Implementation::Definition;

#ifdef __linux__
char* delimeter = "/";
#elif _WIN32
char* delimeter = "\\";
#else
char* delimeter = "/";
#endif


NativeSourceDirectory::NativeSourceDirectory(const SymbolTable& symbolTable, std::string root_dir_path) : _symbolTable(symbolTable)
{
    _include_dir_path = root_dir_path;
    _include_dir_path += delimeter;
    _include_dir_path += "include";
    _src_dir_path = root_dir_path;
    _src_dir_path += delimeter;
    _src_dir_path += "src";

    FileSystem::createDirectories(root_dir_path);
}

Result NativeSourceDirectory::make()
{
    auto& package = _symbolTable.getPackage();
    auto& modules = package.modules;

    Result result{ Result::Code::SUCCESS };

    result = FileSystem::createDirectory(_include_dir_path);
    if (!result)
        return result;
    result = FileSystem::createDirectory(_src_dir_path);
    if (!result)
        return result;

    for (auto& submodule : modules)
    {
        result = createModule(*submodule, _include_dir_path, _src_dir_path);
        if (!result)
            return result;
    }

    result = createIncludeFile(package, _include_dir_path);
    if (!result)
        return result;

    return result;
}

Result NativeSourceDirectory::createModule(const SymbolModule& object, std::string& parent_include_path, std::string& parent_src_path)
{
    std::string include_path{ parent_include_path };
    include_path += delimeter;
    include_path += object.name;

    std::string src_path{ parent_src_path };
    src_path += delimeter;
    src_path += object.name;

    Result result;
    result = FileSystem::createDirectory(include_path);
    if (!result)
        return result;
    result = FileSystem::createDirectory(src_path);
    if (!result)
        return result;

    auto& submodules = object.submodules;
    for (auto& submodule : submodules)
    {
        auto result = createModule(*submodule, include_path, src_path);
        if (!result)
            return result;
    }

    auto& interfaces = object.interfaces;
    for (auto& interfaze : interfaces)
    {
        auto result = createInterfaceFile(*interfaze, include_path);
        if (!result)
            return result;
    }

    auto& classes = object.classes;
    for (auto& clazz : classes)
    {
        auto result = createClassFile(*clazz, include_path, src_path);
        if (!result)
            return result;
    }

    auto& enums = object.enums;
    for (auto& enumm : enums)
    {
        auto result = createEnumFile(*enumm, include_path);
        if (!result)
            return result;
    }

    auto& global_methods = object.globla_methods;
    for (auto& global_method : global_methods)
    {
        auto result = createMethodFile(*global_method, include_path, src_path);
        if (!result)
            return result;
    }

    return result;
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createInterfaceFile(const SymbolClass& object, std::string& parent_include_path)
{
    std::string header_path{ parent_include_path };
    header_path += delimeter;
    header_path += object.name;
    header_path += ".hpp";

    Result result;
    std::string header_content;
    result = createInterfaceFileContent(object, header_content);
    if (!result)
        return result;

    result = FileSystem::createFile(header_path, header_content);
    if (!result)
        return result;

    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createClassFile(const SymbolClass& object, std::string& parent_include_path, std::string& parent_src_path)
{
    std::string header_path{ parent_include_path };
    header_path += delimeter;
    header_path += object.name;
    header_path += ".hpp";

    std::string src_path{ parent_src_path };
    src_path += delimeter;
    src_path += object.name;
    src_path += ".cpp";

    Result result;
    std::string header_content;
    std::string cpp_content;
    result = createClassFileContent(object, header_content, cpp_content);
    if (!result)
        return result;

    result = FileSystem::createFile(header_path, header_content);
    if (!result)
        return result;

    result = FileSystem::createFile(src_path, cpp_content);
    if (!result)
        return result;

    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createEnumFile(const SymbolEnum& object, std::string& parent_include_path)
{
    std::string header_path{ parent_include_path };
    header_path += delimeter;
    header_path += object.name;
    header_path += ".hpp";

    Result result;
    std::string header_content;
    result = createEnumFileContent(object, header_content);
    if (!result)
        return result;

    result = FileSystem::createFile(header_path, header_content);
    if (!result)
        return result;

    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createMethodFile(const SymbolMethod& object, std::string& parent_include_path, std::string& parent_src_path)
{
    std::string header_path{ parent_include_path };
    header_path += delimeter;
    header_path += object.name;
    header_path += ".hpp";

    std::string src_path{ parent_src_path };
    src_path += delimeter;
    src_path += object.name;
    src_path += ".cpp";

    Result result;
    std::string header_content;
    std::string cpp_content;
    result = createMethodFileContent(object, header_content, cpp_content);
    if (!result)
        return result;

    result = FileSystem::createFile(header_path, header_content);
    if (!result)
        return result;

    result = FileSystem::createFile(src_path, cpp_content);
    if (!result)
        return result;

    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createIncludeFile(const SymbolPackage& package, std::string& parent_include_path)
{
    std::string header_path{ parent_include_path };
    header_path += delimeter;
    header_path += package.name;
    header_path += ".hpp";

    Result result;
    std::string header_content;
    result = createIncludeFileContent(package, parent_include_path, header_content);
    if (!result)
        return result;

    result = FileSystem::createFile(header_path, header_content);
    if (!result)
        return result;

    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createInterfaceFileContent(const SymbolClass& object, std::string& header_content)
{
    header_content = "";
    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createClassFileContent(const SymbolClass& object, std::string& header_content, std::string& cpp_content)
{
    header_content = "";
    cpp_content = "";
    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createEnumFileContent(const SymbolEnum& object, std::string& header_content)
{
    header_content = "";
    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createMethodFileContent(const SymbolMethod& object, std::string& header_content, std::string& cpp_content)
{
    header_content = "";
    cpp_content = "";
    return Result();
}



Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createIncludeFileContent(const SymbolPackage& package, std::string& parent_include_path, std::string& header_content)
{
    /*
    std::stringstream ss;
    std::string indent;
    std::vector<std::string> moduleNames;
    {
        DefineOnce(ss, moduleNames, indent);
        {
        }
    }
    */
    std::stringstream ss;
    std::string indent;
    {
        DefineOnce defineOnce{ ss, {}, package.name, indent };
        {
            Comment comment{ ss, indent };
            comment.add(package);
        }

        {
            DefineInclude defineInclude{ ss, indent };
            std::vector<std::string> includeFiles;
            Result result = FileSystem::findAllFilePath(parent_include_path, includeFiles, { ".h", ".hpp" });
            if (!result)
                return result;

            for (auto& includeFile : includeFiles)
            {
                defineInclude.addInternal(includeFile);
            }
        }

    }
    header_content = ss.str();
    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createEnumBlock(const SymbolEnum& object, std::vector<std::string>& header_content)
{
    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createInterfaceMethodBlock(const SymbolMethod& object, std::vector<std::string>& header_content)
{
    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createClassMethodBlock(const SymbolMethod& object, std::vector<std::string>& header_content, std::vector<std::string>& cpp_content)
{
    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createDerivedMethodBlock(const SymbolMethod& object, std::vector<std::string>& header_content, std::vector<std::string>& cpp_content)
{
    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createParameterBlock(const SymbolParameter& object, std::string& content)
{
    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createInterfacePropertyBlock(const SymbolProperty& object, std::vector<std::string>& header_content)
{
    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createClassPropertyBlock(const SymbolProperty& object, std::vector<std::string>& header_content, std::vector<std::string>& cpp_content)
{
    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createDerivedPropertyBlock(const SymbolProperty& object, std::vector<std::string>& header_content, std::vector<std::string>& cpp_content)
{
    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::addPropertyDataBlock(const SymbolProperty& object, std::vector<std::string>& properties)
{
    return Result();
}


// 4. Class 제작
/*
Result NativeSourceDirectory::createClass(const nlohmann::json& object, std::string& header_content, std::string& cpp_content)
{
    // 헤더 : 


    // 심볼 테이블 기능
    // 1. 특정 클래스의 네임 스페이스풀 불러오기
    // 2. enum, class, interface의 상대적 헤더파일 불러오기 <- 안에 넣자..

    // 1. 전역 전처리
    // 1.1. 중복 포함 방지 풀 생성
    // 1.2. 공통 헤더를 헤더 풀에 넣기. (메모리풀 토글 통해 강제 포함), 메모리, 스트링 무조건 포함
    // 1.3. 네임 스페이스 풀 생성 (심볼테이블에서 가지고 와야할듯)

    // 2.클래스 전처리
    // 2.1 클래스 이름 가져오기
    // 2.2 베이스 인터페이스 및 클래스에서 필요한 거 쌓기
    // 2.2.1 베이스 인터페이스 및 클래스 파일 경로를 헤더풀에 추가
    // 2.2.2 베이스에 있는 프로퍼티 불러서 베이스 프로퍼티 풀에 넣기
    // 2.2.3 베이스에 있는 메소드 불러서 베이스 메소드 풀에 넣기
    // 2.2.3.1 베이스에 있는 메소드의 파라미터, 프로퍼티에서 
    //         enum, class, interface 타입이 있으면 해당 타입의 위치를 헤더풀에 넣기
    // 2.3 본인의 메소드 프로퍼티 불러서 프로퍼티 풀에 넣기
    // 2.3.1 프로퍼티의 타입이 클래스, 인터페이스, enum이면 헤더 풀에 위치 넣기
    // 2.4 메소드 풀에 생성자가 따로 없으면 기본 생성자 넣기
    // 2.4.1 
    // 2.4 base 프로퍼티 풀과  private data 생성해서 쌓기
    // 2.5 enum 풀에 넣기

    // 3. 쓰기
    // 3.1 중복 포함 방지 열기
    //  3.2 헤더 풀 넣기
    //   3.3 네임스페이스 열기
    //    3.4 클래스 이름, 베이스 클래스 및 인터페이스 넣고 열기
    //     3.5 생성자, 소멸자 넣기
    //     3.6 베이스 프로퍼티 함수 넣기
    //     3.7 프로퍼티 넣기
    //     3.7 베이스 메소드 넣기
    //     3.8 메소드 넣기
    //     3.9 private에 프로퍼티 데이터 넣기
    //    3.4 클래스 닫기
    //   3.3 네임스페이스 닫기
    //  3.2 헤더 풀 닫기
    // 3.1 중복 포함 방지 닫기

    
}
*/