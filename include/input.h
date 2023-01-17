#ifndef OPEN_INPUT_H
#define OPEN_INPUT_H

#include <dc_env/env.h>
#include <dc_error/error.h>

/**
 * Read the command line from the user.
 *
 * @param env The environment.
 * @param err The error object
 * @param arg The state object
 * @return SEPARATE_COMMANDS
 */
int read_commands(const struct dc_env *env, struct dc_error *err, void *arg);

#endif
