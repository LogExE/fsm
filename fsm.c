#include "fsm.h"

#include <stdlib.h>
#include <ctype.h>

struct FSM
{
    FSM_Spec *spec;

    FSM_Output output;
    int cur_state;
};

struct FSM *fsm_create(FSM_Spec *spec)
{
    struct FSM *ret = malloc(sizeof(struct FSM));
    ret->spec = spec;
    fsm_reset(ret);
    return ret;
}

void fsm_free(struct FSM *aut)
{
    free(aut);
}

static struct FSM_States *get_out_states(const struct FSM *aut, char input)
{
    return aut->spec->output[aut->cur_state][input];
}

void fsm_reset(struct FSM *aut)
{
    aut->cur_state = aut->spec->init_state;
    aut->output = fsm_spec_check_is_final(*aut->spec, aut->cur_state) ? FSM_RECOGNIZED : FSM_NEXT;
}

static void fsm_change(struct FSM *aut, fsm_state_t new_state)
{
    aut->cur_state = new_state;
    if (fsm_spec_check_is_final(*aut->spec, aut->cur_state))
        aut->output = FSM_RECOGNIZED;
    else
        aut->output = FSM_NEXT;
}

struct FSM_Array *fsm_step(struct FSM *aut, char input)
{
    if (aut->output == FSM_FAILED || !isalpha(input))
        return NULL;
    struct FSM_States *new_states = get_out_states(aut, input - 'a');
    if (new_states == NULL)
    {
        aut->output = FSM_FAILED;
        return NULL;
    }
    fsm_change(aut, fsm_states_at(new_states, 0));

    if (fsm_states_count(new_states) == 1)
        return NULL;

    struct FSM_Array *new_auts = fsm_array_create();
    for (int i = 1; i < fsm_states_count(new_states); ++i)
    {
        fsm_state_t new_state = fsm_states_at(new_states, i);
        struct FSM *new_aut = fsm_create(aut->spec);
        fsm_change(new_aut, new_state);
        fsm_array_add(new_auts, new_aut);
    }
    return new_auts;
}

fsm_state_t fsm_get_state(const struct FSM *aut)
{
    return aut->cur_state;
}

FSM_Output fsm_get_output(const struct FSM *aut)
{
    return aut->output;
}