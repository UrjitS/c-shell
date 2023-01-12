#include "tests.h"
#include "util.h"
#include "dc_env/env.h"
#include "dc_error/error.h"

Describe(input);

BeforeEach(input)
{}

AfterEach(input)
{}

Ensure(input, read_commands) {

}

TestSuite * input_tests(void) {
    TestSuite * suite;

    suite = create_test_suite();
    add_test_with_context(suite, input, read_commands);

    return suite;
}
