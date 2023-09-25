#include <stdio.h>
#include <stdlib.h>

#include "bufreadline.h"
#include "nda.h"
#include "kda.h"

#define FSM_FILE "input.txt"

#define ANSI_GREEN "\033[0;32m"
#define ANSI_RED "\033[0;31m"
#define ANSI_YELLOW "\033[0;33m"
#define ANSI_RESET "\033[0m"

int main(void)
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

    printf("Read spec:\n");
    fsm_spec_output(spec);
    struct KDA *aut = kda_create(&spec);
    if (aut == NULL)
    {
        fprintf(stderr, "This automata specification can't be used to create KDA!\n");
        return 1;
    }

    char inp[LINE_SIZE];
    while (1)
    {
        printf("NEXT WORD\n> ");
        int read = buf_readline(inp, stdin);
        if (read == BUF_RL_EOF)
        {
            printf("<exit>\n");
            break;
        }
        else if (read == BUF_RL_ERR)
        {
            printf("Stdin error!\n");
            return 1;
        }
        else if (read == BUF_RL_TOOMANY)
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
            kda_step(aut, *input);
            printf("At state: %d\n", kda_get_state(aut));
            ++input;
        }
        printf("==================\n");
        printf("Result: ");
        if (kda_recognized(aut))
            printf("%sWord has been recognized!\n%s", ANSI_GREEN, ANSI_RESET);
        else
            printf("%sWord has not been recognized!\n%s", ANSI_RED, ANSI_RESET);

        kda_reset(aut);
        printf("Automaton has been reset.\n");
    }
    free(spec.alphabet);
    fsm_states_free(spec.states);
    fsm_states_free(spec.fin_states);
}