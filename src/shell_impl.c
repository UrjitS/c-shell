#include "command.h"
#include "shell_impl.h"
#include <dc_util/filesystem.h>
#include <dc_posix/dc_string.h>
#include "state.h"
#include "execute.h"
#include "builtins.h"
#include "util.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dc_error/error.h>
#include <regex.h>


regex_t in_regex;
regex_t out_regex;
regex_t err_regex;

int init_state(const struct dc_env *env, __attribute__((unused)) struct dc_error *err, void *arg) {
    // Clear the terminal when load
//    system("@cls||clear");

    // Get the state from arg
    struct state * state = (struct state *) arg;

    // Set maximum line length
    state->max_line_length = sysconf(_SC_ARG_MAX);

    // Set all the in_regex values
    regcomp(&in_regex, "[ \\t\\f\\v]<.*", REG_EXTENDED);
    state->in_redirect_regex = &in_regex;

    regcomp(&out_regex, "[ \\t\\f\\v][1^2]?>[>]?.*", REG_EXTENDED);
    state->out_redirect_regex = &out_regex;

    regcomp(&err_regex, "[ \\t\\f\\v]2>[>]?.*", REG_EXTENDED);
    state->err_redirect_regex = &err_regex;

    // Set the path
    state->path = get_path(env);

    // Set the prompt
    state->prompt = set_prompt(env);

    // Set everything else to default values
    state->current_line = NULL;
    state->command = NULL;
    state->current_line_length = 0;
    state->std_in = stdin;
    state->std_out = stdout;

    return READ_COMMANDS;
}


int separate_commands(const struct dc_env *env, struct dc_error *err, void *arg) {
    // Get the state from arg
    struct state * state = (struct state *) arg;

    // Create a new command object and zero it out
    state->command = dc_calloc(env, err, 1, sizeof(*state->command));
    if (dc_error_has_error(err)) {
        state->fatal_error = true;
        return ERROR;
    }

    // Copy the state.current_line to the state.command.line
    state->command->line = dc_strdup(env, err, state->current_line);
    if (dc_error_has_error(err)) {
        state->fatal_error = true;
        return ERROR;
    }

    // Initialize the rest to default values
    state->command->argc = 0;
    state->command->exit_code = 0;

    state->command->argv = NULL;
    state->command->command = NULL;
    state->command->stdin_file = NULL;
    state->command->stdout_file = NULL;
    state->command->stderr_file = NULL;

    state->command->stdout_overwrite = false;
    state->command->stderr_overwrite = false;

    return PARSE_COMMANDS;
}

int parse_commands(const struct dc_env *env, struct dc_error *err, void *arg) {
    // Get the state from arg
    struct state * state = (struct state *) arg;
    // Call parse_command()
    parse_command(env, err, state);

    // if an error return ERROR else cont
    if (dc_error_has_error(err)) {
        return ERROR;
    }

    return EXECUTE_COMMANDS;
}


int execute_commands(const struct dc_env *env, struct dc_error *err, void *arg) {
    // Get the state from arg
    struct state * state = (struct state *) arg;

    if (state->command->command == NULL) {
        printf("Unable To Parse [%s]\n", state->command->line);
        return RESET_STATE;
    }

    if (dc_strcmp(env, state->command->command, "cd") == 0) {
        builtin_cd(env, err, state);
    }
    else if (dc_strcmp(env,state->command->command, "exit") == 0) {
        return EXIT;
    }
    else {
        execute(env, err, state);
        if (dc_error_has_error(err)) {
            state->fatal_error = true;
        }
    }

    fprintf(state->std_out, "Command Exit Code: %d\n", state->command->exit_code);

    if (state->fatal_error == true) {
        return ERROR;
    }

    return RESET_STATE;
}

int do_exit(const struct dc_env *env, struct dc_error *err, void *arg) {
    do_reset_state(env, err, arg);
    return DESTROY_STATE;
}

int reset_state(const struct dc_env *env, struct dc_error *err, void *arg) {
    do_reset_state(env, err, arg);
    return READ_COMMANDS;
}

int handle_error(__attribute__((unused)) const struct dc_env *env, struct dc_error *err, void *arg) {
    // Get the state from arg
    struct state * state = (struct state *) arg;

    if (state->current_line == NULL) {
        fprintf(stderr, "Internal Error %s\n", dc_error_get_message(err));
    } else {
        fprintf(stderr, "Internal Error %s: %s\n", dc_error_get_message(err), state->current_line);
    }

    if (state->fatal_error == true) {
        return DESTROY_STATE;
    }

    return RESET_STATE;
}

int destroy_state(const struct dc_env *env, struct dc_error *err, void *arg) {
    do_reset_state(env, err, arg);
    return DC_FSM_EXIT;
}
