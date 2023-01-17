#ifndef OPEN_BUILTINS_H
#define OPEN_BUILTINS_H


#include <dc_env/env.h>
#include <dc_error/error.h>

/**
 * Change the working directory.
 *  ~ is converted to the users home directory.
 *  - no arguments is converted to the users home directory.
 *  The command->exit_code is set to 0 on success or err->errno_code on failure.
 *
 * @param env The environment
 * @param err The error object
 * @param arg The state object
 */
void builtin_cd(const struct dc_env *env, struct dc_error *err, void *arg);

#endif
