#include "tests.h"
#include "util.h"
#include "dc_env/env.h"
#include "dc_error/error.h"

Describe(command);

BeforeEach(command)
{}

AfterEach(command)
{}

Ensure(command, parse_command) {

}

TestSuite * command_tests(void) {
    TestSuite * suite;

    suite = create_test_suite();
    add_test_with_context(suite, command, parse_command);

    return suite;
}

