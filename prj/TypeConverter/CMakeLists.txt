cmake_minimum_required (VERSION 3.8)

project ("TypeConverter")

set(repository_dir ${CMAKE_SOURCE_DIR}/../..)
set(src_dir ${repository_dir}/src/library)
set(test_dir ${CMAKE_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

file(GLOB_RECURSE library_files
    ${src_dir}/*.h
    ${src_dir}/*.hpp
    ${src_dir}/*.cpp
)

file(GLOB_RECURSE test_files
  TypeConverterTest.cpp
  KotlinWrapperConverterTest.cpp
  NativeInterfaceConverterTest.cpp)



add_executable(TypeConverterTest
	${library_files}
	${test_files}
	)

target_link_libraries(TypeConverterTest gtest)

#include(GoogleTest)
#gtest_discover_tests(TypeConverterTest)

set_property(TARGET TypeConverterTest PROPERTY CXX_STANDARD 17)
set_property(TARGET TypeConverterTest PROPERTY CXX_STANDARD_REQUIRED ON)

# ABOUT JNI

find_package(JNI REQUIRED)

if (JNI_FOUND)
    include_directories(${JNI_INCLUDE_DIRS})
    message(STATUS "JNI_INCLUDE_DIRS: ${JNI_INCLUDE_DIRS}")
    message(STATUS "JNI_LIBRARIES: ${JNI_LIBRARIES}")

    target_include_directories(TypeConverterTest PUBLIC ${JNI_INCLUDE_DIRS})
    target_link_libraries(TypeConverterTest ${JNI_LIBRARIES})
    target_link_libraries(TypeConverterTest "C:/Program Files/Java/jre-1.8/bin/server/jvm.dll")

else ()
    message(FATAL_ERROR "JNI not found")
endif ()
