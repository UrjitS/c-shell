#ifndef OPEN_SHELL_IMPL_H
#define OPEN_SHELL_IMPL_H

#include <dc_env/env.h>
#include <dc_error/error.h>
#include <dc_c/dc_stdlib.h>
#include <dc_c/dc_string.h>
#include <dc_fsm/fsm.h>

enum scuffed_shell_states
{
    INIT_STATE = DC_FSM_USER_START,    // 2
    READ_COMMANDS,
    ERROR,
    RESET_STATE,
    SEPARATE_COMMANDS,
    PARSE_COMMANDS,
    EXECUTE_COMMANDS,
    EXIT,
    DESTROY_STATE,
};

/**
 * Set up the initial state:
 *  - in_redirect_regex   "[ \t\f\v]<.*"
 *  - in_redirect_regex   "[ \t\f\v][1^2]?>[>]?.*"
 *  - in_redirect_regex   "[ \t\f\v]2>[>]?.*"
 *  - path the PATH env var seperated into directories
 *  - prompt the PS1 env var or "$" if PS1 not set
 *  - max_line_length the value of _SC_ARG_MAX (see sysconf)
 *
 * @param env The environment.
 * @param err The error object.
 * @param arg The current struct state
 * @return READ_COMMANDS or INIT_ERROR
 */
int init_state(const struct dc_env *env, __attribute__((unused)) struct dc_error *err, void *arg);

/**
 * Separate the commands and store them into the state object.
 * Currently, only simple commands are supported, so there will only be one command.
 *
 * @param env The environment.
 * @param err The error object.
 * @param arg The current struct state
 * @return PARSE_COMMANDS or ERROR
 */
int separate_commands(const struct dc_env *env, struct dc_error *err, void *arg);

/**
 * Parse the commands to separate the command name, arguments, and I/O redirection.
 *
 * @param env The environment.
 * @param err The error object.
 * @param arg The current struct state
 * @return EXECUTE_COMMANDS or ERROR
 */
int parse_commands(const struct dc_env *env, struct dc_error *err, void *arg);

/**
 * Check if the command is cd, or exit and perform them respectively, otherwise call run on the command.
 *
 * @param env The environment.
 * @param err The error object.
 * @param arg The current struct state
 * @return EXIT or ERROR on failure/exit command, or RESET_STATE.
 */
int execute_commands(const struct dc_env *env, struct dc_error *err, void *arg);

/**
 * Reset the state object for reading a new line.
 *
 * @param env The environment.
 * @param err The error object.
 * @param arg The current struct state.
 * @return READ_COMMANDS.
 */
int reset_state(const struct dc_env *env, struct dc_error *err, void *arg);

/**
 * Exit the shell. This will lead to the termination of the shell.
 *
 * @param env The environment.
 * @param err The error object.
 * @param arg The current struct state
 * @return DESTROY_STATE.
 */
int do_exit(const struct dc_env *env, struct dc_error *err, void *arg);

/**
 * Handle an error.
 *
 * @param env The environment.
 * @param err The error object.
 * @param arg The current struct state.
 * @return if state.fatal_error then DESTROY_STATE otherwise RESET_STATE.
 */
int handle_error(__attribute__((unused)) const struct dc_env *env, struct dc_error *err, void *arg);

/**
 * Reclaim memory from the state object and zero it out (NULL, 0, false). This will terminate the shell.
 *
 * @param env The environment.
 * @param err The error object.
 * @param arg The current struct state.
 * @return DC_FSM_EXIT.
 */
int destroy_state(const struct dc_env *env, struct dc_error *err, void *arg);

#endif
