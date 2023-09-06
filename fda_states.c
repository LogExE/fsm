#include "fda_states.h"

#include <stdlib.h>

FDA_States *fda_states_create(state_t *values, int count)
{
    FDA_States *res = malloc(sizeof(FDA_States));
    res->count = count;
    res->values = values;
    return res;
}

void fda_states_free(FDA_States *states)
{
    free(states->values);
}