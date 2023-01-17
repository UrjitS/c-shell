#ifndef OPEN_SHELL_H
#define OPEN_SHELL_H

#include <dc_env/env.h>
#include <dc_error/error.h>

/**
 * Run the shell FSM.
 *
 * @param env The environment
 * @param err The error object
 * @return The exit code from the shell
 */
int run_shell(const struct dc_env *env, struct dc_error *err);

#endif
