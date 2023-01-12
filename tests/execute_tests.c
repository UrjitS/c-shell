#include "tests.h"
#include "util.h"
#include "dc_env/env.h"
#include "dc_error/error.h"

Describe(execute);

BeforeEach(execute)
{}

AfterEach(execute)
{}

Ensure(execute, execute) {

}

TestSuite * execute_tests(void) {
    TestSuite * suite;

    suite = create_test_suite();
    add_test_with_context(suite, execute, execute);

    return suite;
}
