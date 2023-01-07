#include <dc_application/command_line.h>
#include <dc_application/config.h>
#include <dc_application/options.h>
#include <dc_env/env.h>
#include <dc_error/error.h>
#include <getopt.h>
#include <stdlib.h>
#include <dc_c/dc_stdlib.h>
#include <dc_c/dc_string.h>
#include <stddef.h>
#include "shell.h"


int main(int argc, char *argv[])
{
    dc_env_tracer tracer;
    struct dc_error * err;
    struct dc_env * env;
    struct dc_application_info *info;
    int ret_val;

    // Set the tracer to trace through the function calls
//    tracer = dc_env_default_tracer; // Trace through function calls
    tracer = NULL; // Don't trace through function calls

    err = dc_error_create(false); // Create error struct
    env = dc_env_create(err, false, tracer); // Create environment struct

    dc_error_init(err, false); // Initialize error struct
    dc_env_set_tracer(env, tracer); // Set tracer

    ret_val = run_shell(env, err);

    return ret_val;
}

