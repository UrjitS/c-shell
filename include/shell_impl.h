#ifndef OPEN_SHELL_IMPL_H
#define OPEN_SHELL_IMPL_H

#include <dc_env/env.h>
#include <dc_error/error.h>
#include <getopt.h>
#include <stdlib.h>
#include <dc_c/dc_stdlib.h>
#include <dc_c/dc_string.h>
#include <stddef.h>
#include <dc_fsm/fsm.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "command.h"


enum scuffed_shell_states
{
    INIT_STATE = DC_FSM_USER_START,    // 2
    READ_COMMANDS,
    ERROR,
    RESET_STATE,
    SEPARATE_COMMANDS,
    PARSE_COMMANDS,
    EXECUTE_COMMANDS,
    EXIT,
    DESTROY_STATE,
};

int init_state(const struct dc_env *env, __attribute__((unused)) struct dc_error *err, void *arg);
int read_commands(const struct dc_env *env, struct dc_error *err, void *arg);
int separate_commands(const struct dc_env *env, struct dc_error *err, void *arg);
int parse_commands(const struct dc_env *env, struct dc_error *err, void *arg);
int parse_command(const struct dc_env *env, struct dc_error *err, void *arg);
int execute_commands(const struct dc_env *env, struct dc_error *err, void *arg);
int reset_state(const struct dc_env *env, struct dc_error *err, void *arg);
void builtin_cd(const struct dc_env *env, struct dc_error *err, void *arg);
void execute(const struct dc_env *env, struct dc_error *err, void *arg);
void redirect(const struct dc_env *env, struct dc_error *err, void *arg);
void run(const struct dc_env *env, struct dc_error *err, struct command * command, char ** path);
int handle_run_error(__attribute__((unused)) const struct dc_env *env, struct dc_error *err, void *arg);
int do_exit(const struct dc_env *env, struct dc_error *err, void *arg);
int do_reset_state(__attribute__((unused)) const struct dc_env *env, struct dc_error *err, void *arg);
int handle_error(__attribute__((unused)) const struct dc_env *env, struct dc_error *err, void *arg);
int destroy_state(const struct dc_env *env, struct dc_error *err, void *arg);

#endif //OPEN_SHELL_IMPL_H
