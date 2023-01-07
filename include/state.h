#ifndef OPEN_STATE_H
#define OPEN_STATE_H

#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <dc_env/env.h>

struct state {
    regex_t * in_redirect_regex;   // used to find input redirection: < path
    regex_t * out_redirect_regex;  // used to find output redirection: [1]>[>] path
    regex_t * err_redirect_regex;  // used to find output redirection: 2>[>] path
    char ** path;                  // An array of directories to search for external commands
    char * prompt;                 // Prompt to display to the user, defaults to $
    long max_line_length;        // The longest possible command line
    char * current_line;           // The current command line
    size_t current_line_length;    // The length of the current command line
    struct command * command;      // The command to execute
    bool fatal_error;              // True if an error happened that should exit the shell
};

#endif //OPEN_STATE_H
