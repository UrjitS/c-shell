#include <dc_util/filesystem.h>
#include <dc_posix/dc_stdio.h>
#include <dc_posix/dc_string.h>
#include <dc_posix/dc_stdlib.h>
#include <dc_util/strings.h>
#include "shell_impl.h"
#include "state.h"
#include "shell.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

char *set_prompt(const struct dc_env *env);
char **get_path(const struct dc_env *env);

int init_state(const struct dc_env *env, struct dc_error *err, void *arg) {
    regex_t regex;
    // Get the state from arg
    struct state * state = (struct state *) arg;

    // Set maximum line length
    state->max_line_length = sysconf(_SC_ARG_MAX);

    // Set all the regex values
    regcomp(&regex, "[ \\t\\f\\v]<.*", REG_EXTENDED);
    regcomp(&regex, "[ \\t\\f\\v][1^2]?>[>]?.*", REG_EXTENDED);
    regcomp(&regex, "[ \\t\\f\\v]2>[>]?.*", REG_EXTENDED);

    state->in_redirect_regex = &regex;
    state->out_redirect_regex = &regex;
    state->err_redirect_regex = &regex;

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

    // Nothing updates to err struct so might not need to check
//    if (dc_error_has_error(err)) {
//        state->fatal_error = true;
//        return ERROR;
//    }

    return READ_COMMANDS;
}

char *set_prompt(const struct dc_env *env) {
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

char **get_path(const struct dc_env *env)
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

int read_commands(const struct dc_env *env, struct dc_error *err, void *arg) {
    // Get the state from arg
    struct state * state = (struct state *) arg;
    size_t line_length = 0;

    // Get working directory
    char * current_directory = dc_get_working_dir(env, err);
    if (dc_error_has_error(err)) {
        state->fatal_error = true;
        return ERROR;
    }

    // Display input line
    fprintf(state->std_out, "[%s] %s", current_directory, state->prompt);

    // Read Line
    state->current_line = dc_malloc(env, err, sizeof(char));
    if (dc_error_has_error(err)) {
        state->fatal_error = true;
        return ERROR;
    }

    dc_getline(env, err, &state->current_line, &line_length, state->std_in);
    if (dc_error_has_error(err)) {
        state->fatal_error = true;
        return ERROR;
    }

    dc_str_trim(env, state->current_line);
    line_length = dc_strlen(env, state->current_line);

    // If empty string then reset state
    if (line_length == 0) {
        return RESET_STATE;
    }

    // Set line length to state object
    state->current_line_length = line_length;

    return SEPARATE_COMMANDS;

}

int separate_commands(const struct dc_env *env, struct dc_error *err, void *arg) {
    // Get the state from arg
    struct state * state = (struct state *) arg;

    printf("%s\n", state->current_line);

    return DC_FSM_EXIT;

}
int parse_commands(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

      return DC_FSM_EXIT;

}
int parse_command(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

    return DC_FSM_EXIT;

}
int execute_commands(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

     return DC_FSM_EXIT;

}
int builtin_cd(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

     return DC_FSM_EXIT;

}
int execute(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

     return DC_FSM_EXIT;

}
int redirect(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

     return DC_FSM_EXIT;

}
int handle_run_error(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

     return DC_FSM_EXIT;

}
int do_exit(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

     return DC_FSM_EXIT;

}
int do_reset_state(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

     return DC_FSM_EXIT;

}
int handle_error(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

     return DC_FSM_EXIT;

}
int destroy_state(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

     return DC_FSM_EXIT;

}
