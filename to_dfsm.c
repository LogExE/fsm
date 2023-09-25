#include "stdio.h"
#include "stdlib.h"

#include "fsm_spec.h"

void convert_write(struct FSM_Spec spec, FILE *file)
{
    fprintf(file, "%s\n", spec.alphabet);
    struct FSM_States *new_states_col[FSM_MAX_STATE_NUM];
    new_states_col[0] = fsm_states_create();
    fsm_states_add(new_states_col[0], spec.init_state);
    struct FSM_States *new_rules[FSM_MAX_STATE_NUM][FSM_ALPHABET_SIZE];
    struct FSM_States *fin_states = fsm_states_create();
    int table_size = 1;
    for (int i = 0; i < table_size; ++i)
        for (char *alpha = spec.alphabet; *alpha != '\0'; ++alpha)
        {
            int idx = *alpha - 'a';
            new_rules[i][idx] = fsm_states_step(new_states_col[i], spec, *alpha);
            for (int j = 0; j < table_size; ++j)
                if (fsm_states_alike(new_rules[i][idx], new_states_col[j]))
                    goto found;
            new_states_col[table_size] = new_rules[i][idx];
            ++table_size;
        found:
        }
    printf("New states:\n");
    for (int i = 0; i < table_size; ++i)
    {
        printf("%d: {", i + 1);
        int cnt = fsm_states_count(new_states_col[i]);
        for (int j = 0; j < cnt; ++j)
        {
            printf("%d", fsm_states_at(new_states_col[i], j));
            if (j < cnt - 1)
                printf(", ");
        }
        printf("}\n");
    }
    // Нумеруем новые состояния
    for (int i = 1; i <= table_size; ++i)
        fprintf(file, "%d ", i);
    fprintf(file, "\n");
    // Новые конечные состояния
    for (int i = 0; i < table_size; ++i)
        for (int j = 0; j < fsm_states_count(spec.fin_states); ++j)
            if (fsm_states_contains(new_states_col[i], fsm_states_at(spec.fin_states, j)))
                fprintf(file, "%d ", i + 1);
    fprintf(file, "\n");
    // Новое начальное состояние - всегда 1
    fprintf(file, "1\n");
    // Новые правила перехода
    for (int i = 0; i < table_size; ++i)
        for (char *alpha = spec.alphabet; *alpha != '\0'; ++alpha)
        {
            int idx = *alpha - 'a';
            int j = 0;
            while (!fsm_states_alike(new_states_col[j], new_rules[i][idx]))
                ++j;
            fprintf(file, "%d %c %d\n", i + 1, *alpha, j + 1);
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