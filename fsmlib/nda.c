#include "nda.h"

#include <stdlib.h>
#include <ctype.h>

struct NDA
{
    struct FSM_Spec *spec;
    struct FSM_States *cur_states;
};

struct NDA *nda_create(struct FSM_Spec *spec)
{
    struct NDA *ret = malloc(sizeof(struct NDA));
    ret->spec = spec;
    ret->cur_states = fsm_states_create();
    fsm_states_add(ret->cur_states, spec->init_state);
    return ret;
}

void nda_free(struct NDA *aut)
{
    fsm_states_free(aut->cur_states);
    free(aut);
}

void nda_reset(struct NDA *aut)
{
    fsm_states_reset(aut->cur_states);
    fsm_states_add(aut->cur_states, aut->spec->init_state);
}

void nda_step(struct NDA *aut, char input)
{
    struct FSM_States *next = nda_states_step(aut->cur_states, *aut->spec, input);
    fsm_states_free(aut->cur_states);
    aut->cur_states = next;
}

struct FSM_States *nda_states_step(const struct FSM_States *states, const struct FSM_Spec spec, char input)
{
    struct FSM_States *stepped = fsm_states_create();
    for (int i = 0; i < fsm_states_count(states); ++i)
    {
        fsm_state_t state = fsm_states_at(states, i);
        struct FSM_States *to = spec.output[state][input - 'a'];
        if (to == NULL)
            continue;
        for (int j = 0; j < fsm_states_count(to); ++j)
        {
            fsm_state_t value = fsm_states_at(to, j);
            if (!fsm_states_contains(stepped, value))
                fsm_states_add(stepped, value);
        }
    }
    return stepped;
}

struct FSM_States *nda_get_states(const struct NDA *aut)
{
    return aut->cur_states;
}

bool nda_recognized(const struct NDA *aut)
{
    for (int i = 0; i < fsm_states_count(aut->cur_states); ++i)
        if (fsm_states_contains(aut->spec->fin_states, fsm_states_at(aut->cur_states, i)))
            return true;
    return false;
}