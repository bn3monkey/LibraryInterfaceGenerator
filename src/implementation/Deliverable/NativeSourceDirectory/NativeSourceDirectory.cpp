#include "NativeSourceDirectory.hpp"
#include "../Converter/CXXConverter.hpp"

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

    auto& global_methods = object.global_methods;
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

    auto ss = createInterfaceFileContent(object);
    auto content = ss.str();

    result = FileSystem::createFile(header_path, content);
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
    
    {
        auto ss = createClassHeaderFileContent(object);
        auto content = ss.str();

        result = FileSystem::createFile(header_path, content);
        if (!result)
            return result;
    }

    {
        auto ss = createClassCppFileContent(object);
        auto content = ss.str();

        result = FileSystem::createFile(src_path, content);
        if (!result)
            return result;
    }

    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createEnumFile(const SymbolEnum& object, std::string& parent_include_path)
{
    std::string header_path{ parent_include_path };
    header_path += delimeter;
    header_path += object.name;
    header_path += ".hpp";

    Result result;

    {
        auto ss = createEnumFileContent(object);
        auto content = ss.str();

        result = FileSystem::createFile(header_path, content);
        if (!result)
            return result;
    }

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

    {
        auto ss = createMethodHeaderFileContent(object);
        auto content = ss.str();

        result = FileSystem::createFile(header_path, content);
        if (!result)
            return result;
    }

    {
        auto ss = createMethodCppFileContent(object);
        auto content = ss.str();

        result = FileSystem::createFile(src_path, content);
        if (!result)
            return result;
    }

    return Result();
}

Result LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createIncludeFile(const SymbolPackage& package, std::string& parent_include_path)
{
    std::string header_path{ parent_include_path };
    header_path += delimeter;
    header_path += package.name;
    header_path += ".hpp";

    Result result;

    std::vector<std::string> include_files;
    result = FileSystem::findAllFilePath(parent_include_path, include_files, { ".h", ".hpp" });
    if (!result)
        return result;

    auto ss = createIncludeFileContent(package, include_files);
    auto header_content = ss.str();

    result = FileSystem::createFile(header_path, header_content);
    if (!result)
        return result;

    return Result();
}

SourceStream LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createInterfaceFileContent(const SymbolClass& object)
{
    SourceStream ss;
    {
        HeaderGuardCXXSourceScopedStream headerguard_scope{ ss, object.parentModules, object.name };

        ExternalIncludeCXXSourceStream ex1{ ss, "cstdint" };
        ExternalIncludeCXXSourceStream ex2{ ss, "vector" };
        ExternalIncludeCXXSourceStream ex2{ ss, "string" };
        ExternalIncludeCXXSourceStream ex2{ ss, "memory" };

        createForwardDeclaration(ss, object);

        {
            NamespaceCXXSourceScopedStream namespace_scope{ ss, object.parentModules };
            createComment(ss, object);

            {
                ClassCXXSourceScopedStream class_scope{ ss, false, object.name };
                {
                    AccessCXXSourceScopedStream public_scope{ ss, AccessCXXSourceScopedStream::Specifier::Public };
                    for (auto& enum_object : object.enums)
                    {
                        createComment(ss, *enum_object);
                        createEnumDefinition(ss, *enum_object);
                    }

                    for (auto& property_object : object.properties)
                    {
                        createComment(ss, *property_object);
                        createInterfacePropertyDeclaration(ss, *property_object);
                    }

                    for (auto& method_object : object.methods)
                    {
                        auto& method = method_object.first;
                        createComment(ss, *method);
                        createInterfaceMethodDeclaration(ss, object, *method);
                    }
                }
            }
        }
    }

    return ss;
}

