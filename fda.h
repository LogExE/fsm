#pragma once

#include "fda_states.h"

#define FDA_OUTPUT_STATE_NONE 0
#define FDA_ALPHABET_SIZE 26

typedef enum {
    WORD,
    NOT_WORD,
    NEXT
} FDA_Result;

typedef struct 
{
    char *alphabet;
    FDA_States states;
    FDA_States fin_states;

    state_t output[FDA_MAX_STATE_NUM][FDA_ALPHABET_SIZE];

    int init_state;
    int cur_state;
} FDA;

void fda_set_alphabet(FDA *aut, char *alphabet);
void fda_set_states(FDA *aut, FDA_States allowed_states);
void fda_set_final_states(FDA *aut, FDA_States final_states);
void fda_set_initial_state(FDA *aut, state_t initial_state);
void fda_add_rule(FDA *aut, state_t state, char input, state_t res_state);

void fda_init(FDA *aut);
void fda_free(FDA *aut);

static state_t fda_get_out_state(const FDA *aut, char input);

void fda_reset(FDA *aut);
FDA_Result fda_step(FDA *aut, char input);
void fda_output_rules(const FDA *aut);