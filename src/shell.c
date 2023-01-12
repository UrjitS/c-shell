#include "shell_impl.h"
#include <dc_fsm/fsm.h>
#include "shell.h"
#include <string.h>
#include <stdlib.h>
#include "state.h"
#include "input.h"

int run_shell(const struct dc_env *env, struct dc_error *err) {
    struct dc_fsm_info *fsm_info;
    static struct dc_fsm_transition transitions[] = {
            {DC_FSM_INIT,      INIT_STATE,         init_state},
            {INIT_STATE,       READ_COMMANDS,      read_commands},
            {INIT_STATE,       ERROR,              handle_error},
            {READ_COMMANDS,    RESET_STATE,        reset_state},
            {READ_COMMANDS,    SEPARATE_COMMANDS,  separate_commands},
            {READ_COMMANDS,    ERROR,              handle_error},
            {SEPARATE_COMMANDS,PARSE_COMMANDS,     parse_commands},
            {SEPARATE_COMMANDS,ERROR,              handle_error},
            {PARSE_COMMANDS,   EXECUTE_COMMANDS,   execute_commands},
            {PARSE_COMMANDS,   ERROR,              handle_error},
            {EXECUTE_COMMANDS,RESET_STATE,        reset_state},
            {EXECUTE_COMMANDS,EXIT,               do_exit},
            {EXECUTE_COMMANDS,ERROR,              handle_error},
            {RESET_STATE,     READ_COMMANDS,      read_commands},
            {EXIT,            DESTROY_STATE ,     destroy_state},
            {ERROR,           RESET_STATE,        reset_state},
            {ERROR,           DESTROY_STATE,      destroy_state},
            {DESTROY_STATE,   DC_FSM_EXIT,        NULL},
    };

    fsm_info = dc_fsm_info_create(env, err, "scuffed shell");

    if(dc_error_has_no_error(err))
    {
        // Create a state to pass around
        struct state * state;
        state = malloc(sizeof(struct state *));

        int from_state;
        int to_state;

        dc_fsm_run(env, err, fsm_info, &from_state, &to_state, state, transitions);
        dc_fsm_info_destroy(env, &fsm_info);
    }

    return EXIT_SUCCESS;
}
