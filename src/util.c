#include "util.h"

#include "shell_impl.h"
#include <string.h>
#include <stdlib.h>
#include "state.h"
#include <dc_error/error.h>

char * set_prompt(const struct dc_env *env) {
    // Get the PS1 environment variable
    char * prompt = dc_getenv(env, "PS1");

    const char * dollar_prompt = "$ ";
    char * return_prompt = NULL;

    // if PS1 is null then return the dollar prompt
    if (!prompt) {
        return_prompt = strdup(dollar_prompt);
        return return_prompt;
    }

    // Return PS1 value
    return_prompt = strdup(prompt);
    return return_prompt;
}

char ** get_path(const struct dc_env *env)
{
    // Get the path and tokenize it
    char * path = dc_getenv(env, "PATH");
    const char * delim = ":";
    char * tokenized_path = strtok(path, delim);
    unsigned rows = 0;
    char ** array = NULL;

    // loop through tokenized path and assemble an array
    while (tokenized_path) {
        array = realloc(array, (rows + 1) * sizeof(array));
        array[rows] = malloc(strlen(tokenized_path) + 1);
        strcpy(array[rows], tokenized_path);
        rows++;
        tokenized_path = strtok(NULL, delim);
    }

    array = realloc(array, (rows + 1) * sizeof(array));
    array[rows] = NULL;

    return array;
}


void do_reset_state(__attribute__((unused)) const struct dc_env *env, struct dc_error *err, void *arg) {
    // Get the state from arg
    struct state * state = (struct state *) arg;

    state->fatal_error = false;
    state->current_line_length = 0;
    state->current_line = NULL;
    free(state->current_line);

    if (state->command != NULL) {
        for (size_t i = 0; i < state->command->argc; i++) {
            free(state->command->argv[i]);
            state->command->argv[i] = NULL;
        }
        state->command->argc = 0;
        state->command->exit_code = 0;

        state->command->line = NULL;
        state->command->command = NULL;
        state->command->argv = NULL;
        state->command->stdin_file = NULL;
        state->command->stdout_file = NULL;
        state->command->stderr_file = NULL;

        free(state->command->line);
        free(state->command->command);
        free(state->command->stdin_file);
        free(state->command->stdout_file);
        free(state->command->stderr_file);
    }

    state->command = NULL;
    dc_error_reset(err);

}
