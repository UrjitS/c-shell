#include "execute.h"
#include "util.h"
#include "shell_impl.h"
#include <dc_util/filesystem.h>
#include <dc_posix/dc_unistd.h>
#include "state.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dc_error/error.h>
#include <regex.h>
#include <fcntl.h>
#include <sys/wait.h>

void execute(const struct dc_env *env, struct dc_error *err, void *arg) {
    // Get the state from arg
    struct state * state = (struct state *) arg;
    int status;
    pid_t pid;
    pid = fork();

    if (pid == 0) {
        redirect(env, err, state);
        if (dc_error_has_error(err)) {
            exit(126);
        }
        run(env, err, state->command, state->path);
        status = handle_run_error(env, err, state);
        exit(status);
    } else {
        waitpid(pid, &status, WUNTRACED | WCONTINUED);

        if (WIFEXITED(status)) {
//            printf("exited, status=%d\n", WEXITSTATUS(status));
            state->command->exit_code = WEXITSTATUS(status);
        }
    }
}

void redirect(const struct dc_env *env, struct dc_error *err, void *arg) {
    // Get the state from arg
    struct state * state = (struct state *) arg;

    if (state->command->stdin_file != NULL) {
        int fd;
        // Open the new stdin_file
        fd =  open(state->command->stdin_file, O_RDONLY, S_IRWXO | S_IRWXG | S_IRWXU);
        // change standard in file to new stdin_file
        dc_dup2(env, err, fd, STDIN_FILENO);
        if (dc_error_has_error(err)) {
            state->fatal_error = true;
            return;
        }
        // Close stdin_file
        dc_close(env, err, fd);
    }

    if (state->command->stdout_file != NULL) {
        int fd;
        // Check if it should overwrite or truncate
        if (state->command->stdout_overwrite == true) {
            // Open the new stdout_file to truncate
            fd = open(state->command->stdout_file, O_CREAT | O_RDWR | O_TRUNC, S_IRWXO | S_IRWXG | S_IRWXU);
        } else {
            // Open the new stdout_file to append
            fd = open(state->command->stdout_file, O_CREAT | O_RDWR | O_APPEND, S_IRWXO | S_IRWXG | S_IRWXU);
        }
        // change standard in file to new stdout_file
        dc_dup2(env, err, fd, STDOUT_FILENO);
        if (dc_error_has_error(err)) {
            state->fatal_error = true;
            return;
        }
        // Close stdout_file
        dc_close(env, err, fd);
    }

    if (state->command->stderr_file != NULL) {
        int fd;
        // Check if it should overwrite or truncate
        if (state->command->stderr_overwrite == true) {
            // Open the new stderr_file to truncate
            fd = open(state->command->stderr_file, O_CREAT | O_RDWR | O_TRUNC, S_IRWXO | S_IRWXG | S_IRWXU);
        } else {
            // Open the new stderr_file to truncate
            fd = open(state->command->stderr_file, O_CREAT | O_RDWR | O_APPEND, S_IRWXO | S_IRWXG | S_IRWXU);
        }
        // change standard in file to new stderr_file
        dc_dup2(env, err, fd, STDERR_FILENO);
        if (dc_error_has_error(err)) {
            state->fatal_error = true;
            return;
        }
        // Close stderr_file
        dc_close(env, err, fd);
    }
}

void run(const struct dc_env * env, struct dc_error * err, struct command * command, char ** path) {
    if (dc_strstr(env, command->command, "/") != NULL) {
        command->argv[0] = command->command;
        dc_execv(env, err, command->command, command->argv);
    } else {
        if (path[0] == NULL) {
            DC_ERROR_RAISE_ERRNO(err, ENONET);
        } else {
            for (size_t i = 0; i < strlen(*path); ++i)
            {
                // Set cmd to path[i]/command.command
                char * dest = string_cat(env, err, path[i], "/");
                dest = string_cat(env, err, dest, command->command);
                command->argv[0] = dest;
                // Execute command
                dc_execv(env, err, dest, command->argv);
                // Check if command worked
                if(dc_error_has_error(err)) {
                    if (!dc_error_is_errno(err, ENOENT)) {
                        free(dest);
                        break;
                    }
                }
            }
        }
    }
}

int handle_run_error(__attribute__((unused)) const struct dc_env * env, struct dc_error * err, void * arg) { // NOLINT(Wunused-parameter)
    // Get the state from arg
    struct state * state = (struct state *) arg;

    if (dc_error_is_errno(err, E2BIG)) {
        fprintf(stdout, "[%s] Argument List Too Long\n", state->command->command); // NOLINT(cert-err33-c)
        return 1; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    } else if (dc_error_is_errno(err, EACCES)) {
        fprintf(stdout, "[%s] Permission Denied\n", state->command->command); // NOLINT(cert-err33-c)
        return 2; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    } else if (dc_error_is_errno(err, EINVAL)) {
        fprintf(stdout, "[%s] Invalid Argument \n", state->command->command); // NOLINT(cert-err33-c)
        return 3; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    } else if (dc_error_is_errno(err, ELOOP)) {
        fprintf(stdout, "[%s] Too Many Symbolic Links Encountered\n", state->command->command); // NOLINT(cert-err33-c)
        return 4; // NOLINT(cppcorstate->eguidelines-avoid-magic-numbers,readability-magic-numbers)
    } else if (dc_error_is_errno(err, ENAMETOOLONG)) {
        fprintf(stdout, "[%s] File Name Too Long\n", state->command->command); // NOLINT(cert-err33-c)
        return 5; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    } else if (dc_error_is_errno(err, ENOENT)) {
        fprintf(stdout, "[%s] Not Found\n", state->command->command); // NOLINT(cert-err33-c)
        return 127; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    } else if (dc_error_is_errno(err, ENOTDIR)) {
        fprintf(stdout, "[%s] Not A Directory\n", state->command->command); // NOLINT(cert-err33-c)
        return 6; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    } else if (dc_error_is_errno(err, ENOEXEC)) {
        fprintf(stdout, "[%s] Exec Format Error\n", state->command->command); // NOLINT(cert-err33-c)
        return 7; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    } else if (dc_error_is_errno(err, ENOMEM)) {
        fprintf(stdout, "[%s] Out Of Memory\n", state->command->command); // NOLINT(cert-err33-c)
        return 8; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    } else if (dc_error_is_errno(err, ETXTBSY)) {
        fprintf(stdout, "[%s] Text File Busy\n", state->command->command); // NOLINT(cert-err33-c)
        return 9; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    } else {
        return 125; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    }
}
