set(TEST_HEADER_LIST
        tests.h
        )

set(TEST_SOURCE_LIST
        builtin_tests.c
        command_tests.c
        execute_tests.c
        input_tests.c
        main.c
        shell_impl_tests.c
        shell_tests.c
        util_tests.c
        )

include_directories(${CGREEN_PUBLIC_INCLUDE_DIRS} ${PROJECT_BINARY_DIR})
add_executable(libdc_shell_test ${TEST_SOURCE_LIST} ${TEST_HEADER_LIST} ${SOURCE_LIST} ${HEADER_LIST})

target_compile_features(libdc_shell_test PRIVATE c_std_17)

target_include_directories(libdc_shell_test PRIVATE ../include)
target_include_directories(libdc_shell_test PRIVATE /usr/local/include)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_include_directories(libdc_shell_test PRIVATE /opt/homebrew/include)
else ()
    target_include_directories(libdc_shell_test PRIVATE /usr/include)
endif ()

find_library(LIBCGREEN cgreen REQUIRED)
find_library(LIBDC_ERROR dc_error REQUIRED)
find_library(LIBDC_ENV dc_env REQUIRED)
find_library(LIBDC_C dc_c REQUIRED)
find_library(LIBDC_POSIX dc_posix REQUIRED)
find_library(LIBDC_UNIX dc_unix REQUIRED)
find_library(LIBDC_UTIL dc_util REQUIRED)
find_library(LIBDC_FSM dc_fsm REQUIRED)
find_library(LIB_CONFIG config REQUIRED)
find_library(LIBDC_APPLICATION dc_application REQUIRED)

target_link_libraries(libdc_shell_test PRIVATE ${LIBCGREEN})
target_link_libraries(libdc_shell_test PUBLIC ${LIBDC_ERROR})
target_link_libraries(libdc_shell_test PUBLIC ${LIBDC_ENV})
target_link_libraries(libdc_shell_test PUBLIC ${LIBDC_C})
target_link_libraries(libdc_shell_test PUBLIC ${LIBDC_POSIX})
target_link_libraries(libdc_shell_test PUBLIC ${LIBDC_UNIX})
target_link_libraries(libdc_shell_test PUBLIC ${LIBDC_UTIL})
target_link_libraries(libdc_shell_test PUBLIC ${LIBDC_FSM})
target_link_libraries(libdc_shell_test PUBLIC ${LIB_CONFIG})
target_link_libraries(libdc_shell_test PUBLIC ${LIBDC_APPLICATION})

add_test(NAME libdc_shell_test COMMAND libdc_shell_test)