#include <dc_env/env.h>
#include <dc_error/error.h>
#include <stddef.h>
#include <stdlib.h>
#include "shell.h"


int main(int argc, char *argv[]) // NOLINT(Wunused-parameter)
{
    dc_env_tracer tracer;
    struct dc_error * err;
    struct dc_env * env;
    int ret_val;

    // Set the tracer to trace through the function calls
//    tracer = dc_env_default_tracer; // Trace through function calls
    tracer = NULL; // Don't trace through function calls

    err = dc_error_create(false); // Create error struct
    env = dc_env_create(err, false, tracer); // Create environment struct

    dc_env_set_tracer(env, tracer); // Set tracer

    // Run Shell
    ret_val = run_shell(env, err);

    // Free memory
    free(err);
    free(env);

    return ret_val;
}

