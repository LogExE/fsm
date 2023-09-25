#pragma once

#include "fsm_spec.h"
#include "stdbool.h"

struct KDA;

#define KDA_STATE_FAILED 0

struct KDA *kda_create(struct FSM_Spec *spec);
void kda_free(struct KDA *aut);

void kda_reset(struct KDA *aut);
void kda_step(struct KDA *aut, char input);

fsm_state_t nda_get_state(const struct KDA *aut);
bool kda_recognized(const struct KDA *aut);