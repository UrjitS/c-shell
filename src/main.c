#include <dc_application/command_line.h>
#include <dc_application/config.h>
#include <dc_application/defaults.h>
#include <dc_application/environment.h>
#include <dc_application/options.h>
#include <dc_posix/dc_stdlib.h>
#include <dc_posix/dc_string.h>
#include <dc_env/env.h>
#include <dc_error/error.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "shell.h"
#include <dc_c/dc_stdlib.h>
#include <dc_c/dc_string.h>

struct application_settings
{
    struct dc_opt_settings  opts;
    struct dc_setting_bool *verbose;
};


static int run(const struct dc_env *env, struct dc_error *err, struct dc_application_settings *settings);
//static struct dc_application_settings * create_settings(const struct dc_posix_env *env, struct dc_error *err);
//static int destroy_settings(const struct dc_posix_env *env, struct dc_error *err, struct dc_application_settings **psettings);



int main(int argc, char *argv[])
{
    dc_env_tracer tracer;
    struct dc_error * err;
    struct dc_env * env;
    struct dc_application_info *info;
    int ret_val;

    // Set the tracer to trace through the function calls
    //    tracer = trace_reporter; // Trace through function calls
    tracer = NULL; // Don't trace through function calls

    err = dc_error_create(false); // Create error struct
    env = dc_env_create(err, false, tracer); // Create environment struct

    dc_error_init(err, false); // Initialize error struct
    dc_env_set_tracer(env, tracer); // Set tracer

//    info = dc_application_info_create(env, err, "Hello World Application"); // Create info for this application
//    // Run the application
//    ret_val = dc_application_run(env, err, info, NULL, NULL, run, dc_default_create_lifecycle, dc_default_destroy_lifecycle, NULL, argc, argv);
    info    = dc_application_info_create(env, err, "ScuffedShell");
    ret_val = dc_application_run(env,
                                 err,
                                 info,
                                 NULL,
                                 NULL,
                                 run,
                                 dc_default_create_lifecycle,
                                 dc_default_destroy_lifecycle,
                                 "~/.ScuffedShell.conf",
                                 argc,
                                 argv);
    // Cleanup program
    dc_application_info_destroy(env, &info);
    dc_error_reset(err);

    return ret_val;
}

static int run(const struct dc_env *env, struct dc_error *err, struct dc_application_settings *settings)
{
    DC_TRACE(env);

    run_shell();

    return EXIT_SUCCESS;
}

