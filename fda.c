#include "fda.h"

#include <stdio.h>

void fda_set_alphabet(FDA *aut, char *alphabet)
{
    aut->alphabet = alphabet;
}

void fda_set_states(FDA *aut, FDA_States allowed_states)
{
    aut->states.count = allowed_states.count;
    aut->states.states = allowed_states.states;
}

void fda_set_final_states(FDA *aut, FDA_States final_states)
{
    aut->fin_states.count = final_states.count;
    aut->fin_states.states = final_states.states;
}

void fda_set_initial_state(FDA *aut, state_t initial_state)
{
    aut->init_state = initial_state;
}

void fda_add_rule(FDA *aut, state_t state, char input, state_t res_state)
{
    aut->output[state][input] = res_state;
}

static state_t fda_get_out_state(const FDA *aut, char input)
{
    return aut->output[aut->cur_state][input];
}

void fda_reset(FDA *aut)
{
    aut->cur_state = aut->init_state;
}

FDA_Result fda_step(FDA *aut, char input)
{
    state_t new_state = fda_get_out_state(aut, input);
    printf("Automata got input %c, changing state from %d into %d\n", 
        input, aut->cur_state, new_state);
    aut->cur_state = new_state;
    for (int i = 0; i < aut->fin_states.count; ++i)
        if (aut->fin_states.states[i] == new_state)
        {
            printf("Automata is in final state!\n");
            return WORD;
        }
    return NEXT;
}

void fda_free(FDA *aut)
{
    free(aut->alphabet);
    fda_states_free(&aut->states);
    fda_states_free(&aut->fin_states);
}
