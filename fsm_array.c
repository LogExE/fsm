#include "fsm_array.h"

#include "stdlib.h"

struct FSM_Array
{
    fsm_t **values;
    int count;
    int capacity;
};

fsm_array_t *fsm_array_create()
{
    fsm_array_t *ret = malloc(sizeof(struct FSM_Array));
    ret->capacity = 2;
    ret->values = malloc(ret->capacity * sizeof(fsm_t *));
    ret->count = 0;
    return ret;
}

void fsm_array_free(fsm_array_t *arr)
{
    for (int i = 0; i < arr->count; ++i)
        fsm_free(arr->values[i]);
    free(arr);
}

void fsm_array_add(fsm_array_t *arr, fsm_t *aut)
{
    if (arr->count >= arr->capacity)
    {
        arr->capacity *= 2;
        arr->values = realloc(arr->values, arr->capacity * sizeof(struct FSM_Array));
    }
    arr->values[arr->count] = aut;
    ++arr->count;
}

fsm_t *fsm_array_at(const fsm_array_t *arr, int idx)
{
    return arr->values[idx];
}

int fsm_array_count(const fsm_array_t *arr)
{
    return arr->count;
}
