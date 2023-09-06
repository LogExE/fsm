#pragma once

typedef int state_t;

#define FDA_MAX_STATE_NUM 1024

typedef struct
{
    state_t *values;
    int count;
} FDA_States;

FDA_States fda_states_alloc(int count);
void fda_states_free(FDA_States *states);