#pragma once

#include "fsm_spec.h"
#include "stdbool.h"

struct NDA_Eps;

struct NDA_Eps *nda_eps_create(struct FSM_Spec *spec);
void nda_eps_free(struct NDA_Eps *aut);

void nda_eps_reset(struct NDA_Eps *aut);
void nda_eps_step(struct NDA_Eps *aut, char input);
struct FSM_States *nda_eps_states_step(const struct FSM_States *states, struct FSM_Spec spec, char input);
struct FSM_States *nda_eps_closure(fsm_state_t state, struct FSM_Spec spec);
struct FSM_States *nda_eps_states_closure(const struct FSM_States *states, struct FSM_Spec spec);

struct FSM_States *nda_eps_get_states(const struct NDA_Eps *aut);
bool nda_eps_recognized(const struct NDA_Eps *aut);

struct FSM_Spec nda_eps_convert_spec_to_nda(struct FSM_Spec spec);