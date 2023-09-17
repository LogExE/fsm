#include "fsm_array.h"

#include "stdlib.h"

#include "fsm.h"

struct FSM_Array
{
    struct FSM **values;
    int count;
    int capacity;
};

struct FSM_Array *fsm_array_create()
{
    struct FSM_Array *ret = malloc(sizeof(struct FSM_Array));
    ret->capacity = 2;
    ret->values = malloc(ret->capacity * sizeof(struct FSM *));
    ret->count = 0;
    return ret;
}

void fsm_array_free(struct FSM_Array *arr)
{
    for (int i = 0; i < arr->count; ++i)
        fsm_free(arr->values[i]);
    free(arr);
}

void fsm_array_add(struct FSM_Array *arr, struct FSM *aut)
{
    if (arr->count >= arr->capacity)
    {
        arr->capacity *= 2;
        arr->values = realloc(arr->values, arr->capacity * sizeof(struct FSM *));
    }
    arr->values[arr->count] = aut;
    ++arr->count;
}

struct FSM *fsm_array_at(const struct FSM_Array *arr, int idx)
{
    return arr->values[idx];
}

int fsm_array_count(const struct FSM_Array *arr)
{
    return arr->count;
}
