#include "fda.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

FDA *fda_create(FDA_Spec *spec)
{
    FDA *res = malloc(sizeof(FDA));
    res->spec = spec;
    return res;
}

void fda_free(FDA *aut)
{
    free(aut);
}

static state_t fda_get_out_state(const FDA *aut, char input)
{
    return aut->spec->output[aut->cur_state][input];
}

void fda_reset(FDA *aut)
{
    aut->cur_state = aut->spec->init_state;
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
    for (int i = 0; i < aut->spec->fin_states->count; ++i)
        if (aut->spec->fin_states->values[i] == new_state)
            return WORD;
    return NEXT;
}

void fda_output_rules(const FDA *aut)
{
    printf(" |");
    char *seek = aut->spec->alphabet;
    while (*seek)
        printf("%c|", *seek++);
    printf("\n");
    for (int i = 0; i < aut->spec->states->count; ++i)
    {
        state_t state = aut->spec->states->values[i];
        printf("%d|", state);
        seek = aut->spec->alphabet;
        while (*seek)
        {
            state_t to_print = aut->spec->output[state][*seek++ - 'a'];
            if (to_print == FDA_OUTPUT_STATE_NONE)
                printf("x");
            else
                printf("%d", to_print);
            printf("|");
        }
        bool initial = state == aut->spec->init_state;
        bool final = false;
        for (int j = 0; j < aut->spec->fin_states->count; ++j)
            if (aut->spec->fin_states->values[j] == state)
            {
                final = true;
                break;
            }
        if (initial || final)
        {
            printf(" <- ");
            if (initial && final)
                printf("inital and final");
            else if (initial)
                printf("initial");
            else
                printf("final");
            printf(" state");
        }
        printf("\n");
    }
}
