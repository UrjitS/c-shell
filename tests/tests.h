#ifndef OPEN_TESTS_H
#define OPEN_TESTS_H

#include <cgreen/cgreen.h>

TestSuite * builtin_tests(void);
TestSuite * command_tests(void);
TestSuite * execute_tests(void);
TestSuite * input_tests(void);
TestSuite * shell_tests(void);
TestSuite * shell_impl_tests(void);
TestSuite * util_tests(void);

#endif //OPEN_TESTS_H
