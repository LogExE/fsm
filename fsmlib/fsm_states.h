#pragma once

#define FSM_MAX_STATE_NUM 1024

typedef int fsm_state_t;

struct FSM_States;

struct FSM_States *fsm_states_create();
void fsm_states_free(struct FSM_States *states);

void fsm_states_add(struct FSM_States *arr, fsm_state_t state);
fsm_state_t fsm_states_at(const struct FSM_States *states, int i);
int fsm_states_count(const struct FSM_States *states);