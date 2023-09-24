#include <stdio.h>
#include <stdlib.h>

#include "mydefs.h"
#include "fsm.h"
#include "fsm_array.h"

#define FSM_FILE "input.txt"

int main()
{
    FILE *file = fopen(FSM_FILE, "r");
    if (file == NULL)
    {
        fprintf(stderr, "File %s doesn't exist!\n", FSM_FILE);
        return 1;
    }
    struct FSM_Spec spec;
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
    while (1)
    {
        printf("NEXT WORD\n> ");
        int read = buf_readline(inp, stdin);
        if (read == 0)
        {
            printf("<exit>\n");
            break;
        }
        else if (read == -1)
        {
            printf("Stdin error!\n");
            return 1;
        }
        else if (read == -2)
        {
            printf("Input is too long!\n");
            return 1;
        }
        printf("Input length: %ld\n", read);
        automata = fsm_array_create();
        fsm_array_add(automata, fsm_create(&spec));

        char *input = inp;
        while (*input)
        {
            printf("==================\n");
            printf("Step %ld:\n", input - inp + 1);
            printf("Passing character '%c'\n", *input);
            fsm_array_input(automata, *input);
            printf("We have %d automata\n", fsm_array_count(automata));
            ++input;
        }
        printf("==================\n");
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
    free(spec.alphabet);
    fsm_states_free(spec.states);
    fsm_states_free(spec.fin_states);
}