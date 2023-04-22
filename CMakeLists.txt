cmake_minimum_required(VERSION 3.22)

project(dc-shell
        VERSION 0.2.1
        DESCRIPTION ""
        LANGUAGES C)

set(CMAKE_C_STANDARD 17)

if (DEFINED ENV{DC_BUILD_SANITIZE})
    set(SANITIZE $ENV{DC_BUILD_SANITIZE})
else ()
    set(SANITIZE FALSE)
endif ()

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

set(SOURCE_LIST
        ${SOURCE_DIR}/builtins.c
        ${SOURCE_DIR}/command.c
        ${SOURCE_DIR}/execute.c
        ${SOURCE_DIR}/input.c
        ${SOURCE_DIR}/shell.c
        ${SOURCE_DIR}/shell_impl.c
        ${SOURCE_DIR}/util.c
        )
SET(SOURCE_MAIN ${SOURCE_DIR}/main.c
        )
set(HEADER_LIST
        ${INCLUDE_DIR}/builtins.h
        ${INCLUDE_DIR}/command.h
        ${INCLUDE_DIR}/execute.h
        ${INCLUDE_DIR}/input.h
        ${INCLUDE_DIR}/shell.h
        ${INCLUDE_DIR}/shell_impl.h
        ${INCLUDE_DIR}/state.h
        ${INCLUDE_DIR}/util.h
        )

add_compile_definitions(_POSIX_C_SOURCE=200809L)
add_compile_definitions(_XOPEN_SOURCE=700)
add_compile_definitions(_GNU_SOURCE)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_compile_definitions(_DARWIN_C_SOURCE)
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    add_compile_definitions(__BSD_VISIBLE)
endif ()

include_directories(${INCLUDE_DIR})
include(CheckCCompilerFlag)

function(AddCompileOptions)
    foreach(FLAG IN LISTS ARGN)
        string(REPLACE "-" "" FLAG_NO_HYPHEN ${FLAG})
        # set(CMAKE_C_FLAGS "-Werror -Wunknown-warning-option")
        check_c_compiler_flag(${FLAG} COMPILER_SUPPORTS_${FLAG_NO_HYPHEN})
        if (COMPILER_SUPPORTS_${FLAG_NO_HYPHEN})
            add_compile_options(${FLAG})
        endif ()
    endforeach()
endfunction()

list(APPEND COMPILER_FLAGS
        "-Wall"
        "-Wextra"
        "-Wpedantic"
        "-Wshadow"
        "-Wstrict-overflow=4"
        "-Wswitch-default"
        "-Wswitch-enum"
        "-Wunused"
        "-Wunused-macros"
        "-Wdate-time"
        "-Winvalid-pch"
        "-Wmissing-declarations"
        "-Wmissing-include-dirs"
        "-Wmissing-prototypes"
        "-Wstrict-prototypes"
        "-Wundef"
        "-Wnull-dereference"
        "-Wstack-protector"
        "-Wdouble-promotion"
        "-Wvla"
        "-Walloca"
        "-Woverlength-strings"
        "-Wdisabled-optimization"
        "-Winline"
        "-Wcast-qual"
        "-Wfloat-equal"
        "-Wformat=2"
        "-Wfree-nonheap-object"
        "-Wshift-overflow"
        "-Wwrite-strings"
        "-Wunsuffixed-float-constants"
        "-Warith-conversion"
        "-Wunsafe-loop-optimizations"
        "-Wvector-operation-performance"
        "-Walloc-zero"
        "-Wtrampolines"
        "-Wtsan"
        "-Wformat-overflow=2"
        "-Wformat-signedness"
        "-Wjump-misses-init"
        "-Wformat-truncation=2")

if (${SANITIZE})
    list(APPEND COMPILER_FLAGS
            "-fsanitize=pointer-compare"
            "-fsanitize=pointer-subtract"
            "-fsanitize=undefined"
            "-fsanitize-address-use-after-scope"
            "-fcf-protection=full"
            ##            "-fharden-compares"
            ##            "-fharden-conditional-branches"
            "-fharden-control-flow-redundancy"
            "-fstack-protector=all"
            ##            "-fstack-check"
            ##            "-fstack-clash-protection"
            "-finstrument-functions"
            "-fsanitize=leak"
            "-fno-delete-null-pointer-checks"
            "-fno-omit-frame-pointer"
            )

    add_compile_options("-fsanitize=address")
    add_link_options("-fsanitize=address")
    add_link_options("-fsanitize=bounds")
endif ()

AddCompileOptions(${COMPILER_FLAGS})

