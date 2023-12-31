#include <stdio.h>
#include <stdlib.h>

#include "bufreadline.h"
#include "kda.h"
#include "nda.h"
#include "nda_eps.h"

#define FSM_SPEC_FILE_VAR "FSM_SPEC_FILE"
#define FSM_SPEC_FILE_DEFAULT "input.txt"

#define ANSI_GREEN "\033[0;32m"
#define ANSI_RED "\033[0;31m"
#define ANSI_YELLOW "\033[0;33m"
#define ANSI_RESET "\033[0m"

#if defined(TYPE_KDA)
#define FSM_STRING "KDA"
#define FSM KDA
#define fsm_create kda_create
#define fsm_step kda_step
#define fsm_recognized kda_recognized
#define fsm_reset kda_reset
#define print_fsm_info print_kda_info
#elif defined(TYPE_NDA)
#define FSM_STRING "NDA"
#define FSM NDA
#define fsm_create nda_create
#define fsm_step nda_step
#define fsm_recognized nda_recognized
#define fsm_reset nda_reset
#define print_fsm_info print_nda_info
#else
#define FSM_STRING "NDA_EPS"
#define FSM NDA_Eps
#define fsm_create nda_eps_create
#define fsm_step nda_eps_step
#define fsm_recognized nda_eps_recognized
#define fsm_reset nda_eps_reset
#define print_fsm_info print_nda_eps_info
#endif

void print_kda_info(struct KDA *aut)
{
    printf("At state: ");
    fsm_state_t state = kda_get_state(aut);
    if (state == KDA_STATE_FAILED)
        printf("KDA_STATE_FAILED");
    else
        printf("%d", state);
    printf("\n");
}

void print_nda_info(struct NDA *aut)
{
    struct FSM_States *states = nda_get_states(aut);
    printf("At states: {");
    int cnt = fsm_states_count(states);
    for (int i = 0; i < cnt; ++i)
    {
        printf("%d", fsm_states_at(states, i));
        if (i < cnt - 1)
            printf(", ");
    }
    printf("}\n");
}

void print_nda_eps_info(struct NDA_Eps *aut)
{
    struct FSM_States *states = nda_eps_get_states(aut);
    printf("At states: {");
    int cnt = fsm_states_count(states);
    for (int i = 0; i < cnt; ++i)
    {
        printf("%d", fsm_states_at(states, i));
        if (i < cnt - 1)
            printf(", ");
    }
    printf("}\n");
}

int main(void)
{
    printf("Compiled for type %s\n", FSM_STRING);
    printf("Reading environment variable \"%s\"...\n", FSM_SPEC_FILE_VAR);
    const char *spec_file = getenv(FSM_SPEC_FILE_VAR);
    if (!spec_file)
    {
        printf("Environment variable \"%s\" doesn't exist.\n", FSM_SPEC_FILE_VAR);
        spec_file = FSM_SPEC_FILE_DEFAULT;
    }
    printf("Using path %s\n", spec_file);
    FILE *file = fopen(spec_file, "r");
    if (file == NULL)
    {
        fprintf(stderr, "File %s doesn't exist!\n", spec_file);
        return 1;
    }
    struct FSM_Spec spec;
    printf("Reading automata specification...\n");
    bool read = fsm_spec_read_from(file, &spec);
    if (!read)
    {
        fprintf(stderr, "Failed to read FSM.\n");
        return 1;
    }
    fclose(file);

    printf("Read specification:\n");
    fsm_spec_output(spec);
    struct FSM *aut = fsm_create(&spec);
    if (aut == NULL)
    {
        fprintf(stderr, "This automata specification can't be used to create FSM of specified type!\n");
        return 1;
    }

    char inp[LINE_SIZE];
    while (1)
    {
        print_fsm_info(aut);
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
            fsm_step(aut, *input);
            print_fsm_info(aut);
            ++input;
        }
        printf("==================\n");
        printf("Result: ");
        if (fsm_recognized(aut))
            printf("%sWord has been recognized!\n%s", ANSI_GREEN, ANSI_RESET);
        else
            printf("%sWord has not been recognized!\n%s", ANSI_RED, ANSI_RESET);

        fsm_reset(aut);
        printf("Automaton has been reset.\n");
    }
    fsm_spec_free(spec);
}