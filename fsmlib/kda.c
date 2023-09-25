#include "kda.h"

#include <stdlib.h>
#include <ctype.h>

#include "fsm_spec.h"

struct KDA
{
    struct FSM_Spec *spec;
    fsm_state_t cur_state;
};

struct KDA *kda_create(struct FSM_Spec *spec)
{
    for (int i = 0; i < fsm_states_count(spec->states); ++i)
        for (char *alpha = spec->alphabet; *alpha != '\0'; ++alpha)
        {
            struct FSM_States *next = spec->output[i][*alpha - 'a'];
            if (next != NULL && fsm_states_count(next) > 1)
                return NULL;
        }
    struct KDA *ret = malloc(sizeof(struct KDA));
    ret->spec = spec;
    ret->cur_state = spec->init_state;
    return ret;
}

void kda_free(struct KDA *aut)
{
    free(aut);
}

void kda_reset(struct KDA *aut)
{
    aut->cur_state = aut->spec->init_state;
}

void kda_step(struct KDA *aut, char input)
{
    struct FSM_States *s = aut->spec->output[aut->cur_state][input - 'a'];
    if (fsm_states_count(s) == 1)
        aut->cur_state = fsm_states_at(s, 0);
    else
        aut->cur_state = KDA_STATE_FAILED;
}

bool kda_recognized(const struct KDA *aut)
{
    return fsm_states_contains(aut->spec->fin_states, aut->cur_state);
}

fsm_state_t kda_get_state(const struct KDA *aut)
{
    return aut->cur_state;
}