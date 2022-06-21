#include "NativeSourceDirectory.hpp"

using namespace LibraryInterfaceGenerator::Implementation;
using namespace LibraryInterfaceGenerator::Implementation::Definition;

#ifdef __linux__
static char* delimeter = "/";
#elif _WIN32
static char* delimeter = "\\";
#else
static char* delimeter = "/";
#endif


NativeSourceDirectory::NativeSourceDirectory(const NativeExternalLibraryDirectory& libDirectory, 
                const SymbolTable& symbolTable, 
                std::string root_dir_path,
                const char* include_dir_name,
                const char* src_dir_name)
    : _symbolTable(symbolTable), _libDirectory(libDirectory), _include_dir_name(include_dir_name), _src_dir_name(src_dir_name)
{
    _include_dir_path = root_dir_path;
    _include_dir_path += delimeter;
    _include_dir_path += _include_dir_name;
    _src_dir_path = root_dir_path;
    _src_dir_path += delimeter;
    _src_dir_path += _src_dir_name;

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
    if (!global_methods.empty())
    {
        auto result = createMethodFile(object, include_path, src_path);
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

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createMethodFile(const SymbolModule& object, std::string& parent_include_path, std::string& parent_src_path)
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
    std::stringstream ss;
    std::string indent;
    
    {
        DefineOnce defineOnce{ ss, object.parentModules, object.name, indent };

        {
            
            DefineInclude defineInclude{ ss, indent };
            
            defineInclude.addExternal("cstdint");
            defineInclude.addExternal("vector");
            defineInclude.addExternal("string");
            defineInclude.addExternal("memory");



            ss << "\n";
        }

        addForwardDeclaration(ss, indent, object);

        {
            auto classObjects = object.collectAllClassReference();
            for (auto& wClassObject : classObjects)
            {
                if (auto classObject = wClassObject.lock())
                {
                    auto clazz = std::dynamic_pointer_cast<SymbolClass>(classObject);
                    DefineNamespace defineNamespace(ss, clazz->parentModules, indent);
                    defineNamespace.addLine("class " + clazz->name + ";");
                }
            }
        }

        {
            auto enumObjects = object.collectAllEnumReference();
            for (auto& wEnumObject : enumObjects)
            {
                if (auto enumObject = wEnumObject.lock())
                {
                    auto enumm = std::dynamic_pointer_cast<SymbolEnum>(enumObject);
                    DefineNamespace defineNamespace(ss, enumm->parentModules, indent);
                    defineNamespace.addLine("enum class " + enumm->name + ";");
                }
            }
        }

        ss << "\n";
        
        {
            DefineNamespace defineNamespace {ss, object.parentModules, indent};
            {
                Comment comment{ ss, indent };
                comment.add(object);
            }
            {
                std::string objectName {"class "};
                objectName += object.name;
                DefineObject defineObject {ss, objectName, indent};

                defineObject.addLine("public:");
                for (auto& enumObject : object.enums)
                {
                    {
                        Comment comment{ ss, indent };
                        comment.add(*enumObject);
                    }
                    auto lines = createEnumDefinition(*enumObject);
                    for(auto& line : lines)
                    {
                        defineObject.addLine(line);
                    }
                }
                for(auto& propertyObject : object.properties)
                {
                    {
                        Comment comment{ ss, indent };
                        comment.add(*propertyObject);
                    }
                    auto lines = createInterfacePropertyDeclaration(*propertyObject);
                    for(auto& line : lines)
                    {
                        defineObject.addLine(line);
                    }
                }
                for (auto& methodObject : object.methods)
                {
                    {
                        Comment comment{ ss, indent };
                        comment.add(*methodObject);
                    }
                    auto line = createInterfaceMethodDeclaration(object, *methodObject);
                    defineObject.addLine(line);
                }
            }
        }
    }
    header_content = ss.str();
    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createClassFileContent(const SymbolClass& object, std::string& header_content, std::string& cpp_content)
{
    // 4. Class 제작
/*
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

*/
    std::stringstream ss;
    std::string indent;

    auto& constructors = object.constructors;
    auto& methods = object.methods;
    auto baseMethods = object.getBaseMethods();
    auto& properties = object.properties;
    auto& baseProperties = object.getBaseProperties();


    {
        DefineOnce defineOnce{ ss, object.parentModules, object.name, indent };

        {

            DefineInclude defineInclude{ ss, indent };

            defineInclude.addExternal("cstdint");
            defineInclude.addExternal("vector");
            defineInclude.addExternal("string");
            defineInclude.addExternal("memory");

            ss << "\n";
            // 전방 선언하거나 헤더 포함!

            {
                auto baseObjects = object.bases;
                for (auto& wbaseObject : baseObjects)
                {
                    if (auto baseObject = wbaseObject.lock())
                    {
                        std::string path;

                        auto& currentModules = object.parentModules;
                        auto& baseModules = object.parentModules;

                        size_t baseDepth = baseModules.size();
                        size_t currentDepth = currentModules.size();

                        size_t depthIndicator = 0;
                        for (depthIndicator = currentDepth-1; depthIndicator > baseDepth; depthIndicator--)
                        {
                            path += "../";
                        }
                        for (; depthIndicator >= 0; depthIndicator--)
                        {
                            if (baseModules[depthIndicator] == currentModules[depthIndicator])
                                break;
                            path += "../";
                        }
                        for (; depthIndicator < baseDepth; depthIndicator++)
                        {
                            path += baseModules[depthIndicator];
                            path += "/";
                        }
                        
                        if (baseObject->parentObjects.empty())
                        {
                            path += baseObject->name;
                            
                        }
                        else
                        {
                            path += baseObject->parentObjects[0];
                        }
                        path += ".hpp";
                        defineInclude.addInternal(path);
                    }
                }
            }
        }

        addForwardDeclaration(ss, indent, object);

        {
            DefineNamespace defineNamespace{ ss, object.parentModules, indent };
            {
                Comment comment{ ss, indent };
                comment.add(object);
            }
            {
                std::string objectName{ "class " };
                objectName += object.name;
                if (!object.bases.empty())
                {
                    objectName += " : ";
                    for (auto& wbaseClass : object.bases)
                    {
                        if (auto baseClass = wbaseClass.lock())
                        {
                            objectName += "public ";
                            objectName += baseClass->getCppName();
                            objectName += ", ";
                        }
                    }
                    objectName.pop_back();
                    objectName.pop_back();
                }

                DefineObject defineObject{ ss, objectName, indent };

                defineObject.addLine("public:");
                for (auto& enumObject : object.enums)
                {
                    {
                        Comment comment{ ss, indent };
                        comment.add(*enumObject);
                    }
                    auto lines = createEnumDefinition(*enumObject);
                    for (auto& line : lines)
                    {
                        defineObject.addLine(line);
                    }
                }

                for (auto& constructor : constructors)
                {
                    {
                        Comment comment{ ss, indent };
                        comment.add(*constructor);
                    }
                    auto line = createConstructorDeclaration(object, *constructor);
                    defineObject.addLine(line);
                }
                {
                    auto line = createDestructorDeclaration(object);
                    defineObject.addLine(line);
                }
                for (auto& propertyObject : properties)
                {
                    {
                        Comment comment{ ss, indent };
                        comment.add(*propertyObject);
                    }
                    auto lines = createClassPropertyDeclaration(*propertyObject);
                    for (auto& line : lines)
                    {
                        defineObject.addLine(line);
                    }
                }

                for (auto& basePropertyObject : baseProperties)
                {
                    {
                        Comment comment{ ss, indent };
                        comment.add(*basePropertyObject);
                    }
                    auto lines = createDerivedPropertyDeclaration(*basePropertyObject);
                    for (auto& line : lines)
                    {
                        defineObject.addLine(line);
                    }
                }


                for (auto& methodObject : methods)
                {
                    {
                        Comment comment{ ss, indent };
                        comment.add(*methodObject);
                    }
                    auto line = createClassMethodDeclaration(object, *methodObject);
                    defineObject.addLine(line);
                }

                for (auto& baseMethod : baseMethods)
                {
                    {
                        Comment comment{ ss, indent };
                        comment.add(*baseMethod);
                    }
                    auto line = createDerivedMethodDeclaration(*baseMethod);
                    defineObject.addLine(line);                    
                }

                defineObject.addLine("\n");
                defineObject.addLine("private:");

                for (auto& propertyObject : object.properties)
                {
                    auto line = createPropertyField(*propertyObject);
                    defineObject.addLine(line);
                }

                for (auto& propertyObject : baseProperties)
                {
                    auto line = createPropertyField(*propertyObject);
                    defineObject.addLine(line);
                }
            }
        }
    }
    header_content = ss.str();

    ss.str("");
    {
        DefineInclude defineInclude{ ss, indent };

        // out of src directory
        std::string prefix = "../";
        std::string postfix = "include/";
        for (size_t idx = 1; idx < object.parentModules.size(); idx++)
        {
            prefix += "../";
            postfix += object.parentModules[idx];
            postfix += "/";
        }

        postfix += object.name;
        postfix += ".hpp";

        std::string header_path = prefix;
        header_path += postfix;

        defineInclude.addInternal(header_path);
        if (_libDirectory.existsExternalTool(NativeExternalLibraryDirectory::ExternalTool::Log))
        {
            auto header_path = _libDirectory.getRelativeHeaderPath(NativeExternalLibraryDirectory::ExternalTool::Log, object);
            defineInclude.addInternal(header_path);
        }
        if (_libDirectory.existsExternalTool(NativeExternalLibraryDirectory::ExternalTool::MemoryPool))
        {
            auto header_path = _libDirectory.getRelativeHeaderPath(NativeExternalLibraryDirectory::ExternalTool::MemoryPool, object);
            defineInclude.addInternal(header_path);
        }

        // 전방 선언하거나 헤더 포함!

        ss << "\n";

        for (auto& constructor : constructors)
        {
            auto ret = createConstructorDefinition(object, *constructor);
            for (auto& line : ret)
            {
                ss << indent << line << "\n";
            }
            ss << "\n";
        }

        auto ret = createDestructorDefinition(object);
        for (auto& line : ret)
        {
            ss << indent << line << "\n";
        }
        ss << "\n";
        
        for (auto& method : methods)
        {
            auto ret = createClassMethodDefinition(object, *method);
            for (auto& line : ret)
            {
                ss << indent << line << "\n";
            }
            ss << "\n";
        }
        for (auto& method : baseMethods)
        {
            auto ret = createClassMethodDefinition(object, *method);
            for (auto& line : ret)
            {
                ss << indent << line << "\n";
            }
            ss << "\n";
        }
        for (auto& prop : properties)
        {
            auto ret = createPropertyDefinition(object, *prop);
            for (auto& line : ret)
            {
                ss << indent << line << "\n";
            }
            ss << "\n";
        }
        for (auto& prop : baseProperties)
        {
            auto ret = createPropertyDefinition(object, *prop);
            for (auto& line : ret)
            {
                ss << indent << line << "\n";
            }
            ss << "\n";
        }
    }
    cpp_content = ss.str();

    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createEnumFileContent(const SymbolEnum& object, std::string& header_content)
{
    std::stringstream ss;
    std::string indent;
    
    {
        DefineOnce defineOnce{ ss, object.parentModules, object.name, indent };

        {
            
            DefineInclude defineInclude{ ss, indent };
            
            defineInclude.addExternal("cstdint");
            defineInclude.addExternal("vector");
            defineInclude.addExternal("string");
            defineInclude.addExternal("memory");

            ss << "\n";
            // 전방 선언하거나 헤더 포함!           
        }
        
        {
            DefineNamespace defineNamespace {ss, object.parentModules, indent};
            {
                Comment comment{ ss, indent };
                comment.add(object);
            }
            auto ret = createEnumDefinition(object);
            for (auto& line : ret)
            {
                ss << indent << line << "\n";
            }
        }
    }
    header_content = ss.str();
    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createMethodFileContent(const SymbolModule& object, std::string& header_content, std::string& cpp_content)
{
    std::stringstream ss;
    std::string indent;
    
    {
        DefineOnce defineOnce{ ss, object.moduleNames, "", indent };

        {
            
            DefineInclude defineInclude{ ss, indent };
            
            defineInclude.addExternal("cstdint");
            defineInclude.addExternal("vector");
            defineInclude.addExternal("string");
            defineInclude.addExternal("memory");

            ss << "\n";
            // 전방 선언하거나 헤더 포함!           
        }
        
        addForwardDeclaration(ss, indent, object);

        {
            DefineNamespace defineNamespace {ss, object.moduleNames, indent};
            for (auto& method : object.globla_methods)
            {
                {
                    Comment comment{ ss, indent };
                    comment.add(*method);
                }
                ss << indent << createStaticMethodDeclaration(*method) << "\n\n";
            }
        }       
    }
    header_content = ss.str();
    
    ss.str("");
    {
        DefineInclude defineInclude{ ss, indent };

        // out of src directory
        std::string prefix = "../";
        std::string postfix = "include/";
        for (size_t idx = 1; idx < object.moduleNames.size(); idx++)
        {
            prefix += "../";
            postfix += object.moduleNames[idx];
            postfix += "/";
        }

        postfix += object.name;
        postfix += ".hpp";
        
        std::string header_path = prefix;
        header_path += postfix;

        defineInclude.addInternal(header_path);
        if (_libDirectory.existsExternalTool(NativeExternalLibraryDirectory::ExternalTool::Log))
        {
            auto header_path = _libDirectory.getRelativeHeaderPath(NativeExternalLibraryDirectory::ExternalTool::Log, object);
            defineInclude.addInternal(header_path);
        }
        if (_libDirectory.existsExternalTool(NativeExternalLibraryDirectory::ExternalTool::MemoryPool))
        {
            auto header_path = _libDirectory.getRelativeHeaderPath(NativeExternalLibraryDirectory::ExternalTool::MemoryPool, object);
            defineInclude.addInternal(header_path);
        }

        ss << "\n";
        for (auto& method : object.globla_methods)
        {
            auto ret = createStaticMethodDefinition(*method);
            for (auto& line : ret)
            {
                ss << indent << line << "\n";
            }
            ss << "\n";
        }
    }
    cpp_content = ss.str();

    return Result();
}



Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createIncludeFileContent(const SymbolPackage& package, std::string& parent_include_path, std::string& header_content)
{

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

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::addForwardDeclaration(std::stringstream& ss, std::string& indent, const SymbolClass& object)
{
    {
        auto classObjects = object.collectAllClassReference();
        for (auto& wClassObject : classObjects)
        {
            if (auto classObject = wClassObject.lock())
            {
                auto clazz = std::dynamic_pointer_cast<SymbolClass>(classObject);
                DefineNamespace defineNamespace(ss, clazz->parentModules, indent);
                defineNamespace.addLine("class " + clazz->name + ";");
            }
        }
    }

    {
        auto enumObjects = object.collectAllEnumReference();
        for (auto& wEnumObject : enumObjects)
        {
            if (auto enumObject = wEnumObject.lock())
            {
                auto enumm = std::dynamic_pointer_cast<SymbolEnum>(enumObject);
                DefineNamespace defineNamespace(ss, enumm->parentModules, indent);
                defineNamespace.addLine("enum class " + enumm->name + ";");
            }
        }
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::addForwardDeclaration(std::stringstream& ss, std::string& indent, const SymbolModule& object)
{
    {
        auto classObjects = object.collectAllClassReference();
        for (auto& wClassObject : classObjects)
        {
            if (auto classObject = wClassObject.lock())
            {
                auto clazz = std::dynamic_pointer_cast<SymbolClass>(classObject);
                DefineNamespace defineNamespace(ss, clazz->parentModules, indent);
                defineNamespace.addLine("class " + clazz->name + ";");
            }
        }
    }

    {
        auto enumObjects = object.collectAllEnumReference();
        for (auto& wEnumObject : enumObjects)
        {
            if (auto enumObject = wEnumObject.lock())
            {
                auto enumm = std::dynamic_pointer_cast<SymbolEnum>(enumObject);
                DefineNamespace defineNamespace(ss, enumm->parentModules, indent);
                defineNamespace.addLine("enum class " + enumm->name + ";");
            }
        }
    }
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createEnumDefinition(const SymbolEnum& object)
{
    std::vector<std::string> ret;

    std::string name{ "enum class " };
    name += object.name;
    ret.push_back(name);
   
    ret.push_back("{");

    for (auto& value : object.values)
    {
        std::stringstream ss;
        ss << "    " << value.first << " = " << value.second << ",";
        ret.push_back(ss.str());
    }
    ret.push_back("};");
    return ret;
}

std::string LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createMethodDeclaration(const SymbolMethod& object)
{
    std::string method{ "" };
    method += object.type->toCppType();
    method += " ";
    method += object.name;
    method += "(";

    if (!object.parameters.empty())
    {
        method += createParametersDefinition(object);
    }

    method += ")";
    return method;
}

std::string LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createStaticMethodDeclaration(const SymbolMethod& object)
{
    std::string method = createMethodDeclaration(object);
    method += ";";
    return method;
}

std::string LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createClassMethodDeclaration(const SymbolClass& clazz, const SymbolMethod& object)
{   
    std::string method;
    method = createMethodDeclaration(object);
    method += ";";
    return method;
}

std::string LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createInterfaceMethodDeclaration(const SymbolClass& clazz, const SymbolMethod& object)
{
    std::string method;

    method = "virtual ";
    method += createMethodDeclaration(object);
    method += "= 0;";
    
    return method;
}



std::string LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createDerivedMethodDeclaration(const SymbolMethod& object)
{
    std::string method;
    method = createMethodDeclaration(object);
    method += " override;";
    
    return method;
}

std::string LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createConstructorDeclaration(const SymbolClass& clazz, const SymbolMethod& object)
{
    std::string constructor;
    constructor = "explicit ";
    constructor += clazz.name;
    constructor += "(";
     if (!object.parameters.empty())
    {
        constructor += createParametersDefinition(object);
    }

    constructor += ");";
    return constructor;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createConstructorDefinition(const SymbolClass& clazz, const SymbolMethod& object)
{
    std::vector<std::string> ret;
    std::string constructor = createScope(clazz);
    constructor += clazz.name;
    constructor += "(";
    if (!object.parameters.empty())
    {
        constructor += createParametersDefinition(object);
    }

    constructor += ")";
    ret.push_back(constructor);
    ret.push_back("{");
    ret.push_back("");
    ret.push_back("}");
    return ret;
}

std::string LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createDestructorDeclaration(const SymbolClass& clazz)
{
    std::string ret = "virtual ~";
    ret += clazz.name;
    ret += "();";
    return ret;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createDestructorDefinition(const SymbolClass& clazz)
{
    std::vector<std::string> ret;
    std::string name = createScope(clazz);
    name += "~";
    name += clazz.name;
    name += "()";
    ret.push_back(name);
    ret.push_back("{");
    ret.push_back(" // Release Resources;");
    ret.push_back("}");
    return ret;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createStaticMethodDefinition(const SymbolMethod& object)
{
    std::vector<std::string> ret;
    std::string method{ "" };
    method += object.type->toCppType();
    method += " ";
    method += createScope(object);
    method += object.name;
    method += "(";

    if (!object.parameters.empty())
    {
        method += createParametersDefinition(object);
    }

    method += ")";
    ret.push_back(method);
    ret.push_back("{");
    if (object.type->getTypeName() == SymbolType::Name::VOID)
    {
        ret.push_back("");
    }
    else
    {
        ret.push_back("\treturn " + object.type->toCppType() + "();");
    }
    ret.push_back("}");
    return ret;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createClassMethodDefinition(const SymbolClass& clazz, const SymbolMethod& object)
{
    std::vector<std::string> ret;
    std::string method{ "" };
    method += object.type->toCppType();
    method += " ";
    method += createScope(clazz);
    method += object.name;
    method += "(";

    if (!object.parameters.empty())
    {
        method += createParametersDefinition(object);
    }

    method += ")";
    ret.push_back(method);
    ret.push_back("{");
    if (object.type->getTypeName() == SymbolType::Name::VOID)
    {
        ret.push_back("");
    }
    else
    {
        ret.push_back("\treturn " + object.type->toCppType() + "();");
    }
    ret.push_back("}");
    return ret;
}



std::string LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createParametersDefinition(const SymbolMethod& object)
{
    std::string line;
    for (auto& parameter : object.parameters)
    {
        line += createParameterDefinition(*parameter);
        line += ", ";
    }
    line.pop_back();
    line.pop_back();
    return line;
}

std::string LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createParameterDefinition(const SymbolParameter& object)
{
    std::string content = object.io == SymbolParameter::IO::IN ? "const " : "";
    content += object.type->toCppType();
    content += object.type->isPrimitive() ? " " : "& ";
    content += object.name;
    return content;
}


std::string LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createPropertyName(const SymbolProperty& object)
{
    std::string propertyName = object.name;
    char firstChar = propertyName[0];
    if ('a' <= firstChar && firstChar <= 'z')
        firstChar += ('A' - 'a');
    propertyName[0] = firstChar;
    return propertyName;
}
std::string LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createPropertySetterDeclaration(const std::string& propertyName, const SymbolProperty& object)
{
    std::string setter{ "void set" };
    setter += propertyName;
    setter += "(";
    setter += object.type->toCppType();
    if (!object.type->isPrimitive())
        setter += "&";
    setter += " value)";
    return setter;
}

std::string LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createPropertyGetterDeclaration(const std::string& propertyName, const SymbolProperty& object)
{
    std::string getter{ object.type->toCppType() };
    if (!object.type->isPrimitive())
        getter += "&";
    getter += " get";
    getter += propertyName;
    getter += "()";
    return getter;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createClassPropertyDeclaration(const SymbolProperty& object)
{
    std::vector<std::string> ret;

    std::string propertyName = createPropertyName(object);

    std::string getter = createPropertyGetterDeclaration(propertyName, object);
    getter += ";";
    ret.push_back(getter);

    if (!object.readonly)
    {
        std::string setter = createPropertySetterDeclaration(propertyName, object);
        setter += ";";
        ret.push_back(setter);
    }

    return ret;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createInterfacePropertyDeclaration(const SymbolProperty& object)
{
    std::vector<std::string> ret;

    std::string propertyName = createPropertyName(object);

    std::string getter = "virtual ";
    getter += createPropertyGetterDeclaration(propertyName, object);
    getter += "= 0;";
    ret.push_back(getter);

    if (!object.readonly)
    {
        std::string setter = "virtual ";
        setter += createPropertySetterDeclaration(propertyName, object);
        setter += "= 0;";
        ret.push_back(setter);
    }

    return ret;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createDerivedPropertyDeclaration(const SymbolProperty& object)
{
    std::vector<std::string> ret;

    std::string propertyName = createPropertyName(object);

    std::string getter = createPropertyGetterDeclaration(propertyName, object);
    getter += " override;";
    ret.push_back(getter);

    if (!object.readonly)
    {
        std::string setter = createPropertySetterDeclaration(propertyName, object);
        setter += " override;";
        ret.push_back(setter);
    }

    return ret;
}

std::vector<std::string> LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createPropertyDefinition(const SymbolClass& clazz, const SymbolProperty& object)
{
    std::vector<std::string> ret;

    std::string scope = createScope(clazz);

    std::string propertyName = object.name;
    char firstChar = propertyName[0];
    if ('a' <= firstChar && firstChar <= 'z')
        firstChar += ('A' - 'a');
    propertyName[0] = firstChar;

    std::string getter = object.type->toCppType();
    if (!object.type->isPrimitive())
        getter += "&";
    getter += " ";
    getter += scope;
    getter += "get";
    getter += propertyName;
    getter += "()";

    ret.push_back(getter);
    ret.push_back("{");

    std::string return_line{ "   return _" };
    return_line += object.name;
    return_line += ";";

    ret.push_back(return_line);

    ret.push_back("}");

    if (!object.readonly)
    {
        std::string setter{ "void " };
        setter += scope;
        setter += "set";
        setter += propertyName;
        setter += "(";
        setter += object.type->toCppType();
        if (!object.type->isPrimitive())
            setter += "&";
        setter += " value)";
        ret.push_back(setter);
        ret.push_back("{");

        std::string return_line{ "   _" };
        return_line += object.name;
        return_line += " = value;";
        ret.push_back(return_line);

        ret.push_back("}");
    }

    return ret;
}

std::string LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createPropertyField(const SymbolProperty& object)
{
    std::string line{ object.type->toCppType() };
    line += " _";
    line += object.name;
    line += ";";
    return line;
}

std::string LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createScope(const SymbolMethod& method)
{
    std::string scope;
    auto& moduleNames = method.parentModules;
    for (auto& moduleName : moduleNames)
    {
        scope += moduleName;
        scope += "::";
    }
    return scope;
}

std::string LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createScope(const SymbolClass& clazz)
{
    std::string scope;
    auto& moduleNames = clazz.parentModules;
    auto& objectNames = clazz.parentObjects;
    for (auto& moduleName : moduleNames)
    {
        scope += moduleName;
        scope += "::";
    }
    for (auto& objectName : objectNames)
    {
        scope += objectName;
        scope += "::";
    }
    scope += clazz.name;
    scope += "::";

    return scope;
}

/*
std::set<std::string> LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::collectDeclarations(const SymbolClass& clazz)
{
    return std::set<std::string>();
}

std::set<std::string> LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::collectDeclarations(const SymbolMethod& method)
{
    return std::set<std::string>();
}
*/

