#pragma once

#include "fda_states.h"

#include <stdio.h>

#define FDA_ALPHABET_SIZE 26
#define FDA_OUTPUT_STATE_NONE 0

#define LINE_SIZE 512
#define FILE_MAX_RULES_COUNT 512

typedef struct
{
    char *alphabet;

    FDA_States *states;
    FDA_States *fin_states;
    int init_state;

    state_t output[FDA_MAX_STATE_NUM][FDA_ALPHABET_SIZE];
} FDA_Spec;

FDA_Spec *fda_spec_create(char *alphabet, FDA_States *allowed_states, FDA_States *final_states, state_t initial_state);
void fda_spec_add_rule(FDA_Spec *spec, state_t state, char input, state_t res_state);
void fda_spec_free(FDA_Spec *spec);
FDA_Spec *fda_spec_read_from(FILE *stream);