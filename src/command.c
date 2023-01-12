#include <wordexp.h>
#include "shell_impl.h"
#include "command.h"
#include "shell.h"
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <dc_posix/dc_string.h>
#include <ctype.h>

char * remove_whitespace(char * string);
char * word_expand(const struct dc_env * env, struct dc_error * err, struct state * state, char * string);

char * remove_whitespace(char * string) {
    char * duplicate_string;
    duplicate_string = string;

    while (*duplicate_string != '\0') {
        if (isspace(*duplicate_string))
            duplicate_string++;
        else {
            string = duplicate_string;
            break;
        }
    }

    return string;
}

char * word_expand(const struct dc_env * env, struct dc_error * err, struct state * state, char * string) {
    wordexp_t exp;
    int status;
    char * expanded_string;

    status = wordexp(string, &exp, 0);

    if (status == 0) {
        expanded_string = dc_malloc(env, err, strlen(exp.we_wordv[0]));
        if (dc_error_has_error(err)) {
            state->fatal_error = true;
            return string;
        }
        dc_strcpy(env, expanded_string, exp.we_wordv[0]);
    }

    wordfree(&exp);
    return expanded_string;
}

int parse_command(const struct dc_env *env, struct dc_error *err, void *arg) {
    // Get the state from arg
    struct state * state = (struct state *) arg;

    char * regex_error_string = regex_error(env, err, state, state->command->line);
    char * regex_out_string = regex_out(env, err, state, regex_error_string);
    char * regex_in_string = regex_in(env, err, state, regex_out_string);

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
        str = remove_whitespace(str + line_cut_offset);
        // Set state.command.stderr_file to the expanded file
        state->command->stderr_file = word_expand(env, err, state, strdup(str));

        char * changed_str = malloc(sizeof(char) * (unsigned long long int) match.rm_so + 1);
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
        str = remove_whitespace(str + line_cut_offset);
        // Set state.command.stdout_file to the expanded file
        state->command->stdout_file = word_expand(env, err, state, strdup(str));

        char * changed_str = malloc(sizeof(char) * (unsigned long long int) match.rm_so + 1);
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
        str = remove_whitespace(str + line_cut_offset);
        // Set state.command.stdin_file to the expanded file
        state->command->stdin_file = word_expand(env, err, state, strdup(str));


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

    status = wordexp(string, &exp, 0);

    if (status == 0) {
        state->command->argc = exp.we_wordc;
        // NOTE + 2 is for argv[0] will become the program name, argv[n] is always NULL
        state->command->argv = dc_calloc(env, err, exp.we_wordc + 2, sizeof(char*));
        if (dc_error_has_error(err)) {
            wordfree(&exp);
            state->fatal_error = true;
            return;
        }

        // Copy each we_wordv into argv starting at 1
        for (size_t i = 1; i < exp.we_wordc; i++) {
            state->command->argv[i] = dc_strdup(env, err, exp.we_wordv[i]);
            if (dc_error_has_error(err)) {
                wordfree(&exp);
                state->fatal_error = true;
                return;
            }
        }

        state->command->command = dc_strdup(env, err, exp.we_wordv[0]);
        if (dc_error_has_error(err)) {
            wordfree(&exp);
            state->fatal_error = true;
            return;
        }
        wordfree(&exp);
    }
}
