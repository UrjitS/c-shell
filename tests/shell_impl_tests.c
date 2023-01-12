#include "tests.h"
#include "util.h"
#include "dc_env/env.h"
#include "dc_error/error.h"

Describe(shell_impl);

BeforeEach(shell_impl)
{}

AfterEach(shell_impl)
{}

Ensure(shell_impl, init_state) {

}

TestSuite * shell_impl_tests(void) {
    TestSuite * suite;

    suite = create_test_suite();
    add_test_with_context(suite, shell_impl, init_state);

    return suite;
}
