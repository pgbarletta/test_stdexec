cmake_minimum_required(VERSION 3.24)
project(paralelo)

include(CMakePrintHelpers)
include(cmake/CPM.cmake)
include(cmake/colors.cmake)
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(paralelo
        src/main.cpp)

CPMAddPackage(NAME stdexec GITHUB_REPOSITORY NVIDIA/stdexec GIT_TAG main)
target_link_libraries(paralelo PRIVATE STDEXEC::stdexec)
#target_compile_features(STDEXEC::stdexec PRIVATE cxx_std_20)

CPMAddPackage(NAME fmt GITHUB_REPOSITORY fmtlib/fmt GIT_TAG master)
target_link_libraries(paralelo PRIVATE fmt)
#target_compile_features(fmt PRIVATE cxx_std_20)

CPMAddPackage(NAME chemfiles GITHUB_REPOSITORY chemfiles/chemfiles GIT_TAG master)
target_link_libraries(paralelo PRIVATE chemfiles)
#target_compile_features(chemfiles PRIVATE cxx_std_17)

target_compile_features(paralelo PRIVATE cxx_std_20)
set_target_properties(paralelo PROPERTIES CXX_EXTENSIONS OFF)
if (CMAKE_CXX_COMPILER_ID MATCHES "NVHPC")
    message(STATUS "${Green}Using NVHPC compiler${ColourReset}")
    list(APPEND CXX_FLAGS "--experimental-stdpar")
    target_compile_options(paralelo PRIVATE ${CXX_FLAGS})
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    find_package(Threads REQUIRED)
    target_link_libraries(paralelo PRIVATE Threads::Threads)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    find_package(Threads REQUIRED)
    target_link_libraries(paralelo PRIVATE Threads::Threads)
else ()
    message(FATAL_ERROR "${Red}No supporting compiler found.${ColourReset}")
endif ()





