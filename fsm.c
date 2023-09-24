#include "fsm.h"

#include <stdlib.h>
#include <ctype.h>

#include "fsm_array.h"

#define FSM_STATE_FAILED 0

struct FSM
{
    struct FSM_Spec *spec;

    int cur_state;
};

struct FSM *fsm_create(struct FSM_Spec *spec)
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
    return aut->spec->output[aut->cur_state][input - 'a'];
}

void fsm_reset(struct FSM *aut)
{
    aut->cur_state = aut->spec->init_state;
}

struct FSM_Array *fsm_step(struct FSM *aut, char input)
{
    if (aut->cur_state == FSM_STATE_FAILED || !isalpha(input))
        return NULL;
    struct FSM_States *new_states = get_out_states(aut, input);
    if (new_states == NULL)
    {
        aut->cur_state = FSM_STATE_FAILED;
        return NULL;
    }
    aut->cur_state = fsm_states_at(new_states, 0);

    if (fsm_states_count(new_states) == 1)
        return NULL;

    struct FSM_Array *new_auts = fsm_array_create();
    for (int i = 1; i < fsm_states_count(new_states); ++i)
    {
        fsm_state_t new_state = fsm_states_at(new_states, i);
        struct FSM *new_aut = fsm_create(aut->spec);
        aut->cur_state = new_state;
        fsm_array_add(new_auts, new_aut);
    }
    return new_auts;
}

fsm_state_t fsm_get_state(const struct FSM *aut)
{
    return aut->cur_state;
}

enum FSM_Output fsm_get_output(const struct FSM *aut)
{
    if (aut->cur_state == FSM_STATE_FAILED)
        return FSM_FAILED;
    else if (fsm_spec_check_is_final(*aut->spec, aut->cur_state))
        return FSM_RECOGNIZED;
    else
        return FSM_NEXT;
}