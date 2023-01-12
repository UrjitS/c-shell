#include "tests.h"
#include "util.h"
#include <dc_posix/dc_stdlib.h>
#include <dc_c/dc_stdlib.h>
#include "dc_env/env.h"
#include "dc_error/error.h"

Describe(util);

static dc_env_tracer tracer;
static struct dc_env *env;
static struct dc_error * err;

BeforeEach(util)
{
    tracer = NULL; // Don't trace through function calls

    err = dc_error_create(false); // Create error struct
    env = dc_env_create(err, false, tracer); // Create environment struct

    dc_error_init(err, false); // Initialize error struct
    dc_env_set_tracer(env, tracer); // Set tracer

}

AfterEach(util)
{
    free(env);
    free(err);
}

Ensure(util, get_prompt) {
    char * prompt;

    prompt = dc_getenv(env, "PS1");

    if (prompt != NULL) {
        dc_setenv(env, err, "PS1", NULL, true);
    }

    prompt = set_prompt(env);
    assert_that(prompt, is_equal_to_string("$ "));

    dc_setenv(env, err, "PS1", "ABC", true);
    prompt = set_prompt(env);
    assert_that(prompt, is_equal_to_string("ABC"));
}

Ensure(util, get_path) {
    static const char *paths[] = {
            "",
            ".",
            "abc",
            "abc;def",
            "/usr/bin:.",
            ".:/usr/bin",
            ":",
            "/usr/bin:/bin:/usr/local/bin",
            NULL,
    };
    char * path;


    for (int i = 0; paths[i]; ++i)
    {
        dc_setenv(env, err, "PATH", paths[i], true);
        path = dc_getenv(env, "PATH");
        assert_that(path, is_equal_to_string(paths[i]));
    }

}

Ensure(util, do_reset_state) {


}

TestSuite * util_tests(void) {
    TestSuite * suite;

    suite = create_test_suite();
    add_test_with_context(suite, util, get_prompt);
    add_test_with_context(suite, util, get_path);
    add_test_with_context(suite, util, do_reset_state);

    return suite;
}
