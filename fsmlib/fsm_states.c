#include "fsm_states.h"

#include <stdlib.h>

#include "fsm_spec.h"

struct FSM_States
{
    fsm_state_t *values;
    int count;
    int capacity;
};

struct FSM_States *fsm_states_create()
{
    struct FSM_States *ret = malloc(sizeof(struct FSM_States));
    ret->capacity = 1;
    ret->count = 0;
    ret->values = malloc(ret->capacity * sizeof(fsm_state_t));
    return ret;
}

struct FSM_States *fsm_states_copy(const struct FSM_States *states)
{
    struct FSM_States *ret = malloc(sizeof(struct FSM_States));
    ret->capacity = states->capacity;
    ret->count = states->count;
    ret->values = malloc(ret->capacity * sizeof(fsm_state_t));
    for (int i = 0; i < states->count; ++i)
        ret->values[i] = states->values[i];
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

void fsm_states_add_uniq(struct FSM_States *arr, fsm_state_t state)
{
    if (!fsm_states_contains(arr, state))
        fsm_states_add(arr, state);
}

void fsm_states_remove(struct FSM_States *arr, int idx)
{
    if (arr->count <= idx)
        return;
    for (int i = idx; i < arr->count - 1; ++i)
        arr->values[i] = arr->values[i + 1];
    --arr->count;
}

void fsm_states_purge(struct FSM_States *arr, fsm_state_t state)
{
    for (int i = 0; arr->count;)
        if (fsm_states_at(arr, i) == state)
            fsm_states_remove(arr, i);
        else
            ++i;
}

void fsm_states_reset(struct FSM_States *arr)
{
    if (arr->count == 0)
        return;
    arr->capacity = 1;
    arr->count = 0;
    arr->values = malloc(arr->capacity * sizeof(fsm_state_t));
}

fsm_state_t fsm_states_at(const struct FSM_States *states, int i)
{
    return states->values[i];
}

int fsm_states_count(const struct FSM_States *states)
{
    return states->count;
}

bool fsm_states_contains(const struct FSM_States *states, fsm_state_t state)
{
    for (int i = 0; i < states->count; ++i)
        if (fsm_states_at(states, i) == state)
            return true;
    return false;
}

bool fsm_states_subset(const struct FSM_States *states1, const struct FSM_States *states2)
{
    for (int i = 0; i < states1->count; ++i)
        if (!fsm_states_contains(states2, fsm_states_at(states1, i)))
            return false;
    return true;
}

bool fsm_states_alike(const struct FSM_States *states1, const struct FSM_States *states2)
{
    return fsm_states_subset(states1, states2) && fsm_states_subset(states2, states1);
}