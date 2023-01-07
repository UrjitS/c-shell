#include "shell_impl.h"
#include "state.h"
#include "util.h"
#include "shell.h"
#include "input.h"
#include "builtins.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

 int init_state(const struct dc_env *env, struct dc_error *err, void *arg) {
    struct state * state;

    state->max_line_length = sysconf(_SC_ARG_MAX);
    int next_state;

    printf("Hello1\n");

    next_state = READ_COMMANDS;

    return next_state;
}
 int read_commands(const struct dc_env *env, struct dc_error *err, void *arg) {
    printf("Hello2\n");

    return SEPARATE_COMMANDS;

}
 int separate_commands(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("Hello3\n");

 return DC_FSM_EXIT;

}
 int parse_commands(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

      return DC_FSM_EXIT;

}
 int parse_command(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

    return DC_FSM_EXIT;

}
 int execute_commands(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

     return DC_FSM_EXIT;

}
 int builtin_cd(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

     return DC_FSM_EXIT;

}
int execute(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

     return DC_FSM_EXIT;

}
 int redirect(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

     return DC_FSM_EXIT;

}
 int handle_run_error(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

     return DC_FSM_EXIT;

}
 int do_exit(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

     return DC_FSM_EXIT;

}
 int do_reset_state(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

     return DC_FSM_EXIT;

}
 int handle_error(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

     return DC_FSM_EXIT;

}
 int destroy_state(const struct dc_env *env, struct dc_error *err, void *arg) {
     printf("ERROR\n");

     return DC_FSM_EXIT;

}
