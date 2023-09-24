#include <stdio.h>
#include <stdlib.h>

#include "mydefs.h"
#include "fsm.h"
#include "fsm_states_set.h"

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
    struct FSM *aut = fsm_create(&spec);

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
        printf("Input length: %d\n", read);

        char *input = inp;
        while (*input)
        {
            printf("==================\n");
            printf("Step %ld:\n", input - inp + 1);
            printf("Passing character '%c'\n", *input);
            fsm_step(aut, *input);
            struct FSM_States_Set *states = fsm_get_states(aut);
            struct FSM_States *states_out = fsm_states_set_convert(states);
            printf("At states: ");
            for (int i = 0; i < fsm_states_count(states_out); ++i)
                printf("%d ", fsm_states_at(states_out, i));
            printf("\n");
            fsm_states_free(states_out);
            ++input;
        }
        printf("==================\n");
        printf("Result: ");
        if (fsm_get_output(aut) == FSM_RECOGNIZED)
            printf("%sWord has been recognized!\n%s", ANSI_GREEN, ANSI_RESET);
        else
            printf("%sWord has not been recognized!\n%s", ANSI_RED, ANSI_RESET);

        fsm_reset(aut);
        printf("Automaton has been reset.\n");
    }
    free(spec.alphabet);
    fsm_states_free(spec.states);
    fsm_states_free(spec.fin_states);
}