SourceStream LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createClassHeaderFileContent(const SymbolClass& object)
{
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
    SourceStream ss;

    auto& base_objects = object.bases;

    auto& enums = object.enums;

    auto& constructors = object.constructors;
    
    auto& methods = object.methods;
    auto base_methods = object.getBaseMethods();
    
    auto& properties = object.properties;
    auto& base_properties = object.getBaseProperties();
    
    {
        HeaderGuardCXXSourceScopedStream headerguard_scope{ ss, object.parentModules, object.name };

        {
            ExternalIncludeCXXSourceStream ex{ ss, "cstdint" };
        }
        {
            ExternalIncludeCXXSourceStream ex{ ss, "vector" };
        }
        {
            ExternalIncludeCXXSourceStream ex{ ss, "string" };
        }
        {
            ExternalIncludeCXXSourceStream ex{ ss, "memory" };
        }

        std::vector<std::string> base_classes_names;

        for (auto& w_base_object : base_objects)
        {
            if (auto base_object = w_base_object.lock())
            {
                base_classes_names.push_back(base_object->getCppName());

                auto internal_header_path = createRelativeHeaderPath(object, *base_object);
                InternalIncludeCXXSourceStream in{ ss, internal_header_path };
            }
        }

        createForwardDeclaration(ss, object);

        {
            NamespaceCXXSourceScopedStream namespace_scope{ ss, object.parentModules };
            createComment(ss, object);

            {
                ClassCXXSourceScopedStream class_scope{ ss, false, object.name, base_classes_names };

                {
                    AccessCXXSourceScopedStream public_scope{ ss, AccessCXXSourceScopedStream::Specifier::Public };
                    for (auto& enum_object : enums)
                    {
                        createComment(ss, *enum_object);
                        createEnumDefinition(ss, *enum_object);
                    }

                    for (auto& constructor_object : constructors)
                    {
                        auto& constructor = constructor_object.first;
                        createComment(ss, *constructor);
                        createConstructorDeclaration(ss, object, *constructor);
                    }

                    createDestructorDeclaration(ss, object);

                    for (auto& property_object : properties)
                    {
                        createComment(ss, *property_object);
                        createClassPropertyDeclaration(ss, *property_object);
                    }

                    for (auto& base_property_object : base_properties)
                    {
                        createComment(ss, *base_property_object);
                        createDerivedPropertyDeclaration(ss, *base_property_object);
                    }


                    for (auto& method_object : methods)
                    {
                        auto& method = method_object.first;
                        createComment(ss, *method);
                        createClassMethodDeclaration(ss, *method);
                    }

                    for (auto& base_method_object : base_methods)
                    {
                        auto& method = base_method_object.first;
                        createComment(ss, *method);
                        createDerivedMethodDeclaration(ss, *method);
                    }

                }
                {
                    AccessCXXSourceScopedStream public_scope{ ss, AccessCXXSourceScopedStream::Specifier::Public };
                    for (auto& property_object : properties)
                    {
                        createPropertyField(ss, *property_object);
                    }

                    for (auto& base_property_object : base_properties)
                    {
                        createPropertyField(ss, *base_property_object);
                    }
                }
            }
        }
    }

    return ss;
}

SourceStream LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createClassCppFileContent(const SymbolClass& object)
{
    SourceStream ss;

    auto& constructors = object.constructors;
    
    auto& methods = object.methods;
    auto base_methods = object.getBaseMethods();
    
    auto& properties = object.properties;
    auto& base_properties = object.getBaseProperties();
    
    {
        {
            auto header_path = createRelativeHeaderPath(object);
            InternalIncludeCXXSourceStream in{ ss, header_path };
        }
        if (_libDirectory.existsExternalTool(NativeExternalLibraryDirectory::ExternalTool::Log))
        {
            auto header_path = _libDirectory.getRelativeHeaderPath(NativeExternalLibraryDirectory::ExternalTool::Log, object);
            InternalIncludeCXXSourceStream in{ ss, header_path };
        }
        if (_libDirectory.existsExternalTool(NativeExternalLibraryDirectory::ExternalTool::MemoryPool))
        {
            auto header_path = _libDirectory.getRelativeHeaderPath(NativeExternalLibraryDirectory::ExternalTool::MemoryPool, object);
            InternalIncludeCXXSourceStream in{ ss, header_path };
        }

        for (auto& constructor_object : constructors)
        {
            auto& constructor = constructor_object.first;
            createConstructorDefinition( ss, object, *constructor );
        }

        createDestructorDefinition(ss, object);

        for (auto& method_object : methods)
        {
            auto& method = method_object.first;
            createClassMethodDefinition(ss, object, *method);
        }

        for (auto& method_object : base_methods)
        {
            auto& method = method_object.first;
            createClassMethodDefinition(ss, object, *method);
        }

        for (auto& property_object : properties)
        {
            createPropertyDefinition(ss, object, *property_object);
        }

        for (auto& property_object : properties)
        {
            createPropertyDefinition(ss, object, *property_object);
        }
    }

    return ss;
}

SourceStream LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createEnumFileContent(const SymbolEnum& object)
{
    SourceStream ss;

    {
        HeaderGuardCXXSourceScopedStream headerguard_scope{ ss, object.parentModules, object.name };

        {
            ExternalIncludeCXXSourceStream ex{ ss, "cstdint" };
        }
        {
            ExternalIncludeCXXSourceStream ex{ ss, "vector" };
        }
        {
            ExternalIncludeCXXSourceStream ex{ ss, "string" };
        }
        {
            ExternalIncludeCXXSourceStream ex{ ss, "memory" };
        }

        {
            NamespaceCXXSourceScopedStream namespace_scope{ ss, object.parentModules };
            createComment(ss, object);
            createEnumDefinition(ss, object);
        }
    }

    return ss;
}

