#pragma once

#include "fsm_states.h"

#include <stdio.h>
#include <stdbool.h>

#define FSM_ALPHABET_SIZE 27

#define FILE_MAX_RULES_COUNT 512

#define FSM_SYMBOL_EPS '{'

struct FSM_Spec
{
    char *alphabet;

    struct FSM_States *states;
    struct FSM_States *fin_states;
    int init_state;

    struct FSM_States *output[FSM_MAX_STATE_NUM][FSM_ALPHABET_SIZE];
};

bool fsm_spec_check_is_final(struct FSM_Spec spec, fsm_state_t state);

bool fsm_spec_read_from(FILE *stream, struct FSM_Spec *spec);
void fsm_spec_free(struct FSM_Spec spec);
void fsm_spec_output(struct FSM_Spec spec);
bool fsm_spec_nondeterministic(struct FSM_Spec spec);
bool fsm_spec_eps(struct FSM_Spec spec);