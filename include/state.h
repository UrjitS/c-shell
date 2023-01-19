#ifndef OPEN_STATE_H
#define OPEN_STATE_H


#include <regex.h>
#include <stdbool.h>
#include <stdio.h>


/**
 * Stores all the information needed to perform a command
 * (i.e. command, arguments for that command, in/out files ...)
 */
struct command
{
    /**
     * Command line for this command
     */
    char * line;
    /**
     * The command (e.g. ls, exit, cd, cat)
     */
    char * command;
    /**
     * The number of arguments passed to the command
     */
    size_t argc;
    /**
     *  The arguments passed to the command
     */
    char ** argv;
    /**
     * The file to redirect stdin from or NULL
     */
    char * stdin_file;
    /**
     * The file to redirect stdout from or NULL
     */
    char * stdout_file;
    /**
     * Append to or truncate the stdout file
     */
    bool stdout_overwrite;
    /**
     * The file to redirect stderr to, or NULL
     */
    char * stderr_file;
    /**
     * Append to or truncate the stderr file
     */
    bool stderr_overwrite;
    /**
     *  The status returned from the command
     */
    int exit_code;
};


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
    size_t max_line_length; // NOLINT(Wattributes)
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
