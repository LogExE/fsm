#include "fda.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct FDA
{
    FDA_Spec *spec;

    FDA_Output output;
    int cur_state;
};

fda_t fda_create(FDA_Spec *spec)
{
    fda_t res = malloc(sizeof(struct FDA));
    res->spec = spec;
    return res;
}

void fda_free(fda_t aut)
{
    free(aut);
}

static state_t get_out_state(const fda_t aut, char input)
{
    return aut->spec->output[aut->cur_state][input];
}

void fda_reset(fda_t aut)
{
    aut->cur_state = aut->spec->init_state;
    aut->output = FDA_NEXT;
}

void fda_step(fda_t aut, char input)
{
    state_t new_state = get_out_state(aut, input - 'a');
    if (new_state != FDA_OUTPUT_STATE_NONE)
    {
        aut->cur_state = new_state;
        if (fda_spec_check_is_final(*aut->spec, aut->cur_state))
            aut->output = FDA_RECOGNIZED;
        else
            aut->output = FDA_NEXT;
    }
    else
    {
        printf("Automaton got input %c, rule wasn't set!\n", input);
        aut->output = FDA_FAILED;
    }
}

state_t fda_get_state(const fda_t aut)
{
    return aut->cur_state;
}

FDA_Output fda_get_output(const fda_t aut)
{
    return aut->output;
}