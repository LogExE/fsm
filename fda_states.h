#pragma once

typedef int state_t;

#define MAX_STATE_NUM 1024

typedef struct
{
    state_t *states;
    int count;
} FDA_States;

void fda_states_free(FDA_States *states);