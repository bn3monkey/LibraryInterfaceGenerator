# CMakeList.txt: LibraryInterfaceGeneratorWindow에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#
cmake_minimum_required (VERSION 3.8)

project ("LibraryInterfaceGeneratorWindow")

# execute_process(COMMAND "python ../../script/makeNativeExternalLibrary.py")
# execute_process(COMMAND "python ../../script/makeConverter.py")

set(repository_dir ${CMAKE_SOURCE_DIR}/../../..)

message("repository_dir = ${repository_dir}")
set(src_dir ${repository_dir}/src)
set(include_dir ${src_dir}/include)
message("include_dirs = ${include_dir}")
set(implementation_dir ${src_dir}/implementation)
message("implementation_dirs = ${implementation_dir}")

set(test_dir "${repository_dir}/test")
message("test_dir = ${test_dir}")
set(test_source_dir "${test_dir}/test")
message("test_source_dir = ${test_source_dir}")
set(gtest_dir "${test_dir}/gtest")
message("gtest_dir = ${gtest_dir}")

include(FetchContent)
FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

file(GLOB_RECURSE include_files 
	LIST_DIRECTORIES true
	"${include_dir}/*.h"
	"${include_dir}/*.hpp")

message("include_files = ${include_files}")


macro(importExternalLibrary library_path)
	get_filename_component(library_filename "${library_path}" NAME)
	message("library file name : ${library_filename}")
	get_filename_component(library_dirpath "${library_path}" DIRECTORY)
	message("library directory path : ${library_dirpath}")
	
	string(TOUPPER "${library_filename}" variable_name)
	string(REPLACE "." "_" variable_name "${variable_name}")
	string(REPLACE "CXX" "CPP" variable_name "${variable_name}")
	string(REPLACE "HXX" "HPP" variable_name "${variable_name}")
	message("variable name : ${variable_name}")

	set(header_guard "__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_${variable_name}__")
	message("header guard : ${header_guard}")

	file(READ "${library_path}" library_content)
	string(REPLACE "\"" "\\\"" processed_library_content "${library_content}")
	string(REPLACE "\n" "\\n\"\n\"" processed_library_content "${processed_library_content}")
	set(processed_library_content "\"${processed_library_content}\\n\"")

	set(output_library_content
		"#if !defined(${header_guard})\n#define ${header_guard}\nstatic constexpr char* ${variable_name} = \n${processed_library_content};\n#endif //${header_guard}\n"
	)

	set(output_library_filename "${library_filename}")
	string(REPLACE "." "_" output_library_filename "${output_library_filename}")
	string(REPLACE "cxx" "cpp" output_library_filename "${output_library_filename}")
	string(REPLACE "hxx" "hpp" output_library_filename "${output_library_filename}")
	set(output_library_filename "${output_library_filename}.h")
	message("output library filename : ${output_library_filename}")

	set(output_library_filepath "${library_dirpath}/${output_library_filename}")
	file(WRITE "${output_library_filepath}" "${output_library_content}")

endmacro()

