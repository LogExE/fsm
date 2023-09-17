#include "fsm_states.h"

#include <stdlib.h>

struct FSM_States
{
    state_t *values;
    int count;
};

fsm_states_t *fsm_states_create(state_t *values, int count)
{
    fsm_states_t *res = malloc(sizeof(struct FSM_States));
    res->count = count;
    res->values = values;
    return res;
}

void fsm_states_free(fsm_states_t *states)
{
    free(states->values);
}

state_t fsm_states_at(const fsm_states_t *states, int i)
{
    return states->values[i];
}

int fsm_states_count(const fsm_states_t *states)
{
    return states->count;
}
