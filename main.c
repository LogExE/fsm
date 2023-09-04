#include <stdio.h>
#include <stdlib.h>

#include "fda.h"

int main()
{
    FDA test;
    fda_set_alphabet(&test, "xyz");
    // состояния 0 1 2 3
    FDA_States st1;
    st1.states = malloc(4 * sizeof(state_t));
    st1.count = 4;
    for (int i = 0; i < 4; ++i)
        st1.states[i] = i;
    fda_set_states(&test, st1);
    // 3
    FDA_States st2;
    st2.states = malloc(1 * sizeof(state_t));
    st2.count = 1;
    st2.states[0] = 3;
    fda_set_final_states(&test, st2);
    fda_set_initial_state(&test, 0);

    fda_add_rule(&test, 0, 'x', 1);
    fda_add_rule(&test, 1, 'y', 2);
    fda_add_rule(&test, 2, 'z', 3);

    fda_reset(&test);
    const char *our_input = "xyz";
    FDA_Result res;
    do
    {
        res = fda_step(&test, *our_input++);
        printf("res == %d\n", res);
    } while (res == NEXT);
}