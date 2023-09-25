#include "stdio.h"
#include "stdlib.h"

#include "fsm_spec.h"

void convert_write(struct FSM_Spec spec, FILE *file)
{
    fprintf(file, "%s\n", spec.alphabet);
    struct FSM_States *new_states_col[FSM_MAX_STATE_NUM];
    struct FSM_States *new_rules[FSM_MAX_STATE_NUM][FSM_ALPHABET_SIZE];
    new_states_col[0] = fsm_states_create();
    fsm_states_add(new_states_col[0], spec.init_state);
    int table_size = 1;
    for (int i = 0; i < table_size; ++i)
        for (char *alpha = spec.alphabet; *alpha != '\0'; ++alpha)
        {
            int j = *alpha - 'a';
            new_rules[i][j] = spec.output[i][j];
        }
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <file_in> <file_out>\n", argv[0]);
        return 1;
    }
    FILE *file_in = fopen(argv[1], "r");
    if (file_in == NULL)
    {
        fprintf(stderr, "file_in doesn't exist!\n");
        return 1;
    }
    struct FSM_Spec spec;
    fsm_spec_read_from(file_in, &spec);
    fclose(file_in);
    FILE *file_out = fopen(argv[2], "w");
    convert_write(spec, file_out);
    fclose(file_out);
    free(spec.alphabet);
    fsm_states_free(spec.states);
    fsm_states_free(spec.fin_states);
    return 0;
}