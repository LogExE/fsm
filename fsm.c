#include "fsm.h"

#include <stdlib.h>
#include <ctype.h>

struct FSM
{
    FSM_Spec *spec;

    FSM_Output output;
    int cur_state;
};

fsm_t *fsm_create(FSM_Spec *spec)
{
    fsm_t *res = malloc(sizeof(struct FSM));
    res->spec = spec;
    return res;
}

void fsm_free(fsm_t *aut)
{
    free(aut);
}

static state_t get_out_state(const fsm_t *aut, char input)
{
    return aut->spec->output[aut->cur_state][input];
}

void fsm_reset(fsm_t *aut)
{
    aut->cur_state = aut->spec->init_state;
    aut->output = fsm_spec_check_is_final(*aut->spec, aut->cur_state) ? FSM_RECOGNIZED : FSM_NEXT;
}

void fsm_step(fsm_t *aut, char input)
{
    if (!isalpha(input))
        return;
    state_t new_state = get_out_state(aut, input - 'a');
    if (new_state != FSM_OUTPUT_STATE_NONE)
    {
        aut->cur_state = new_state;
        if (fsm_spec_check_is_final(*aut->spec, aut->cur_state))
            aut->output = FSM_RECOGNIZED;
        else
            aut->output = FSM_NEXT;
    }
    else
        aut->output = FSM_FAILED;
}

state_t fsm_get_state(const fsm_t *aut)
{
    return aut->cur_state;
}

FSM_Output fsm_get_output(const fsm_t *aut)
{
    return aut->output;
}