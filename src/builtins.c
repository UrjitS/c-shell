#include "builtins.h"
#include <dc_posix//dc_unistd.h>
#include <dc_util/path.h>
#include "state.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dc_error/error.h>

void builtin_cd(const struct dc_env *env, struct dc_error *err, void *arg) {
    // Get the state from arg
    struct state * state = (struct state *) arg;

    char * path;
    if (state->command->argv[1] == NULL) {
        // Set path to home and change directories
        dc_expand_path(env, err, &path, "~/");
        dc_chdir(env, err, path);
    } else {
        // Set path to given location and change directories
        dc_chdir(env, err, state->command->argv[1]);
        path = strdup(state->command->argv[1]);
    }
    // Check for errors when changing directories
    if (dc_error_has_error(err)) {
        if (dc_error_is_errno(err, EACCES)) {
            fprintf(stdout, "%s: Permission Denied\n", path);
        }
        else if (dc_error_is_errno(err, ELOOP)) {
            fprintf(stdout, "%s: Too Many Symbolic Links Encountered\n", path);
        }
        else if (dc_error_is_errno(err, ENAMETOOLONG)) {
            fprintf(stdout, "%s: File Name Too Long\n", path);
        }
        else if (dc_error_is_errno(err, ENOENT)) {
            fprintf(stdout, "%s: No Such File Or Directory\n", path);
        }
        else if (dc_error_is_errno(err, ENOTDIR)) {
            fprintf(stdout, "%s: Not A Directory\n", path);
        }
        state->command->exit_code = 1;
    } else {
        state->command->exit_code = 0;
    }
    // Free resources
    free(path);
}
