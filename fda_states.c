#include "fda_states.h"

#include <stdlib.h>

struct FDA_States
{
    state_t *values;
    int count;
};

fda_states_t fda_states_create(state_t *values, int count)
{
    fda_states_t res = malloc(sizeof(struct FDA_States));
    res->count = count;
    res->values = values;
    return res;
}

void fda_states_free(fda_states_t states)
{
    free(states->values);
}

state_t fda_states_at(const fda_states_t states,int i)
{
    return states->values[i];
}

int fda_states_count(const fda_states_t states)
{
    return states->count;
}