SourceStream LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createMethodHeaderFileContent(const SymbolModule& object)
{
    SourceStream ss;

    {
        HeaderGuardCXXSourceScopedStream headerguard_scope{ ss, object.moduleNames, object.name };

        {
            ExternalIncludeCXXSourceStream ex{ ss, "cstdint" };
        }
        {
            ExternalIncludeCXXSourceStream ex{ ss, "vector" };
        }
        {
            ExternalIncludeCXXSourceStream ex{ ss, "string" };
        }
        {
            ExternalIncludeCXXSourceStream ex{ ss, "memory" };
        }

        createForwardDeclaration(ss, object);

        {
            NamespaceCXXSourceScopedStream namespace_scope{ ss, object.moduleNames };
            for (auto& method_object : object.global_methods)
            {
                auto& method = method_object.first;
                createComment(ss, object);
                createStaticMethodDeclaration(ss, *method);
            }
        }
    }

    return ss;
}

SourceStream LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createMethodCppFileContent(const SymbolModule& object)
{
    SourceStream ss;

    {
        {
            auto header_path = createRelativeHeaderPath(object);
            InternalIncludeCXXSourceStream in{ ss, header_path };
        }
        if (_libDirectory.existsExternalTool(NativeExternalLibraryDirectory::ExternalTool::Log))
        {
            auto header_path = _libDirectory.getRelativeHeaderPath(NativeExternalLibraryDirectory::ExternalTool::Log, object);
            InternalIncludeCXXSourceStream in{ ss, header_path };
        }
        if (_libDirectory.existsExternalTool(NativeExternalLibraryDirectory::ExternalTool::MemoryPool))
        {
            auto header_path = _libDirectory.getRelativeHeaderPath(NativeExternalLibraryDirectory::ExternalTool::MemoryPool, object);
            InternalIncludeCXXSourceStream in{ ss, header_path };
        }

        for (auto& method_object : object.global_methods)
        {
            auto& method = method_object.first;
            createStaticMethodDefinition(ss, *method);
        }

    }

    return ss;
}

