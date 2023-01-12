#ifndef OPEN_COMMAND_H
#define OPEN_COMMAND_H

#include "state.h"
#include <stdlib.h>
#include <stdbool.h>

/**
 * Stores all the information needed to perform a command
 * (i.e. command, arguments for that command, in/out files ...)
 */
struct command
{
    /**
     * Command line for this command
     */
    char * line;
    /**
     * The command (e.g. ls, exit, cd, cat)
     */
    char * command;
    /**
     * The number of arguments passed to the command
     */
    size_t argc;
    /**
     *  The arguments passed to the command
     */
    char ** argv;
    /**
     * The file to redirect stdin from or NULL
     */
    char * stdin_file;
    /**
     * The file to redirect stdout from or NULL
     */
    char * stdout_file;
    /**
     * Append to or truncate the stdout file
     */
    bool stdout_overwrite;
    /**
     * The file to redirect stderr to, or NULL
     */
    char * stderr_file;
    /**
     * Append to or truncate the stderr file
     */
    bool stderr_overwrite;
    /**
     *  The status returned from the command
     */
    int exit_code;
};

/**
 * Parse the command. Take the command->line and use it to fill in all of the fields.
 *
 * @param env The environment
 * @param err The error object
 * @param arg The state object
 * @return EXECUTE_COMMAND state.
 */
int parse_command(const struct dc_env *env, struct dc_error *err, void *arg);

/**
 * Runs the error regex ([ \t\f\v]2>[>]?.*) on the line read from user.
 *
 * @param env The environment
 * @param err The error object
 * @param state The state object
 * @param string The string to run the regex on.
 * @return The altered or same string after running regexc() and expanding.
 */
char * regex_error(const struct dc_env *env, struct dc_error *err,struct state *state, char * string);

/**
 * Runs the in regex ([ \t\f\v]<.*) on the line returned from regex_out.
 *
 * @param env The environment
 * @param err The error object
 * @param state The state object
 * @param string The string to run the regex on.
 * @return The altered or same string after running regexc() and expanding.
 */
char * regex_in(const struct dc_env *env, struct dc_error *err,struct state *state, char * string);

/**
 * Runs the out regex ([ \t\f\v][1^2]?>[>]?.*) on the line returned from regex_error.
 *
 * @param env The environment
 * @param err The error object
 * @param state The state object
 * @param string The string to run the regex on.
 * @return The altered or same string after running regexc() and expanding.
 */
char * regex_out(const struct dc_env *env, struct dc_error *err,struct state *state, char * string);

/**
 * Expands the requested files (i.e. *.txt -> a.txt, b.txt ...).
 * Sets the state->commands argc and argv to the values from wordexp() and sets the state->command->command to the
 * finalized string.
 *
 * @param env The environment
 * @param err The error object
 * @param state The state object
 * @param string The string after all the regex functions have been called.
 */
void set_command_arguments(const struct dc_env * env, struct dc_error * err, struct state * state, char * string);

#endif //OPEN_COMMAND_H
