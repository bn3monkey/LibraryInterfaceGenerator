# CMakeList.txt: LibraryInterfaceGeneratorWindow에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#
cmake_minimum_required (VERSION 3.8)

project ("LibraryInterfaceGeneratorWindow")

execute_process(COMMAND "python ../../script/makeNativeExternalLibrary.py")
execute_process(COMMAND "python ../../script/makeConverter.py")

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

add_executable(LibraryInerfaceGeneratorTest
	"${implementation_dir}/SourceStream/SourceStream.cpp"
	"${implementation_dir}/SourceStream/CXXSourceStream.cpp"
	${test_files}
	)

target_link_libraries(LibraryInerfaceGeneratorTest gtest)
target_include_directories(LibraryInerfaceGeneratorTest PUBLIC
	"${include_dir}")

include(GoogleTest)
gtest_discover_tests(LibraryInerfaceGeneratorTest)

set_property(TARGET LibraryInerfaceGeneratorTest PROPERTY CXX_STANDARD 17)
set_property(TARGET LibraryInerfaceGeneratorTest PROPERTY CXX_STANDARD_REQUIRED ON)
