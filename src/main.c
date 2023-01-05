#include <dc_application/command_line.h>
#include <dc_application/config.h>
#include <dc_application/defaults.h>
#include <dc_application/environment.h>
#include <dc_application/options.h>
#include <dc_env/env.h>
#include <dc_error/error.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <dc_c/dc_stdlib.h>
#include <dc_c/dc_string.h>
#include "shell.h"
#include <stddef.h>

struct application_settings
{
    struct dc_opt_settings opts;
    struct dc_setting_string *message;
};


static int run(const struct dc_env *env, struct dc_error *err, struct dc_application_settings *settings);
static struct dc_application_settings *create_settings(const struct dc_env *env, struct dc_error *err);
static int destroy_settings(const struct dc_env *env,
                            struct dc_error *err,
                            struct dc_application_settings **psettings);


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

    // Create application info
    info    = dc_application_info_create(env, err, "ScuffedShell");
    // Run application
    ret_val = dc_application_run(env,
                                 err,
                                 info,
                                 create_settings,
                                 destroy_settings,
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

static struct dc_application_settings *create_settings(const struct dc_env *env, struct dc_error *err)
{
    static bool default_verbose = false;
    struct application_settings *settings;

    DC_TRACE(env);
    settings = dc_malloc(env, err, sizeof(struct application_settings));

    if(settings == NULL)
    {
        return NULL;
    }

    settings->opts.parent.config_path = dc_setting_path_create(env, err);
    settings->message = dc_setting_string_create(env, err);

    struct options opts[] = {
            {(struct dc_setting *)settings->opts.parent.config_path,
                    dc_options_set_path,
                    "config",
                    required_argument,
                    'c',
                    "CONFIG",
                    dc_string_from_string,
                    NULL,
                    dc_string_from_config,
                    NULL},
            {(struct dc_setting *)settings->message,
                    dc_options_set_string,
                    "message",
                    required_argument,
                    'm',
                    "MESSAGE",
                    dc_string_from_string,
                    "message",
                    dc_string_from_config,
                    "Hello, Default World!"},
    };

    // note the trick here - we use calloc and add 1 to ensure the last line is all 0/NULL
    settings->opts.opts_count = (sizeof(opts) / sizeof(struct options)) + 1;
    settings->opts.opts_size = sizeof(struct options);
    settings->opts.opts = dc_calloc(env, err, settings->opts.opts_count, settings->opts.opts_size);
    dc_memcpy(env, settings->opts.opts, opts, sizeof(opts));
    settings->opts.flags = "m:";
    settings->opts.env_prefix = "DC_EXAMPLE_";

    return (struct dc_application_settings *)settings;
}


static int destroy_settings(const struct dc_env *env,
                            struct dc_error *err,
                            struct dc_application_settings **psettings)
{
    struct application_settings *app_settings;

    DC_TRACE(env);
    app_settings = (struct application_settings *)*psettings;
    dc_setting_string_destroy(env, &app_settings->message);
    dc_free(env, app_settings->opts.opts);
    dc_free(env, *psettings);
//
//    if(env->null_free)
//    {
//        *psettings = NULL;
//    }

    return 0;
}
