#include "fsm.h"

#include <stdlib.h>
#include <ctype.h>

#include "fsm_states_set.h"

struct FSM
{
    struct FSM_Spec *spec;

    struct FSM_States_Set *cur_states;
};

struct FSM *fsm_create(struct FSM_Spec *spec)
{
    struct FSM *ret = malloc(sizeof(struct FSM));
    ret->spec = spec;
    ret->cur_states = fsm_states_set_create();
    fsm_states_set_add(ret->cur_states, spec->init_state);
    return ret;
}

void fsm_free(struct FSM *aut)
{
    fsm_states_set_free(aut->cur_states);
    free(aut);
}

void fsm_reset(struct FSM *aut)
{
    fsm_states_set_reset(aut->cur_states);
    fsm_states_set_add(aut->cur_states, aut->spec->init_state);
}

void fsm_step(struct FSM *aut, char input)
{
    struct FSM_States *cur = fsm_states_set_convert(aut->cur_states);
    fsm_states_set_reset(aut->cur_states);
    for (int i = 0; i < fsm_states_count(cur); ++i)
    {
        fsm_state_t state = fsm_states_at(cur, i);
        struct FSM_States *to = aut->spec->output[state][input - 'a'];
        if (to == NULL)
            continue;
        for (int j = 0; j < fsm_states_count(to); ++j)
            fsm_states_set_add(aut->cur_states, fsm_states_at(to, j));
    }
    fsm_states_free(cur);
}

struct FSM_States_Set *fsm_get_states(const struct FSM *aut)
{
    return aut->cur_states;
}

enum FSM_Output fsm_get_output(const struct FSM *aut)
{
    for (int i = 0; i < fsm_states_count(aut->spec->fin_states); ++i)
    {
        fsm_state_t fin_state = fsm_states_at(aut->spec->fin_states, i);
        if (fsm_states_set_contains(aut->cur_states, fin_state))
            return FSM_RECOGNIZED;
    }
    for (int i = 0; i < fsm_states_count(aut->spec->states); ++i)
    {
        fsm_state_t state = fsm_states_at(aut->spec->states, i);
        if (fsm_states_set_contains(aut->cur_states, state))
            return FSM_NEXT;
    }
    return FSM_FAILED;
}