SourceStream LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createIncludeFileContent(const SymbolPackage& package, const std::vector<std::string>& include_files)
{
    SourceStream ss;

    {
        HeaderGuardCXXSourceScopedStream headerguard_scope{ ss, {}, package.name };
        createComment(ss, package);

        for (auto& include_file : include_files)
        {
            InternalIncludeCXXSourceStream in{ ss, include_file };
        }
    }

    return ss;
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createForwardDeclaration(SourceStream& ss, const SymbolClass& object)
{
    {
        auto class_objects = object.collectAllClassReference();
        for (auto& w_class_object : class_objects)
        {
            if (auto class_object = w_class_object.lock())
            {
                auto clazz = std::dynamic_pointer_cast<SymbolClass>(class_object);
                NamespaceCXXSourceScopedStream namespace_scope{ ss, clazz->parentModules };
                ClassCXXSourceScopedStream class_scope{ ss, true, clazz->name, {} };
            }
        }
    }

    {
        auto enum_objects = object.collectAllEnumReference();
        for (auto& w_enum_object : enum_objects)
        {
            if (auto enum_object = w_enum_object.lock())
            {
                auto enumm = std::dynamic_pointer_cast<SymbolEnum>(enum_object);
                NamespaceCXXSourceScopedStream namespace_scope(ss, enumm->parentModules);
                EnumCXXSourceScopedStream enum_scope { ss, true, enumm->name };
            }
        }
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createForwardDeclaration(SourceStream& ss, const SymbolModule& object)
{
    {
        auto class_objects = object.collectAllClassReference();
        for (auto& w_class_object : class_objects)
        {
            if (auto class_object = w_class_object.lock())
            {
                auto clazz = std::dynamic_pointer_cast<SymbolClass>(class_object);
                NamespaceCXXSourceScopedStream namespace_scope{ ss, clazz->parentModules };
                ClassCXXSourceScopedStream class_scope{ ss, true, clazz->name, {} };
            }
        }
    }

    {
        auto enum_objects = object.collectAllEnumReference();
        for (auto& w_enum_object : enum_objects)
        {
            if (auto enum_object = w_enum_object.lock())
            {
                auto enumm = std::dynamic_pointer_cast<SymbolEnum>(enum_object);
                NamespaceCXXSourceScopedStream namespace_scope(ss, enumm->parentModules);
                EnumCXXSourceScopedStream enum_scope{ ss, true, enumm->name };
            }
        }
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createEnumDefinition(SourceStream& ss, const SymbolEnum& object)
{
    {
        EnumCXXSourceScopedStream enum_scope{ ss, false, object.name };
        for (auto& value : object.values)
        {
            enum_scope.addElement(value.first, value.second);
        }
    }
}
static MethodCXXSourceScopedStream::Parameter createParameter(const SymbolParameter& parameter)
{
    int io;
    if (parameter.type->isPrimitive())
    {
        io = MethodCXXSourceScopedStream::Parameter::VALUE;
    }
    else
    {
        if (parameter.io == SymbolParameter::IO::OUT)
        {
            io = MethodCXXSourceScopedStream::Parameter::REFERENCE_OUT;
        }
        else
        {
            io = MethodCXXSourceScopedStream::Parameter::REFERENCE_IN;
        }
    }
    return MethodCXXSourceScopedStream::Parameter(io, parameter.type->toCppType(), parameter.name);
}

static std::vector<MethodCXXSourceScopedStream::Parameter> createParameters(const SymbolMethod& object)
{
    std::vector<MethodCXXSourceScopedStream::Parameter> ret;
    for (auto& parameter : object.parameters)
    {
        ret.push_back(createParameter(*parameter));
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createStaticMethodDeclaration(SourceStream& ss, const SymbolMethod& object)
{
    {
        auto parameters = createParameters(object);
        MethodCXXSourceScopedStream method_scope{ ss, true, "", "", object.type->toCppType(), {}, object.name, parameters };        
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createClassMethodDeclaration(SourceStream& ss, const SymbolMethod& object)
{
    {
        auto parameters = createParameters(object);
        MethodCXXSourceScopedStream method_scope{ ss, true, "", "", object.type->toCppType(), {}, object.name, parameters };
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createInterfaceMethodDeclaration(SourceStream& ss, const SymbolMethod& object)
{
    {
        auto parameters = createParameters(object);
        MethodCXXSourceScopedStream method_scope{ ss, true, "virtual", "= 0", object.type->toCppType(), {}, object.name, parameters };
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createDerivedMethodDeclaration(SourceStream& ss, const SymbolMethod& object)
{
    {
        auto parameters = createParameters(object);
        MethodCXXSourceScopedStream method_scope{ ss, true, "", "override", object.type->toCppType(), {}, object.name, parameters };
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createStaticMethodDefinition(SourceStream& ss, const SymbolMethod& object)
{
    {
        auto parameters = createParameters(object);
        auto scopes = createScope(object);
        MethodCXXSourceScopedStream method_scope{ ss, false, "", "", object.type->toCppType(), scopes, object.name, parameters };
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createClassMethodDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object)
{
    {
        auto parameters = createParameters(object);
        auto scopes = createScope(clazz);
        MethodCXXSourceScopedStream method_scope{ ss, false, "", "", object.type->toCppType(), scopes, object.name, parameters };
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createConstructorDeclaration(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object)
{
    {
        auto parameters = createParameters(object);
        MethodCXXSourceScopedStream method_scope{ ss, true, "explicit", "", "", {}, object.name, parameters };
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createConstructorDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolMethod& object)
{
    {
        auto parameters = createParameters(object);
        auto scopes = createScope(clazz);
        MethodCXXSourceScopedStream method_scope{ ss, false, "", "", "", scopes, object.name, parameters };
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createDestructorDeclaration(SourceStream& ss, const SymbolClass& clazz)
{
    {
        std::string name = "~";
        name.append(clazz.name);

        MethodCXXSourceScopedStream method_scope{ ss, true, "", "", "", {}, name, {} };
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createDestructorDefinition(SourceStream& ss, const SymbolClass& clazz)
{
    {
        std::string name = "~";
        name.append(clazz.name);
        auto scopes = createScope(clazz);
        MethodCXXSourceScopedStream method_scope{ ss, true, "", "", "", scopes, name, {} };
    }
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

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createClassPropertySetterDeclaration(SourceStream& ss, const std::string& propertyName, const SymbolProperty& object)
{
    {
        std::string name = "set";
        name += propertyName;

        MethodCXXSourceScopedStream method_scope{ ss, false, "", "", "void", {}, name,
            {
                MethodCXXSourceScopedStream::Parameter(
                    MethodCXXSourceScopedStream::Parameter::REFERENCE_IN,
                    object.type->toCppType(),
                    "value")
            }
        };
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createClassPropertyGetterDeclaration(SourceStream& ss, const std::string& propertyName, const SymbolProperty& object)
{
    {
        std::string name = "get";
        name += propertyName;

        std::string type = object.type->toCppType();
        if (!object.type->isPrimitive())
            type += "&";

        MethodCXXSourceScopedStream method_scope{ ss, false, "", "", type, {}, name,
            {}
        };
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createClassPropertyDeclaration(SourceStream& ss, const SymbolProperty& object)
{
    auto name = createPropertyName(object);

    createClassPropertyGetterDeclaration(ss, name, object);    
    createClassPropertySetterDeclaration(ss, name, object);
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createInterfacePropertySetterDeclaration(SourceStream& ss, const std::string& propertyName, const SymbolProperty& object)
{
    {
        std::string name = "set";
        name += propertyName;

        MethodCXXSourceScopedStream method_scope{ ss, false, "virtual", "= 0", "void", {}, name,
            {
                MethodCXXSourceScopedStream::Parameter(
                    MethodCXXSourceScopedStream::Parameter::REFERENCE_IN,
                    object.type->toCppType(),
                    "value")
            }
        };
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createInterfacePropertyGetterDeclaration(SourceStream& ss, const std::string& propertyName, const SymbolProperty& object)
{
    {
        std::string name = "get";
        name += propertyName;

        std::string type = object.type->toCppType();
        if (!object.type->isPrimitive())
            type += "&";

        MethodCXXSourceScopedStream method_scope{ ss, false, "virtual", "= 0", type, {}, name,
            {}
        };
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createInterfacePropertyDeclaration(SourceStream& ss, const SymbolProperty& object)
{
    auto name = createPropertyName(object);

    createInterfacePropertyGetterDeclaration(ss, name, object);
    createInterfacePropertySetterDeclaration(ss, name, object);
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createDerivedPropertySetterDeclaration(SourceStream& ss, const std::string& propertyName, const SymbolProperty& object)
{
    {
        std::string name = "set";
        name += propertyName;

        MethodCXXSourceScopedStream method_scope{ ss, false, "", "override", "void", {}, name,
            {
                MethodCXXSourceScopedStream::Parameter(
                    MethodCXXSourceScopedStream::Parameter::REFERENCE_IN,
                    object.type->toCppType(),
                    "value")
            }
        };
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createDerivedPropertyGetterDeclaration(SourceStream& ss, const std::string& propertyName, const SymbolProperty& object)
{
    {
        std::string name = "get";
        name += propertyName;

        std::string type = object.type->toCppType();
        if (!object.type->isPrimitive())
            type += "&";

        MethodCXXSourceScopedStream method_scope{ ss, false, "", "override", type, {}, name,
            {}
        };
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createDerivedPropertyDeclaration(SourceStream& ss, const std::string& propertyName, const SymbolProperty& object)
{
    auto name = createPropertyName(object);

    createDerivedPropertyGetterDeclaration(ss, name, object);
    createDerivedPropertySetterDeclaration(ss, name, object);
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createPropertyGetterDefinition(SourceStream& ss, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object)
{
    std::string name = "get";
    name += propertyName;

    std::string type = object.type->toCppType();
    if (!object.type->isPrimitive())
        type += "&";

    auto scopes = createScope(clazz);

    {
        MethodCXXSourceScopedStream method_scope{ ss, true, "", "", type, scopes, name, {} };

        ss << "return _" << propertyName << ";\n";
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createPropertySetterDefinition(SourceStream& ss, const std::string& propertyName, const SymbolClass& clazz, const SymbolProperty& object)
{
    std::string name = "set";
    name += propertyName;

    auto scopes = createScope(clazz);

    {
        MethodCXXSourceScopedStream method_scope{ ss, true, "", "", "void", scopes, name,
            {
                MethodCXXSourceScopedStream::Parameter(
                    MethodCXXSourceScopedStream::Parameter::REFERENCE_IN,
                    object.type->toCppType(),
                    "value")
            }
        };

        ss << "_" << propertyName << " = value;\n";
    }
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createPropertyDefinition(SourceStream& ss, const SymbolClass& clazz, const SymbolProperty& object)
{
    auto name = createPropertyName(object);

    createPropertyGetterDefinition(ss, name, clazz, object);
    createPropertySetterDefinition(ss, name, clazz, object);
}

void LibraryInterfaceGenerator::Implementation::NativeSourceDirectory::createPropertyField(SourceStream& ss, const SymbolProperty& object)
{
    ss << object.type->toCppType() << " " << object.name << ";\n";
}
