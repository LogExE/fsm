#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "fsm_spec.h"
#include "nda.h"

struct FSM_Spec convert_nda_to_kda(struct FSM_Spec spec)
{
    struct FSM_Spec ret;

    ret.alphabet = malloc(strlen(spec.alphabet));
    strcpy(ret.alphabet, spec.alphabet);

    struct FSM_States *new_states_col[FSM_MAX_STATE_NUM];
    new_states_col[1] = fsm_states_create();
    fsm_states_add(new_states_col[1], spec.init_state);
    struct FSM_States *rules[FSM_MAX_STATE_NUM][FSM_ALPHABET_SIZE];
    for (fsm_state_t state = 0; state < FSM_MAX_STATE_NUM; ++state)
        for (int i = 0; i < FSM_ALPHABET_SIZE; ++i)
            rules[state][i] = NULL;
    int table_size = 1;
    for (int i = 1; i <= table_size; ++i)
        for (char *alpha = spec.alphabet; *alpha != '\0'; ++alpha)
        {
            int idx = *alpha - 'a';
            rules[i][idx] = nda_states_step(new_states_col[i], spec, *alpha);
            for (int j = 1; j <= table_size; ++j)
                if (fsm_states_alike(rules[i][idx], new_states_col[j]))
                    goto found;
            new_states_col[table_size + 1] = rules[i][idx];
            ++table_size;
        found:
        }

    ret.states = fsm_states_create();
    ret.fin_states = fsm_states_create();
    printf("New states:\n");
    for (int i = 1; i <= table_size; ++i)
    {
        printf("%d: {", i);
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
        fsm_states_add(ret.states, i);
    // Новые конечные состояния
    for (int i = 1; i <= table_size; ++i)
        for (int j = 0; j < fsm_states_count(spec.fin_states); ++j)
            if (fsm_states_contains(new_states_col[i], fsm_states_at(spec.fin_states, j)))
                fsm_states_add(ret.fin_states, i + 1);
    ret.init_state = 1;
    // Новые правила перехода
    for (fsm_state_t state = 0; state < FSM_MAX_STATE_NUM; ++state)
        for (int i = 0; i < FSM_ALPHABET_SIZE; ++i)
        {
            ret.output[state][i] = fsm_states_create();
            if (rules[state][i] == NULL)
                continue;
            int j = 1;
            while (!fsm_states_alike(rules[state][i], new_states_col[j]))
                ++j;
            fsm_states_add(ret.output[state][i], j);
        }
    return ret;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <file_in> <file_out>\n", argv[0]);
        return 1;
    }
    printf("Reading %s...\n", argv[1]);
    FILE *file_in = fopen(argv[1], "r");
    if (file_in == NULL)
    {
        fprintf(stderr, "file_in doesn't exist!\n");
        return 1;
    }
    struct FSM_Spec spec;
    fsm_spec_read_from(file_in, &spec);
    fclose(file_in);
    struct FSM_Spec new_spec = convert_nda_to_kda(spec);
    FILE *file_out = fopen(argv[2], "w");
    fsm_spec_write_to(file_out, new_spec);
    fclose(file_out);
    fsm_spec_free(spec);
    fsm_spec_free(new_spec);
    return 0;
}