file(GLOB_RECURSE external_library_files
	${implementation_dir}/*.hxx
	${implementation_dir}/*.cxx
)

foreach(external_library_file ${external_library_files})
	importExternalLibrary(${external_library_file})
endforeach()

file(GLOB_RECURSE implementation_files
	${implementation_dir}/SourceStream/*.h
	${implementation_dir}/SourceStream/*.hpp
	${implementation_dir}/SourceStream/*.cpp
	${implementation_dir}/SourceStream/*.c
	${implementation_dir}/SymbolTable/*.h
	${implementation_dir}/SymbolTable/*.hpp
	${implementation_dir}/SymbolTable/*.cpp
	${implementation_dir}/SymbolTable/*.c
	${implementation_dir}/FileSystem/*.h
	${implementation_dir}/FileSystem/*.hpp
	${implementation_dir}/FileSystem/*.cpp
	${implementation_dir}/FileSystem/*.c
	${implementation_dir}/ExternalLibrary/*.h
	${implementation_dir}/ExternalLibrary/*.hpp
	${implementation_dir}/ExternalLibrary/*.cpp
	${implementation_dir}/ExternalLibrary/*.c

	${implementation_dir}/Deliverable/NativeSourceDirectory/*.h
	${implementation_dir}/Deliverable/NativeSourceDirectory/*.hpp
	${implementation_dir}/Deliverable/NativeSourceDirectory/*.cpp
	${implementation_dir}/Deliverable/NativeSourceDirectory/*.c

	${implementation_dir}/Deliverable/NativeInterface/*.h
	${implementation_dir}/Deliverable/NativeInterface/*.hpp
	${implementation_dir}/Deliverable/NativeInterface/*.cpp
	${implementation_dir}/Deliverable/NativeInterface/*.c

	${implementation_dir}/Deliverable/Wrapper/*.h
	${implementation_dir}/Deliverable/Wrapper/*.hpp
	${implementation_dir}/Deliverable/Wrapper/*.cpp
	${implementation_dir}/Deliverable/Wrapper/*.c

	${implementation_dir}/Deliverable/SourceDirectory/*.h
	${implementation_dir}/Deliverable/SourceDirectory/*.hpp
	${implementation_dir}/Deliverable/SourceDirectory/*.c
	${implementation_dir}/Deliverable/SourceDirectory/*.cpp

	${implementation_dir}/Auxiliary/*.h
	${implementation_dir}/Auxiliary/*.hpp
	${implementation_dir}/Auxiliary/*.cpp
	${implementation_dir}/Auxiliary/*.c


	${implementation_dir}/Converter/*.h
	${implementation_dir}/Converter/*.hpp
	${implementation_dir}/Converter/*.cpp
	${implementation_dir}/Converter/*.c

	${implementation_dir}/Environment.hpp
	${implementation_dir}/LibraryInterfaceGenerator.cpp

)

message("implementation_files =  ${implementation_files}")

include_directories(${include_dir})
include_directories(${implementation_dir})

# 이 프로젝트의 실행 파일에 소스를 추가합니다.

add_executable (LibraryInterfaceGeneratorWindow 
	${include_files}
	${implementation_files}
	"LibraryInterfaceGeneratorWindow.cpp" 
	"LibraryInterfaceGeneratorWindow.h")

target_include_directories(LibraryInterfaceGeneratorWindow PUBLIC "C:/Program Files/Java/jdk-11.0.14/include")
target_include_directories(LibraryInterfaceGeneratorWindow PUBLIC "C:/Program Files/Java/jdk-11.0.14/include/win32")

set_property(TARGET LibraryInterfaceGeneratorWindow PROPERTY CXX_STANDARD 17)
set_property(TARGET LibraryInterfaceGeneratorWindow PROPERTY CXX_STANDARD_REQUIRED ON)
# TODO: 필요한 경우 테스트를 추가하고 대상을 설치합니다.




enable_testing()



file(GLOB_RECURSE test_files
	${test_source_dir}/*.hpp
	${test_source_dir}/*.h
	${test_source_dir}/*.cpp
	${test_source_dir}/*.c
)


# add_executable(LibraryInerfaceGeneratorTest 
#	${include_files}
#	${implementation_files}
#	${test_files}
#	)

file(GLOB_RECURSE source_files_for_test
	${implementation_dir}/SourceStream/*.h
	${implementation_dir}/SourceStream/*.hpp
	${implementation_dir}/SourceStream/*.cpp
	${implementation_dir}/SourceStream/*.c
	${implementation_dir}/SymbolTable/*.h
	${implementation_dir}/SymbolTable/*.hpp
	${implementation_dir}/SymbolTable/*.cpp
	${implementation_dir}/SymbolTable/*.c
	${implementation_dir}/FileSystem/*.h
	${implementation_dir}/FileSystem/*.hpp
	${implementation_dir}/FileSystem/*.cpp
	${implementation_dir}/FileSystem/*.c
	${implementation_dir}/ExternalLibrary/*.h
	${implementation_dir}/ExternalLibrary/*.hpp
	${implementation_dir}/ExternalLibrary/*.cpp
	${implementation_dir}/ExternalLibrary/*.c

	${implementation_dir}/Deliverable/NativeSourceDirectory/*.h
	${implementation_dir}/Deliverable/NativeSourceDirectory/*.hpp
	${implementation_dir}/Deliverable/NativeSourceDirectory/*.cpp
	${implementation_dir}/Deliverable/NativeSourceDirectory/*.c

	${implementation_dir}/Deliverable/NativeInterface/*.h
	${implementation_dir}/Deliverable/NativeInterface/*.hpp
	${implementation_dir}/Deliverable/NativeInterface/*.cpp
	${implementation_dir}/Deliverable/NativeInterface/*.c

	${implementation_dir}/Deliverable/Wrapper/*.h
	${implementation_dir}/Deliverable/Wrapper/*.hpp
	${implementation_dir}/Deliverable/Wrapper/*.cpp
	${implementation_dir}/Deliverable/Wrapper/*.c
	
	${implementation_dir}/Deliverable/SourceDirectory/*.h
	${implementation_dir}/Deliverable/SourceDirectory/*.hpp
	${implementation_dir}/Deliverable/SourceDirectory/*.c
	${implementation_dir}/Deliverable/SourceDirectory/*.cpp

	${implementation_dir}/Auxiliary/*.h
	${implementation_dir}/Auxiliary/*.hpp
	${implementation_dir}/Auxiliary/*.cpp
	${implementation_dir}/Auxiliary/*.c


	${implementation_dir}/Converter/*.h
	${implementation_dir}/Converter/*.hpp
	${implementation_dir}/Converter/*.cpp
	${implementation_dir}/Converter/*.c)

add_executable(LibraryInerfaceGeneratorTest
	${source_files_for_test}

	${test_files}
	)

target_link_libraries(LibraryInerfaceGeneratorTest gtest)
target_include_directories(LibraryInerfaceGeneratorTest PUBLIC
	"${include_dir}")

include(GoogleTest)
gtest_discover_tests(LibraryInerfaceGeneratorTest)

set_property(TARGET LibraryInerfaceGeneratorTest PROPERTY CXX_STANDARD 17)
set_property(TARGET LibraryInerfaceGeneratorTest PROPERTY CXX_STANDARD_REQUIRED ON)
