import os

def makeConverter() :
    current_file_path = os.path.dirname(os.path.abspath(__file__))
    project_path = os.path.join(current_file_path, "..")
    src_path = os.path.join(project_path, "src")
    implementation_path = os.path.join(src_path, "implementation")
    native_external_library_path = os.path.join(implementation_path, "Converter")

    entries = os.scandir(native_external_library_path)
    
    #for entry in entries :
    #     if entry.is_dir() :
    #        library_name = entry.name
    #        print("Library Path : {0}".format(library_name))
    #return
    

    for entry in entries :
        if entry.is_dir() :
            library_name = entry.name
            library_path = os.path.join(native_external_library_path, library_name)
            print("Library Path : {0}".format(library_path))

            library_h_path = os.path.join(library_path, library_name + ".h")
            with open(library_h_path, 'w') as macro_file :
                macro_file.write("#ifndef __BN3MONKEY_LIBRARYINTERFACEGENERATOR_{0}__\n".format(library_name))
                macro_file.write("#define __BN3MONKEY_LIBRARYINTERFACEGENERATOR_{0}__\n".format(library_name))
                macro_file.write("static constexpr char* {0} = \n".format(library_name))

                library_cpp_path = os.path.join(library_path, library_name + ".cxx")
                with open(library_cpp_path, 'r') as f :
                    while True :
                        line = f.readline()
                        if not line : break
                        replaced = line.replace("\t", "\\t")
                        replaced = replaced.replace("\n", "\\n")
                        replaced = replaced.replace("\"", "\\\"")
                        macro_file.write("\"")
                        macro_file.write(replaced)
                        macro_file.write("\"\n")

                macro_file.write("\"\";\n")
                macro_file.write("#endif // __BN3MONKEY_LIBRARYINTERFACEGENERATOR_{0}__\n".format(library_name))

if __name__ == "__main__":
    makeConverter()