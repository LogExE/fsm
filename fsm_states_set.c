#include "fsm_states_set.h"

#include "stdlib.h"

struct FSM_States_Set
{
    bool *values;
    int capacity;
};

struct FSM_States_Set *fsm_states_set_create()
{
    struct FSM_States_Set *ret = malloc(sizeof(struct FSM_States_Set));
    ret->capacity = 0;
    return ret;
}

void fsm_states_set_free(struct FSM_States_Set *set)
{
    free(set->values);
    free(set);
}

void fsm_states_set_add(struct FSM_States_Set *set, fsm_state_t state)
{
    if (set->capacity <= state)
    {
        int new_size = state + 1;
        if (set->capacity > 0)
            set->values = realloc(set->values, sizeof(bool) * new_size);
        else
            set->values = malloc(sizeof(bool) * new_size);
        for (int i = set->capacity; i < new_size; ++i)
            set->values[i] = false;
        set->capacity = new_size;
    }
    set->values[state] = true;
}

void fsm_states_set_remove(struct FSM_States_Set *set, fsm_state_t state)
{
    if (set->capacity > state)
        set->values[state] = false;
}

void fsm_states_set_reset(struct FSM_States_Set *set)
{
    set->capacity = 0;
    free(set->values);
}

bool fsm_states_set_contains(const struct FSM_States_Set *set, fsm_state_t state)
{
    if (set->capacity > state)
        return set->values[state];
    return false;
}

struct FSM_States *fsm_states_set_convert(const struct FSM_States_Set *set)
{
    struct FSM_States *ret = fsm_states_create();
    for (int i = 0; i < set->capacity; ++i)
        if (set->values[i])
            fsm_states_add(ret, i);
    return ret;
}
