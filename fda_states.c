#include "fda_states.h"

void fda_states_free(FDA_States *states)
{
    free(states->states);
}