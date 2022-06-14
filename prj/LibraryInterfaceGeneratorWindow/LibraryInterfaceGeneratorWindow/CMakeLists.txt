# CMakeList.txt: LibraryInterfaceGeneratorWindow에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#
cmake_minimum_required (VERSION 3.8)

project ("LibraryInterfaceGeneratorWindow")

set(repository_dir ${CMAKE_SOURCE_DIR}/../../..)

message("repository_dir = ${repository_dir}")
set(src_dir ${repository_dir}/src)
set(include_dir ${src_dir}/include)
message("include_dirs = ${include_dir}")
set(implementation_dir ${src_dir}/implementation)
message("implementation_dirs = ${implementation_dir}")

file(GLOB_RECURSE include_files 
	LIST_DIRECTORIES true
	"${include_dir}/*.h"
	"${include_dir}/*.hpp")

message("include_files = ${include_files}")

file(GLOB_RECURSE implementation_files
	${implementation_dir}/*.h
	${implementation_dir}/*.hpp
	${implementation_dir}/*.cpp
	${implementation_dir}/*.c)

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
