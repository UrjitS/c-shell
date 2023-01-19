#ifndef OPEN_UTIL_H
#define OPEN_UTIL_H

#include <dc_env/env.h>
#include "state.h"

/**
 * Get and Set the prompt to use.
 *
 * @param env The environment.
 * @return PS1 env var or "$" if PS1 not set.
 */
char * set_prompt(const struct dc_env *env);

/**
 * Get the PATH env var.
 *
 * @param env The environment.
 * @param err The error object.
 * @param arg The state object.
 * @return The PATH env var.
 */
char ** get_path(const struct dc_env *env, struct dc_error *err, struct state * state);

/**
 * Concatenate two strings and get the result.
 * @param env The environment.
 * @param err The error object.
 * @param string_one The first string to concatenate.
 * @param string_two The second string to concatenate.
 * @return A new string with the two strings concatenated together.
 */
char * string_cat(const struct dc_env *env, struct dc_error *err, const char * string_one, const char * string_two);

/**
 * Resets the state for the next read, freeing any dynamic memory.
 *
 * @param env The environment.
 * @param err The error object.
 * @param arg Arguments (State).
 */
void do_reset_state(__attribute__((unused)) const struct dc_env *env, struct dc_error *err, void *arg);


#endif
