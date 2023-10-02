#include "nda.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct NDA
{
    struct FSM_Spec *spec;
    struct FSM_States *cur_states;
};

struct NDA *nda_create(struct FSM_Spec *spec)
{
    if (fsm_spec_eps(*spec))
        return NULL;
    struct NDA *ret = malloc(sizeof(struct NDA));
    ret->spec = spec;
    ret->cur_states = fsm_states_create();
    fsm_states_add(ret->cur_states, spec->init_state);
    return ret;
}

void nda_free(struct NDA *aut)
{
    fsm_states_free(aut->cur_states);
    free(aut);
}

void nda_reset(struct NDA *aut)
{
    fsm_states_reset(aut->cur_states);
    fsm_states_add(aut->cur_states, aut->spec->init_state);
}

void nda_step(struct NDA *aut, char input)
{
    struct FSM_States *next = nda_states_step(aut->cur_states, *aut->spec, input);
    fsm_states_free(aut->cur_states);
    aut->cur_states = next;
}

struct FSM_States *nda_states_step(const struct FSM_States *states, struct FSM_Spec spec, char input)
{
    struct FSM_States *stepped = fsm_states_create();
    for (int i = 0; i < fsm_states_count(states); ++i)
    {
        fsm_state_t state = fsm_states_at(states, i);
        struct FSM_States *to = spec.output[state][input - 'a'];
        for (int j = 0; j < fsm_states_count(to); ++j)
        {
            fsm_state_t value = fsm_states_at(to, j);
            if (!fsm_states_contains(stepped, value))
                fsm_states_add(stepped, value);
        }
    }
    return stepped;
}

struct FSM_States *nda_get_states(const struct NDA *aut)
{
    return aut->cur_states;
}

bool nda_recognized(const struct NDA *aut)
{
    for (int i = 0; i < fsm_states_count(aut->cur_states); ++i)
        if (fsm_states_contains(aut->spec->fin_states, fsm_states_at(aut->cur_states, i)))
            return true;
    return false;
}

struct FSM_Spec nda_convert_spec_to_kda(struct FSM_Spec spec)
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
        found:;
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
                fsm_states_add(ret.fin_states, i);
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
