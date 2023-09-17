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
    struct FSM_Array *automata = fsm_array_create();
    fsm_array_add(automata, fsm_create(&spec));

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

    int failed_count = 0;

    while (*input)
    {
        printf("Step %d:\n", input - inp + 1);
        printf("Passing character '%c'\n", *input);
        int aut_count = fsm_array_count(automata);
        for (int i = 0; i < aut_count; ++i)
        {
            printf("For automaton %d:\n", i);
            struct FSM *aut = fsm_array_at(automata, i);
            struct FSM_Array *new_aut = fsm_step(aut, *input);

            if (new_aut != NULL)
                for (int j = 0; j < fsm_array_count(new_aut); ++j)
                    fsm_array_add(automata, fsm_array_at(new_aut, j));
        }
        ++input;
    }
    for (int i = 0; i < fsm_array_count(automata); ++i)
    {
        struct FSM *aut = fsm_array_at(automata, i);
        if (fsm_get_output(aut) == FSM_RECOGNIZED)
            printf("Word has been recognized!\n");
        else
            printf("Word has not been recognized!\n");
    }

    fsm_array_free(automata);

    free(spec.alphabet);
    fsm_states_free(spec.states);
    fsm_states_free(spec.fin_states);
}