#pragma once

#include "fsm_spec.h"
#include "stdbool.h"

struct NDA;

struct NDA *nda_create(struct FSM_Spec *spec);
void nda_free(struct NDA *aut);

void nda_reset(struct NDA *aut);
void nda_step(struct NDA *aut, char input);
struct FSM_States *nda_states_step(const struct FSM_States *states, struct FSM_Spec spec, char input);

struct FSM_States *nda_get_states(const struct NDA *aut);
bool nda_recognized(const struct NDA *aut);

struct FSM_Spec nda_convert_spec_to_kda(struct FSM_Spec spec);