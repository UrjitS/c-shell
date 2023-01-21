#include <wordexp.h>
#include "shell_impl.h"
#include "command.h"
#include "shell.h"
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <dc_posix/dc_string.h>
#include <ctype.h>

/**
 * Removes leading whitespaces from the given string
 * @param string String to remove leading whitespaces from
 * @return Same string without leading spaces
 */
char * remove_leading_whitespace(char * string);
/**
 * Expands the given string
 * @param env Environment object.
 * @param err Error object.
 * @param string String to expand.
 * @return Expanded string.
 */
char * word_expand(const struct dc_env * env, struct dc_error * err, char * string);

char * remove_leading_whitespace(char * string) {
    // Duplicate the string
    char * duplicate_string;
    duplicate_string = string;

    // Loop through and check for any spaces
    while (*duplicate_string != '\0') {
        if (isspace(*duplicate_string))
            // If space go to next character
            duplicate_string++;
        else {
            string = duplicate_string;
            break;
        }
    }

    return string;
}

char * word_expand(const struct dc_env * env, struct dc_error * err, char * string) {
    wordexp_t exp;
    int status;
    char * expanded_string;
    // Expand string
    status = wordexp(string, &exp, 0);

    if (status == 0) {
        // Set expanded string
        expanded_string = dc_strdup(env, err, exp.we_wordv[0]);
    }
    // Free resources
    wordfree(&exp);
    return expanded_string;
}

int parse_command(const struct dc_env *env, struct dc_error *err, void *arg) {
    // Get the state from arg
    struct state * state = (struct state *) arg;
    // Run input string through error regex
    char * regex_error_string = regex_error(env, err, state, state->command->line);
    // Run error string through the out regex
    char * regex_out_string = regex_out(env, err, state, regex_error_string);
    // Run out string through in regex
    char * regex_in_string = regex_in(env, err, state, regex_out_string);
    // Set command arguments
    set_command_arguments(env, err, state, regex_in_string);

    return EXECUTE_COMMANDS;
}

char * regex_error(const struct dc_env * env, struct dc_error * err,struct state * state, char * string) {
    regmatch_t match;
    char * str = NULL;
    int matched;
    regoff_t line_cut_offset = 3;

    // Check regex
    matched = regexec(state->err_redirect_regex, state->command->line,  1, &match, 0);

    if (matched == 0) {
        regoff_t matched_length = match.rm_eo - match.rm_so;

        str = dc_malloc(env, err, (matched_length + 1));
        if (dc_error_has_error(err)) {
            state->fatal_error = true;
            return NULL;
        }

        dc_strncpy(env, str, &state->command->line[match.rm_so], matched_length);
        // Set null character
        str[matched_length] = '\0';


        // Check if >> is present
        if (strstr(str, ">>") != NULL) {
            state->command->stderr_overwrite = true;
            line_cut_offset++;
        }
        // Remove whitespace from line
        str = remove_leading_whitespace(str + line_cut_offset);
        // Set state.command.stderr_file to the expanded file
        state->command->stderr_file = word_expand(env, err, strdup(str));

        char * changed_str = malloc(match.rm_so + 1);
        strncpy(changed_str, string, match.rm_so);
        changed_str[match.rm_so] = '\0';

        return changed_str;
    }
    return string;
}

char * regex_out(const struct dc_env *env, struct dc_error *err,struct state *state, char* string) {
    regmatch_t match;
    char * str = NULL;
    int matched;
    regoff_t line_cut_offset = 3;

    // Check regex
    matched = regexec(state->out_redirect_regex, state->command->line,  1, &match, 0);

    if (matched == 0) {
        regoff_t matched_length = match.rm_eo - match.rm_so;

        // try (unsigned long)
        str = dc_malloc(env, err, (matched_length + 1));
        if (dc_error_has_error(err)) {
            state->fatal_error = true;
            return NULL;
        }

        dc_strncpy(env, str, &state->command->line[match.rm_so], matched_length);
        // Set null character
        str[matched_length] = '\0';


        // Check if >> is present
        if (strstr(str, ">>") != NULL) {
            state->command->stdout_overwrite = true;
            line_cut_offset++;
        }

        // Remove whitespace from line
        str = remove_leading_whitespace(str + line_cut_offset);
        // Set state.command.stdout_file to the expanded file
        state->command->stdout_file = word_expand(env, err, strdup(str));
        char * changed_str = malloc(match.rm_so + 1);
        strncpy(changed_str, string, match.rm_so);
        changed_str[match.rm_so] = '\0';

        return changed_str;
    }
    return string;
}

char * regex_in(const struct dc_env *env, struct dc_error *err,struct state *state, char* string) {
    regmatch_t match;
    char * str = NULL;
    int matched;
    regoff_t line_cut_offset = 2;

    // Check regex
    matched = regexec(state->in_redirect_regex, state->command->line,  1, &match, 0);

    if (matched == 0) {
        regoff_t length = match.rm_eo - match.rm_so;

        str = malloc((length + 1));
        strncpy(str, &string[match.rm_so], length);
        str[length] = '\0';

        // Remove whitespace from line
        str = remove_leading_whitespace(str + line_cut_offset);
        // Set state.command.stdin_file to the expanded file
        state->command->stdin_file = word_expand(env, err, strdup(str));

        // Set command.line to the cutout version
        char * cutout_string = dc_malloc(env, err, match.rm_so + 1);
        dc_strncpy(env, cutout_string, state->command->line, match.rm_so);
        cutout_string[match.rm_so] = '\0';
        return cutout_string;
    }
    return string;
}

void set_command_arguments(const struct dc_env * env, struct dc_error * err, struct state * state, char * string) {
    wordexp_t exp;
    int status;

    // Expand the final string
    status = wordexp(string, &exp, 0);

    if (status == 0) {
        state->command->argc = exp.we_wordc;
        // NOTE + 2 is for argv[0] will become the program name, argv[n] is always NULL
        state->command->argv = dc_calloc(env, err, exp.we_wordc + 2, sizeof(char*));
        if (dc_error_has_error(err)) {
            wordfree(&exp);
            free(state->command->argv);
            state->fatal_error = true;
            return;
        }

        // Copy each we_wordv into argv starting at 1
        for (size_t i = 1; i < exp.we_wordc; i++) {
            state->command->argv[i] = dc_strdup(env, err, exp.we_wordv[i]);
            if (dc_error_has_error(err)) {
                wordfree(&exp);
                free(state->command->argv);
                state->fatal_error = true;
                return;
            }
        }
        // Copy over the command
        state->command->command = dc_strdup(env, err, exp.we_wordv[0]);
        if (dc_error_has_error(err)) {
            wordfree(&exp);
            free(state->command->argv);
            state->fatal_error = true;
            return;
        }
        // Free resources
        wordfree(&exp);
    }
}
