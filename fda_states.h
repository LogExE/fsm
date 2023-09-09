#pragma once

typedef int state_t;

#define FDA_MAX_STATE_NUM 1024

typedef struct FDA_States fda_states_t;

fda_states_t* fda_states_create(state_t *values, int count);
void fda_states_free(fda_states_t *states);

state_t fda_states_at(const fda_states_t *states, int i);
int fda_states_count(const fda_states_t *states);