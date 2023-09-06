#include "fda_states.h"

#include <stdlib.h>

FDA_States fda_states_alloc(int count)
{
    FDA_States res;
    res.count = count;
    res.values = malloc(count * sizeof(state_t));
    return res;
}

void fda_states_free(FDA_States *states)
{
    free(states->values);
}