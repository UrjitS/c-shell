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

char * regex_error(const struct dc_env *env, struct dc_error *err,struct state *state, char * string);
char * regex_in(const struct dc_env *env, struct dc_error *err,struct state *state, char * string);
char * regex_out(const struct dc_env *env, struct dc_error *err,struct state *state, char * string);
void set_command_arguments(const struct dc_env * env, struct dc_error * err, struct state * state, char * string);

#endif //OPEN_COMMAND_H
