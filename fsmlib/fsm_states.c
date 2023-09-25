#include "fsm_states.h"

#include <stdlib.h>

struct FSM_States
{
    fsm_state_t *values;
    int count;
    int capacity;
};

struct FSM_States *fsm_states_create()
{
    struct FSM_States *ret = malloc(sizeof(struct FSM_States));
    ret->capacity = 2;
    ret->values = malloc(ret->capacity * sizeof(fsm_state_t));
    ret->count = 0;
    return ret;
}

void fsm_states_free(struct FSM_States *states)
{
    free(states->values);
}

void fsm_states_add(struct FSM_States *arr, fsm_state_t state)
{
    if (arr->count >= arr->capacity)
    {
        arr->capacity *= 2;
        arr->values = realloc(arr->values, arr->capacity * sizeof(fsm_state_t));
    }
    arr->values[arr->count] = state;
    ++arr->count;
}

fsm_state_t fsm_states_at(const struct FSM_States *states, int i)
{
    return states->values[i];
}

int fsm_states_count(const struct FSM_States *states)
{
    return states->count;
}
