#ifndef OPEN_COMMAND_H
#define OPEN_COMMAND_H

#include "state.h"

/*
 * Parse the command. Take the command->line and use it to fill in all of the fields.
 *
 * @param env The environment
 * @param err The error object
 * @param arg The state object
 * @return EXECUTE_COMMAND state.
 */
int parse_command(const struct dc_env *env, struct dc_error *err, void *arg);

/*
 * Runs the error regex ([ \t\f\v]2>[>]?.*) on the line read from user.
 *
 * @param env The environment
 * @param err The error object
 * @param state The state object
 * @param string The string to run the regex on.
 * @return The altered or same string after running regexc() and expanding.
 */
char * regex_error(const struct dc_env *env, struct dc_error *err, struct state *state, char *string);

/*
 * Runs the in regex ([ \t\f\v]<.*) on the line returned from regex_out.
 *
 * @param env The environment
 * @param err The error object
 * @param state The state object
 * @param string The string to run the regex on.
 * @return The altered or same string after running regexc() and expanding.
 */
char * regex_in(const struct dc_env *env, struct dc_error *err, struct state *state, char *string);

/*
 * Runs the out regex ([ \t\f\v][1^2]?>[>]?.*) on the line returned from regex_error.
 *
 * @param env The environment
 * @param err The error object
 * @param state The state object
 * @param string The string to run the regex on.
 * @return The altered or same string after running regexc() and expanding.
 */
char * regex_out(const struct dc_env *env, struct dc_error *err, struct state *state, char *string);

/*
 * Expands the requested files (i.e. *.txt -> a.txt, b.txt ...).
 * Sets the state->commands argc and argv to the values from wordexp() and sets the state->command->command to the
 * finalized string.
 *
 * @param env The environment
 * @param err The error object
 * @param state The state object
 * @param string The string after all the regex functions have been called.
 */
void set_command_arguments(const struct dc_env *env, struct dc_error *err, struct state *state, char *string);

#endif
