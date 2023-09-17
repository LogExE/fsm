#pragma once

#define FSM_MAX_STATE_NUM 1024

typedef int state_t;

typedef struct FSM_States fsm_states_t;

fsm_states_t *fsm_states_create(state_t *values, int count);
void fsm_states_free(fsm_states_t *states);

state_t fsm_states_at(const fsm_states_t *states, int i);
int fsm_states_count(const fsm_states_t *states);