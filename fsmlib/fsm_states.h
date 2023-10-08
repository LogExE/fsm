#pragma once

#include "stdbool.h"

#define FSM_MAX_STATE_NUM 1024

typedef int fsm_state_t;

struct FSM_States;
struct FSM_Spec;

struct FSM_States *fsm_states_create();
void fsm_states_free(struct FSM_States *states);
struct FSM_States *fsm_states_copy(const struct FSM_States *states);

void fsm_states_add(struct FSM_States *arr, fsm_state_t state);
void fsm_states_add_uniq(struct FSM_States *arr, fsm_state_t state);
void fsm_states_remove(struct FSM_States *arr, int idx);
void fsm_states_purge(struct FSM_States *arr, fsm_state_t state);
void fsm_states_reset(struct FSM_States *arr);

fsm_state_t fsm_states_at(const struct FSM_States *states, int i);
int fsm_states_count(const struct FSM_States *states);
bool fsm_states_contains(const struct FSM_States *states, fsm_state_t state);
bool fsm_states_subset(const struct FSM_States *states1, const struct FSM_States *states2);
bool fsm_states_alike(const struct FSM_States *states1, const struct FSM_States *states2);