find_package(Doxygen
        REQUIRED
        REQUIRED dot
        OPTIONAL_COMPONENTS mscgen dia)

set(DOXYGEN_ALWAYS_DETAILED_SEC YES)
set(DOXYGEN_REPEAT_BRIEF YES)
set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_WARNINGS YES)
set(DOXYGEN_QUIET YES)

doxygen_add_docs(doxygen
        ${HEADER_LIST}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generating Doxygen documentation for dc-shell")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CLANG_TIDY_CHECKS "*")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-altera-id-dependent-backward-branch")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-altera-struct-pack-align")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-altera-unroll-loops")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-android-cloexec-accept")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-android-cloexec-creat")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-android-cloexec-dup")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-android-cloexec-open")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-android-cloexec-pipe")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-android-cloexec-socket")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-bugprone-easily-swappable-parameters")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-cert-dcl03-c")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-analyzer-deadcode.DeadStores")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-analyzer-security.insecureAPI.strcpy")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-diagnostic-cast-align")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-diagnostic-format-nonliteral")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-diagnostic-incompatible-pointer-types-discards-qualifiers")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-diagnostic-unknown-warning-option")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-diagnostic-unused-but-set-variable")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-diagnostic-unused-function")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-diagnostic-unused-parameter")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-diagnostic-unused-variable")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-diagnostic-vla")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-cppcoreguidelines-init-variables")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-google-readability-todo")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-hicpp-static-assert")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-llvmlibc-restrict-system-libc-headers")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-misc-static-assert")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-misc-unused-parameters")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-readability-identifier-length")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    if (NOT DEFINED ENV{CMDLINE_TOOLS_VER})
        message( FATAL_ERROR "The CMDLINE_TOOLS_VER environment variable must be set" )
    endif ()

    if(NOT EXISTS /Library/Developer/CommandLineTools/usr/lib/clang/$ENV{CMDLINE_TOOLS_VER})
        message( FATAL_ERROR "/Library/Developer/CommandLineTools/usr/lib/clang/$ENV{CMDLINE_TOOLS_VER} does not exist" )
    endif ()

    set(CMAKE_C_CLANG_TIDY clang-tidy -extra-arg=-I/Library/Developer/CommandLineTools/usr/lib/clang/$ENV{CMDLINE_TOOLS_VER}/include -checks=${CLANG_TIDY_CHECKS};--quiet)
else ()
    set(CMAKE_C_CLANG_TIDY clang-tidy -checks=${CLANG_TIDY_CHECKS};--quiet)
endif ()

add_executable(dc-shell ${SOURCE_MAIN} ${SOURCE_LIST} ${HEADER_LIST})
target_include_directories(dc-shell PRIVATE include)
target_include_directories(dc-shell PRIVATE /usr/local/include)
target_link_directories(dc-shell PRIVATE /usr/local/lib)

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_include_directories(dc-shell PRIVATE /usr/include)
endif ()

set_target_properties(dc-shell PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR})

find_library(LIBDC_ERROR dc_error REQUIRED)
find_library(LIBDC_ENV dc_env REQUIRED)
find_library(LIBDC_C dc_c REQUIRED)
find_library(LIBDC_POSIX dc_posix REQUIRED)
find_library(LIBDC_UNIX dc_unix REQUIRED)
find_library(LIBDC_UTIL dc_util REQUIRED)
find_library(LIBDC_FSM dc_fsm REQUIRED)
find_library(LIB_CONFIG config REQUIRED)
find_library(LIBDC_APPLICATION dc_application REQUIRED)

target_link_libraries(dc-shell PUBLIC ${LIBDC_ERROR})
target_link_libraries(dc-shell PUBLIC ${LIBDC_ENV})
target_link_libraries(dc-shell PUBLIC ${LIBDC_C})
target_link_libraries(dc-shell PUBLIC ${LIBDC_POSIX})
target_link_libraries(dc-shell PUBLIC ${LIBDC_UNIX})
target_link_libraries(dc-shell PUBLIC ${LIBDC_UTIL})
target_link_libraries(dc-shell PUBLIC ${LIBDC_FSM})
target_link_libraries(dc-shell PUBLIC ${LIB_CONFIG})
target_link_libraries(dc-shell PUBLIC ${LIBDC_APPLICATION})

set_target_properties(dc-shell PROPERTIES OUTPUT_NAME "ScuffedShell")
install(TARGETS dc-shell DESTINATION bin)

add_dependencies(dc-shell doxygen)

find_library(LIBCGREEN cgreen REQUIRED)
#add_subdirectory(tests)