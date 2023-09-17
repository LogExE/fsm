#pragma once

#include "fsm_states.h"

#include <stdio.h>
#include <stdbool.h>

#define FSM_ALPHABET_SIZE 26
#define FSM_OUTPUT_STATE_NONE 0

#define LINE_SIZE 512
#define FILE_MAX_RULES_COUNT 512

typedef struct
{
    char *alphabet;

    fsm_states_t *states;
    fsm_states_t *fin_states;
    int init_state;

    state_t output[FSM_MAX_STATE_NUM][FSM_ALPHABET_SIZE];
} FSM_Spec;

bool fsm_spec_check_is_final(FSM_Spec spec, state_t state);

bool fsm_spec_read_from(FILE *stream, FSM_Spec *spec);
void fsm_spec_output(FSM_Spec spec);