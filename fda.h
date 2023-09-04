#pragma once

#include "fda_states.h"

typedef enum {
    WORD,
    NOT_WORD,
    NEXT
} FDA_Result;

typedef struct 
{
    const char *alphabet;
    FDA_States states;
    FDA_States fin_states;

    state_t output[MAX_STATE_NUM][256];

    int init_state;
    int cur_state;
} FDA;

void fda_set_alphabet(FDA *aut, const char *alphabet);
void fda_set_states(FDA *aut, FDA_States allowed_states);
void fda_set_final_states(FDA *aut, FDA_States final_states);
void fda_set_initial_state(FDA *aut, state_t initial_state);

void fda_add_rule(FDA *aut, state_t state, char input, state_t res_state);

static state_t fda_get_out_state(const FDA *aut, char input);

void fda_reset(FDA *aut);
FDA_Result fda_step(FDA *aut, char input);