#pragma once

#include "stdbool.h"

#include "fsm_spec.h"

struct FSM_States_Set;

struct FSM_States_Set *fsm_states_set_create();
void fsm_states_set_free(struct FSM_States_Set *set);

void fsm_states_set_add(struct FSM_States_Set *set, fsm_state_t state);
void fsm_states_set_remove(struct FSM_States_Set *set, fsm_state_t state);
void fsm_states_set_reset(struct FSM_States_Set *set);

bool fsm_states_set_contains(const struct FSM_States_Set *set, fsm_state_t state);
struct FSM_States *fsm_states_set_convert(const struct FSM_States_Set *set);