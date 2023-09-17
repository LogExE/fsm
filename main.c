#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fsm.h"
#include "mydefs.h"

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
    struct FSM_Array *automata;

    char inp[LINE_SIZE];
    char *input;
    while (1)
    {
        printf("NEXT WORD\n> ");
        if ((input = fgets(inp, LINE_SIZE, stdin)) == NULL)
        {
            printf("<exit>\n");
            break;
        }
        automata = fsm_array_create();
        fsm_array_add(automata, fsm_create(&spec));
        inp[strcspn(inp, "\n")] = '\0';

        int failed_count = 0;

        while (*input)
        {
            printf("========\n");
            printf("Step %d:\n", input - inp + 1);
            printf("Passing character '%c'\n", *input);
            int aut_count = fsm_array_count(automata);
            for (int i = 0; i < aut_count; ++i)
            {
                printf("For automaton %d:\n", i);
                struct FSM *aut = fsm_array_at(automata, i);
                struct FSM_Array *new_aut = fsm_step(aut, *input);

                if (new_aut != NULL)
                {
                    for (int j = 0; j < fsm_array_count(new_aut); ++j)
                        fsm_array_add(automata, fsm_array_at(new_aut, j));
                    printf("Cloned, new %d automata\n", fsm_array_count(new_aut));
                }
                else
                    printf("No new automata, state is now %d\n", fsm_get_state(aut));
            }
            ++input;
        }
        printf("========\n");
        printf("Result: ");
        for (int i = 0; i < fsm_array_count(automata); ++i)
        {
            struct FSM *aut = fsm_array_at(automata, i);
            if (fsm_get_output(aut) == FSM_RECOGNIZED)
            {
                printf("%sWord has been recognized!\n%s", ANSI_GREEN, ANSI_RESET);
                goto next;
            }
        }
        printf("%sWord has not been recognized!\n%s", ANSI_RED, ANSI_RESET);
    next:
        fsm_array_free(automata);
    }
cleanup:

    free(spec.alphabet);
    fsm_states_free(spec.states);
    fsm_states_free(spec.fin_states);
}