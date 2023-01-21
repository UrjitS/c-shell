#ifndef OPEN_EXECUTE_H
#define OPEN_EXECUTE_H

#include <dc_env/env.h>
#include <dc_error/error.h>
#include "command.h"

/**
 * Create a child process, exec the command with any redirection, set the exit code.
 * If there is an error executing the command print an error message.
 * If the command cannot be found set the command->exit_code to 127.
 *
 * @param env The environment
 * @param err The error object
 * @param arg The state object
 */
void execute(const struct dc_env *env, struct dc_error *err, void *arg);

/**
 * Setup any I/O redirections for the process.
 * If there is an error, any open files are closed and function returns.
 *
 * @param env The environment
 * @param err The error object
 * @param arg The state object
*/
void redirect(const struct dc_env *env, struct dc_error *err, void *arg);

/**
  *  Runs a process.
  *
  * @param env The environment.
  * @param err The error object.
  * @param command The command struct.
  * @param path The array of PATH directories to search for the program.
  * @param path_size The size of the PATH array
*/
void run(const struct dc_env *env, struct dc_error *err, struct command * command, char ** path, size_t path_size);

/**
 * Display the error message when a process fails.
 *
 * @param env The environment.
 * @param err The error object.
 * @param arg The state object.
 * @return Error Code.
 */
int handle_run_error(__attribute__((unused)) const struct dc_env *env, struct dc_error *err, void *arg);

#endif
