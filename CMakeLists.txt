cmake_minimum_required (VERSION 2.8)

# Project: ###################################################################

project (libfatipc)

include_directories (src)

add_library (fatipc
    src/fatipc_buffer.c
    src/fatipc_socket.c
    )

# Tests: #####################################################################

find_package (Threads REQUIRED)

include (ExternalProject)

# Download and install GoogleTest
ExternalProject_Add (
    googletest
    URL https://github.com/google/googletest/archive/release-1.8.0.zip
    INSTALL_COMMAND ""
)
ExternalProject_Get_Property (googletest binary_dir source_dir)
include_directories (${source_dir}/include)
set (GTEST_LIBRARY gtest)
add_library (${GTEST_LIBRARY} UNKNOWN IMPORTED)
set_property (TARGET ${GTEST_LIBRARY} PROPERTY IMPORTED_LOCATION ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a)
set (GTEST_LIBRARY_MAIN gtest_main)
add_library (${GTEST_LIBRARY_MAIN} UNKNOWN IMPORTED)
set_property (TARGET ${GTEST_LIBRARY_MAIN} PROPERTY IMPORTED_LOCATION ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main.a)
add_dependencies (${GTEST_LIBRARY} ${GTEST_LIBRARY_MAIN} googletest)

# Compile tests
add_executable (fatipc_test
    test/fatipc_test.cc
    src/fatipc_buffer.c
    src/fatipc_socket.c
    )
target_link_libraries (fatipc_test ${GTEST_LIBRARY} ${GTEST_LIBRARY_MAIN} ${CMAKE_THREAD_LIBS_INIT})

# Setup test execution
enable_testing ()
add_test (NAME FatIpcTest COMMAND fatipc_test)

