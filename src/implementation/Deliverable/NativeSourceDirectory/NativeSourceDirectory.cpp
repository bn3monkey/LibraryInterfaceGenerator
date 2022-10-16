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
    // ��� :


    // �ɺ� ���̺� ���
    // 1. Ư�� Ŭ������ ���� �����̽�Ǯ �ҷ�����
    // 2. enum, class, interface�� ����� ������� �ҷ����� <- �ȿ� ����..

    // 1. ���� ��ó��
    // 1.1. �ߺ� ���� ���� Ǯ ����
    // 1.2. ���� ����� ��� Ǯ�� �ֱ�. (�޸�Ǯ ��� ���� ���� ����), �޸�, ��Ʈ�� ������ ����
    // 1.3. ���� �����̽� Ǯ ���� (�ɺ����̺��� ������ �;��ҵ�)

    // 2.Ŭ���� ��ó��
    // 2.1 Ŭ���� �̸� ��������
    // 2.2 ���̽� �������̽� �� Ŭ�������� �ʿ��� �� �ױ�
    // 2.2.1 ���̽� �������̽� �� Ŭ���� ���� ��θ� ���Ǯ�� �߰�
    // 2.2.2 ���̽��� �ִ� ������Ƽ �ҷ��� ���̽� ������Ƽ Ǯ�� �ֱ�
    // 2.2.3 ���̽��� �ִ� �޼ҵ� �ҷ��� ���̽� �޼ҵ� Ǯ�� �ֱ�
    // 2.2.3.1 ���̽��� �ִ� �޼ҵ��� �Ķ����, ������Ƽ����
    //         enum, class, interface Ÿ���� ������ �ش� Ÿ���� ��ġ�� ���Ǯ�� �ֱ�
    // 2.3 ������ �޼ҵ� ������Ƽ �ҷ��� ������Ƽ Ǯ�� �ֱ�
    // 2.3.1 ������Ƽ�� Ÿ���� Ŭ����, �������̽�, enum�̸� ��� Ǯ�� ��ġ �ֱ�
    // 2.4 �޼ҵ� Ǯ�� �����ڰ� ���� ������ �⺻ ������ �ֱ�
    // 2.4.1
    // 2.4 base ������Ƽ Ǯ��  private data �����ؼ� �ױ�
    // 2.5 enum Ǯ�� �ֱ�

    // 3. ����
    // 3.1 �ߺ� ���� ���� ����
    //  3.2 ��� Ǯ �ֱ�
    //   3.3 ���ӽ����̽� ����
    //    3.4 Ŭ���� �̸�, ���̽� Ŭ���� �� �������̽� �ְ� ����
    //     3.5 ������, �Ҹ��� �ֱ�
    //     3.6 ���̽� ������Ƽ �Լ� �ֱ�
    //     3.7 ������Ƽ �ֱ�
    //     3.7 ���̽� �޼ҵ� �ֱ�
    //     3.8 �޼ҵ� �ֱ�
    //     3.9 private�� ������Ƽ ������ �ֱ�
    //    3.4 Ŭ���� �ݱ�
    //   3.3 ���ӽ����̽� �ݱ�
    //  3.2 ��� Ǯ �ݱ�
    // 3.1 �ߺ� ���� ���� �ݱ�

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
