# Copyright (C) 2016 Leandro Lisboa Penz <lpenz@lpenz.org>
# This file is subject to the terms and conditions defined in
# file 'LICENSE', which is part of this source code package.

cmake_minimum_required (VERSION 3.9)

project(libfatipc
  VERSION 0.1
  DESCRIPTION "C library that efficiently passes a big memory chunk between two processes")

include(GNUInstallDirs)

# Main targets

include_directories (src)

add_library (
  fatipc SHARED
  src/fatipc_buffer.c
  src/fatipc_socket.c
  )

set_target_properties(fatipc PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 0
  PUBLIC_HEADER "src/fatipc_buffer.h;src/fatipc_socket.h"
  )

# Install

install (
  TARGETS fatipc
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )
install (
  FILES README.md LICENSE AUTHORS
  DESTINATION ${CMAKE_INSTALL_DOCDIR}/
  )

# cpack deb

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_CONTACT "Leandro Lisboa Penz <lpenz@lpenz.org>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "C library that efficiently passes a big memory chunk between two processes")
set(CPACK_DEBIAN_PACKAGE_SECTION libs)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
execute_process(COMMAND git describe --tags
                OUTPUT_VARIABLE GIT_DESCRIBE)
string(REGEX REPLACE "-g[0-9a-f]+\n$" "" CPACK_PACKAGE_VERSION "${GIT_DESCRIBE}")
set(CPACK_PACKAGE_FILE_NAME "libfatipc-${CPACK_PACKAGE_VERSION}")
include(CPack)

# Tests

find_package (Threads REQUIRED)

include (ExternalProject)

# Download and install GoogleTest
ExternalProject_Add (
  googletest
  URL https://github.com/google/googletest/archive/release-1.8.0.zip
  INSTALL_COMMAND ""
  )
ExternalProject_Get_Property (googletest binary_dir source_dir)
include_directories (${source_dir}/googletest/include)
set (GTEST_LIBRARY gtest)
add_library (${GTEST_LIBRARY} UNKNOWN IMPORTED)
set_property (TARGET ${GTEST_LIBRARY} PROPERTY IMPORTED_LOCATION ${binary_dir}/googlemock/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a)
set (GTEST_LIBRARY_MAIN gtest_main)
add_library (${GTEST_LIBRARY_MAIN} UNKNOWN IMPORTED)
set_property (TARGET ${GTEST_LIBRARY_MAIN} PROPERTY IMPORTED_LOCATION ${binary_dir}/googlemock/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main.a)
add_dependencies (${GTEST_LIBRARY} ${GTEST_LIBRARY_MAIN} googletest)

# Compile tests
add_executable (fatipc_test
  test/fatipc_test.cc
  src/fatipc_buffer.c
  src/fatipc_socket.c
  )
target_link_libraries (
  fatipc_test ${GTEST_LIBRARY} ${GTEST_LIBRARY_MAIN} ${CMAKE_THREAD_LIBS_INIT})

# Setup test execution
enable_testing ()
add_test (NAME FatIpcTest COMMAND fatipc_test)

