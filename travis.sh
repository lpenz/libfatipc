#!/bin/bash

function build() {
    make VERBOSE=1
    make test
}

set -e -x -o pipefail

case "$ACTION" in
    doxygen)
        doxygen > /dev/null
        linkchecker --check-extern ./html/index.html
        ;;
    cppcheck)
        cppcheck --version
        cppcheck --enable=warning,style,performance --error-exitcode=1 ./**/*.[ch]
        ;;
    coverage)
        : double up as gcc test
        export CFLAGS="-W -Wall -Werror -Wextra -fprofile-arcs -ftest-coverage"
        export CXXFLAGS="-W -Wall -Werror -Wextra -fprofile-arcs -ftest-coverage"
        cmake .
        build
        ;;
    valgrind)
        cmake .
        make
        valgrind --trace-children=yes --leak-check=full ./fatipc_test
        ;;
    clang)
        export CFLAGS="-W -Wall -Werror -Wextra"
        export CXXFLAGS="-W -Wall -Werror -Wextra -Wno-missing-field-initializers"
        : gtest requires -Wno-missing-field-initializers
        export CC=/usr/bin/clang
        export CXX=/usr/bin/clang++
        cmake .
        build
        ;;
    clang-analyse)
        scan-build -plist -o _clang-analyse cmake .
        scan-build -plist -o _clang-analyse make VERBOSE=1
        if grep '>issue_context<' _clang-analyse/*/*; then
            exit 1
        fi
        ;;
    *)
        echo "Invalid action [$ACTION]"
        exit 1
        ;;
esac

