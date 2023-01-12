#include "input.h"

#include "command.h"
#include "shell_impl.h"
#include <dc_util/filesystem.h>
#include <dc_posix/dc_stdio.h>
#include <dc_util/strings.h>
#include "state.h"
#include <string.h>
#include <stdio.h>
#include <dc_error/error.h>


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
