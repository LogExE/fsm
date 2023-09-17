#pragma once

#include "fsm_states.h"

#include <stdio.h>
#include <stdbool.h>

#define FSM_ALPHABET_SIZE 26

#define LINE_SIZE 512
#define FILE_MAX_RULES_COUNT 512

typedef struct
{
    char *alphabet;

    struct FSM_States *states;
    struct FSM_States *fin_states;
    int init_state;

    struct FSM_States *output[FSM_MAX_STATE_NUM][FSM_ALPHABET_SIZE];
} FSM_Spec;

bool fsm_spec_check_is_final(FSM_Spec spec, fsm_state_t state);

bool fsm_spec_read_from(FILE *stream, FSM_Spec *spec);
void fsm_spec_output(FSM_Spec spec);