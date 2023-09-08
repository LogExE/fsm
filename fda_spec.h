#pragma once

#include "fda_states.h"

#include <stdio.h>
#include <stdbool.h>

#define FDA_ALPHABET_SIZE 26
#define FDA_OUTPUT_STATE_NONE 0

#define LINE_SIZE 512
#define FILE_MAX_RULES_COUNT 512

typedef struct
{
    char *alphabet;

    fda_states_t states;
    fda_states_t fin_states;
    int init_state;

    state_t output[FDA_MAX_STATE_NUM][FDA_ALPHABET_SIZE];
} FDA_Spec;

bool fda_spec_check_is_final(FDA_Spec spec, state_t state);

bool fda_spec_read_from(FILE *stream, FDA_Spec *spec);
void fda_spec_output(FDA_Spec spec);