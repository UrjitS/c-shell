#ifndef OPEN_STATE_H
#define OPEN_STATE_H

#include "command.h"
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <dc_env/env.h>

/**
 * Stores the state of the shell
 * (i.e. in/out files, path, lines, commands to execute)
 */
struct state {
    /**
     * Input file
     */
    FILE * std_in;
    /**
     * Output file
     */
    FILE * std_out;
    /**
     * Used to find input redirection: < path
     */
    regex_t * in_redirect_regex;
    /**
     * Used to find output redirection: [1]>[>] path
     */
    regex_t * out_redirect_regex;
    /**
     * Used to find output redirection: 2>[>] path
     */
    regex_t * err_redirect_regex;
    /**
     * An array of directories to search for external commands
     */
    char ** path;
    /**
     * Prompt to display to the user, defaults to $
     */
    char * prompt;
    /**
     *  The longest possible command line
     */
    long max_line_length; // NOLINT(Wattributes)
    /**
     * The current command line
     */
    char * current_line;
    /**
     *  The length of the current command line
     */
    size_t current_line_length; // NOLINT(Wattributes)
    /**
     *  The command to execute
     */
    struct command * command;
    /**
     * True if an error happened that should exit the shell
     */
    bool fatal_error;
};


#endif
