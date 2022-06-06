#include "NativeSourceDirectory.hpp"

using namespace LibraryInterfaceGenerator::Implementation;
using namespace LibraryInterfaceGenerator::Implementation::Definition;

NativeSourceDirectory::NativeSourceDirectory(std::string json_content, std::string root_dir_path)
{
    try
    {  
        _object = nlohmann::json::parse(json_content);
    }
    catch(const std::exception& e)
    {
        _result = Result(Result::Code::JSON_PARSE_FAIL, e.what());
    }
  
    _include_dir_path = root_dir_path + "/include";
    _src_dir_path = root_dir_path + "/src";
}

Result NativeSourceDirectory::make()
{

}

// 1. Class Table 제작 (Class, Interface, enum class)
Result NativeSourceDirectory::createClassTable(const nlohmann::json& object)
{

}

// 2. Module Directory 제작
Result NativeSourceDirectory::createModule(const nlohmann::json& object, std::string& parent_dir_path)
{
    auto order_iter = object.find(Field::Order);
    if (order_iter == object.end())
    {
        return Result(Result::Code::JSON_OBJECT_HAS_NO_ORDER);
    }

    auto order = order_iter->get<std::string>(); 

    auto name_iter = object.find(Field::Name);
    if (name_iter == object.end())
    {
        return Result(Result::Code::JSON_OBJECT_HAS_NO_NAME);
    }

    auto name = name_iter->get<std::string>();
    
    auto dir_path {parent_dir_path};
    dir_path += "/";
    dir_path += name;

    {
        auto ret = FileSystem::createDirectory(dir_path);
        if (!ret)
            return ret;
    }

    auto child_iter = object.find(Field::Childs);
    if (child_iter == object.end())
    {
        return Result(Result::Code::SUCCESS);
    }

    auto childs = *child_iter;
    for (auto& child : childs)
    {
        auto order_iter = object.find(Field::Order);
        if (order_iter == object.end())
        {
            return Result(Result::Code::JSON_OBJECT_HAS_NO_ORDER);
        }

        auto order = order_iter->get<std::string>();

        if (order == Order::Module)
        {
            Result ret = createModule(child, dir_path);
            if (!ret)
            {
                return ret; 
            }
        }
        else if (order == Order::Interface)
        {
            
        }
        else if (order == Order::Class)
        {
            
        }
        else if (order == Order::Enum)
        {
            
        }
    }
}

Result NativeSourceDirectory::createInterfaceFile(const nlohmann::json& object, std::string& parent_dir_path)
{
    auto name_iter = _object.find(Field::Name);
    if (name_iter == _object.end())
    {
        return Result(Result::Code::JSON_OBJECT_HAS_NO_NAME);
    }

    auto name = name_iter->get<std::string>();

    std::string header_content;
    std::string cpp_content;
    Result ret = createInterface(_object, header_content, cpp_content);
    if (!ret)
    {
        return ret;
    }
    {
        std::string header_path {parent_dir_path};
        header_path += "/";
        header_path += name;
        header_path += ".hpp";
        auto ret = FileSystem::createFile(header_path, header_content);
        if (!ret)
            return ret;
    }
    {
        std::string cpp_path {parent_dir_path};
        cpp_path += "/";
        cpp_path += name;
        cpp_path += ".cpp";
        auto ret = FileSystem::createFile(cpp_path, cpp_content);
        if (!ret)
            return ret; 
    }
}
Result NativeSourceDirectory::createClassFile(const nlohmann::json& object, std::string& parent_dir_path)
{
    auto name_iter = object.find(Field::Name);
    if (name_iter == object.end())
    {
        return Result(Result::Code::JSON_OBJECT_HAS_NO_NAME);
    }
    auto name = name_iter->get<std::string>();
    std::string header_content;
    std::string cpp_content;
    Result ret = createClass(object, header_content, cpp_content);
    if (!ret)
    {
        return ret;
    }
    {
        std::string header_path {parent_dir_path};
        header_path += "/";
        header_path += name;
        header_path += ".hpp";
        auto ret = FileSystem::createFile(header_path, header_content);
        if (!ret)
            return ret;
    }
    {
        std::string cpp_path {parent_dir_path};
        cpp_path += "/";
        cpp_path += name;
        cpp_path += ".cpp";
        auto ret = FileSystem::createFile(cpp_path, cpp_content);
        if (!ret)
            return ret; 
    }
}
Result NativeSourceDirectory::createEnumFile(const nlohmann::json& object, std::string& parent_dir_path)
{
    auto name_iter = object.find(Field::Name);
    if (name_iter == object.end())
    {
        return Result(Result::Code::JSON_OBJECT_HAS_NO_NAME);
    }
    auto name = name_iter->get<std::string>();

    std::string header_content;
    Result ret = createGlobalEnum(object, header_content);
    if (!ret)
    {
        return ret;
    }
    {
        std::string header_path {parent_dir_path};
        header_path += "/";
        header_path += name;
        header_path += ".hpp";
        auto ret = FileSystem::createFile(header_path, header_content);
        if (!ret)
            return ret;
    }
}
Result NativeSourceDirectory::createMethodFile(const nlohmann::json& object, std::string& parent_dir_path)
{
    auto name_iter = object.find(Field::Name);
    if (name_iter == object.end())
    {
        return Result(Result::Code::JSON_OBJECT_HAS_NO_NAME);
    }
    auto name = name_iter->get<std::string>();

    std::string header_content;
    std::string cpp_content;
    Result ret = createGlobalMethod(object, header_content, cpp_content);
    if (!ret)
    {
        return ret;
    }
    {
        std::string header_path {parent_dir_path};
        header_path += "/";
        header_path += name;
        header_path += ".hpp";
        auto ret = FileSystem::createFile(header_path, header_content);
        if (!ret)
            return ret;
    }
    {
        std::string cpp_path {parent_dir_path};
        cpp_path += "/";
        cpp_path += name;
        cpp_path += ".cpp";
        auto ret = FileSystem::createFile(cpp_path, cpp_content);
        if (!ret)
            return ret; 
    }
}

// 3. Interface 제작
Result NativeSourceDirectory::createInterface(const nlohmann::json& object, std::string& header_content, std::string& cpp_content)
{

}

// 4. Class 제작
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
            
// 5. Enum 제작
Result createGlobalEnum(const nlohmann::json& object, std::string& header_content)
{

}
Result NativeSourceDirectory::createEnum(const nlohmann::json& object, std::string& header_content)
{

}

// 5. Method 제작
Result NativeSourceDirectory::createGlobalMethod(const nlohmann::json& object, std::string& header_content, std::string& cpp_content)
{

}            
Result NativeSourceDirectory::createMethod(const nlohmann::json& object, std::string& header_content, std::string& cpp_content)
{

}

// 6. Parameter 제작
Result NativeSourceDirectory::createParameter(const nlohmann::json& object, std::string& header_content, std::string& cpp_content)
{

}

// 7. Property 제작
Result NativeSourceDirectory::createProperty(const nlohmann::json& object, std::string& header_content, std::string& cpp_content)
{

}