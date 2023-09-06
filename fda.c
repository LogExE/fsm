#include "fda.h"

#include <stdio.h>
#include <stdlib.h>

void fda_set_alphabet(FDA *aut, char *alphabet)
{
    aut->alphabet = alphabet;
}

void fda_set_states(FDA *aut, FDA_States allowed_states)
{
    aut->states.count = allowed_states.count;
    aut->states.values = allowed_states.values;
}

void fda_set_final_states(FDA *aut, FDA_States final_states)
{
    aut->fin_states.count = final_states.count;
    aut->fin_states.values = final_states.values;
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
    if (new_state != FDA_OUTPUT_STATE_NONE)
    {
        printf("Automaton got input %c, changing state from %d into %d\n",
               input, aut->cur_state, new_state);
        aut->cur_state = new_state;
    }
    else
        printf("Automaton got input %c, rule wasn't set or state doesn't need to be changed\n", input);
    for (int i = 0; i < aut->fin_states.count; ++i)
        if (aut->fin_states.values[i] == new_state)
            return WORD;
    return NEXT;
}

void fda_output_rules(const FDA *aut)
{
    printf(" |");
    char *seek = aut->alphabet;
    while (*seek)
        printf("%c|", *seek++);
    printf("\n");
    for (int i = 0; i < aut->states.count; ++i)
    {
        printf("%d|", aut->states.values[i]);
        seek = aut->alphabet;
        while (*seek)
        {
            state_t to_print = aut->output[aut->states.values[i]][*seek++];
            if (to_print == FDA_OUTPUT_STATE_NONE)
                printf("x");
            else
                printf("%d", to_print);
            printf("|");
        }
        for (int j = 0; j < aut->fin_states.count; ++j)
            if (aut->fin_states.values[j] == aut->states.values[i])
            {
                printf(" <- final state");
            }
        printf("\n");
    }
}

void fda_init(FDA *aut)
{
    for (int i = 0; i < FDA_MAX_STATE_NUM; ++i)
        for (int j = 0; j < FDA_ALPHABET_SIZE; ++j)
            aut->output[i][j] = FDA_OUTPUT_STATE_NONE;
}

void fda_free(FDA *aut)
{
    free(aut->alphabet);
    fda_states_free(&aut->states);
    fda_states_free(&aut->fin_states);
}
