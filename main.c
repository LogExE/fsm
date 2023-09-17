#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fsm.h"

#define FSM_FILE "input.txt"

int main()
{
    FILE *file = fopen(FSM_FILE, "r");
    if (file == NULL)
    {
        fprintf(stderr, "File %s doesn't exist!\n", FSM_FILE);
        return 1;
    }
    FSM_Spec spec;
    bool read = fsm_spec_read_from(file, &spec);
    if (!read)
    {
        fprintf(stderr, "Failed to read FSM.\n");
        return 1;
    }
    fclose(file);

    printf("Read automaton:\n");
    fsm_spec_output(spec);
    fsm_t *test = fsm_create(&spec);
    fsm_reset(test);

    printf("At state %d\n", fsm_get_state(test));
    printf("Now enter string.\n");
    printf("> ");
    char inp[LINE_SIZE];
    char *input = fgets(inp, LINE_SIZE, stdin);
    if (input == NULL)
    {
        fprintf(stderr, "Got no input.\n");
        return 1;
    }
    inp[strcspn(inp, "\n")] = '\0';

    while (*input)
    {
        printf("Step %d:\n", input - inp + 1);
        printf("Passing character '%c'\n", *input);
        fsm_step(test, *input);
        state_t cur = fsm_get_state(test);
        if (fsm_get_output(test) == FSM_FAILED)
        {
            printf("Recognition failed!\n", input - inp + 1, *input);
            break;
        }
        printf("State -> %d\n", cur);
        ++input;
    }
    if (fsm_get_output(test) == FSM_RECOGNIZED)
        printf("Word has been recognized!\n");
    else
        printf("Word has not been recognized!\n");
    fsm_free(test);

    free(spec.alphabet);
    fsm_states_free(spec.states);
    fsm_states_free(spec.fin_states);
}