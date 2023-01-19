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

char ** get_path(const struct dc_env *env, struct dc_error *err, struct state * state)
{
    // Get the path and tokenize it
    char * path = dc_getenv(env, "PATH");
    const char * delim = ":";
    char * tokenized_path = strtok(path, delim);

    size_t row_index = 0;
    char ** array = NULL;

    // loop through tokenized path and assemble an array
    while ((tokenized_path = strtok(NULL, delim))) {
        // Re size path array
        array = dc_realloc(env, err, array, (row_index + 1) * sizeof(array));
        if (dc_error_has_error(err)) {
            state->fatal_error = true;
            return NULL;
        }
        // Malloc
        array[row_index] = dc_malloc(env, err, strlen(tokenized_path) + 1);
        if (dc_error_has_error(err)) {
            state->fatal_error = true;
            return NULL;
        }
        dc_strcpy(env, array[row_index], tokenized_path);
        row_index++;
    }

    return array;
}

char * string_cat(const struct dc_env *env, struct dc_error *err, const char * string_one, const char * string_two)
{
    char * result_string = NULL;
    size_t string_one_length, string_two_length;

    if(string_one && string_two)
    {
        // Get the strings length
        string_one_length = strlen(string_one);
        string_two_length = strlen(string_two);
        // Set the result string to the new size
        result_string = dc_malloc(env, err, (string_one_length + string_two_length + 1));

        dc_memcpy(env, result_string, string_one, string_one_length);
        dc_memcpy(env, result_string + string_one_length, string_two, string_two_length);
        // Set last character to term byte
        result_string[(string_one_length + string_two_length)] = '\0';
    }

    return result_string;
}

void do_reset_state(__attribute__((unused)) const struct dc_env *env, struct dc_error *err, void *arg) {
    // Get the state from arg
    struct state * state = (struct state *) arg;

    state->fatal_error = false;
    state->current_line_length = 0;

    if (state->command != NULL) {
        for (size_t i = 0; i < state->command->argc; i++) {
            free(state->command->argv[i]);
        }
        free(state->command->command);
        free(state->command->stdin_file);
        free(state->command->stdout_file);
        free(state->command->stderr_file);

        state->command->argc = 0;
        state->command->exit_code = 0;

        state->command->line = NULL;
        state->command->command = NULL;
        state->command->argv = NULL;
        state->command->stdin_file = NULL;
        state->command->stdout_file = NULL;
        state->command->stderr_file = NULL;

    }
    free(state->command);
    state->command = NULL;
    dc_error_reset(err);
}
