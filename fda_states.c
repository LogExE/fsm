#include "fda_states.h"

#include <stdlib.h>

void fda_states_free(FDA_States *states)
{
    free(states->values);
}