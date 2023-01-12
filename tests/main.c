#include <cgreen/cgreen.h>
#include "tests.h"

int main(int argc, char **argv)
{
    TestSuite *suite;
    TestReporter *reporter;
    int suite_result;

    suite = create_test_suite();
    reporter = create_text_reporter();

    add_suite_(suite, "Builtin Tests", builtin_tests());
    add_suite_(suite, "Command Tests", command_tests());
    add_suite_(suite, "Execute Tests", execute_tests());
    add_suite_(suite, "Input Tests", input_tests());
//    add_suite_(suite, "Shell Tests", shell_tests());
    add_suite_(suite, "Shell Impl Tests", shell_impl_tests());
    add_suite_(suite, "Util Tests", util_tests());

    if (argc > 1)
    {
        suite_result = run_single_test(suite, argv[1], reporter);
    } else
    {
        suite_result = run_test_suite(suite, reporter);
    }

    destroy_test_suite(suite);
    destroy_reporter(reporter);

    return suite_result;
}
