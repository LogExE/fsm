#include "fsm_spec.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool fsm_spec_check_is_final(FSM_Spec spec, state_t state)
{
    for (int i = 0; i < fsm_states_count(spec.fin_states); ++i)
        if (fsm_states_at(spec.fin_states, i) == state)
            return true;
    return false;
}

bool fsm_spec_read_from(FILE *stream, FSM_Spec *spec)
{
    // Заготовочка переменных
    char *fsm_alphabet;
    int alph_cnt = 0;
    state_t *fsm_states = NULL;
    int state_cnt = 0;
    state_t *fsm_fin_states = NULL;
    int fin_state_cnt = 0;
    int fsm_init_state;

    // Буфер для считывания строк потока
    char buf[LINE_SIZE];
    int buf_len;

    // Читаем алфавит
    fgets(buf, LINE_SIZE, stream);
    buf_len = strlen(buf);
    if (buf[buf_len - 1] != '\n')
    {
        fprintf(stderr, "Line was too long!\n");
        return false;
    }
    buf[buf_len - 1] = '\0';

    if (buf[0] == '!')
    {
        alph_cnt = atoi(buf + 1);
        if (alph_cnt > FSM_ALPHABET_SIZE)
        {
            fprintf(stderr, "Expected alphabet size <= %d, found %d symbols!\n", FSM_ALPHABET_SIZE, alph_cnt);
            return false;
        }
        fsm_alphabet = malloc(alph_cnt + 1);
        for (int i = 0; i < alph_cnt; ++i)
            fsm_alphabet[i] = 'a' + i;
    }
    else
    {
        char *ptr = buf;
        while (*ptr)
        {
            if (!isalpha(*ptr))
            {
                fprintf(stderr, "Expected alphabet to be english alphabet, found symbol '%c'!\n", *ptr);
                return false;
            }
            ++ptr;
        }
        alph_cnt = ptr - buf + 1;
        fsm_alphabet = malloc(alph_cnt + 1);
        memcpy(fsm_alphabet, buf, alph_cnt);
    }
    fsm_alphabet[alph_cnt] = '\0';
    printf("Alphabet: \"%s\", size %d\n", fsm_alphabet, alph_cnt);

    // Читаем возможные состояния автомата
    fgets(buf, LINE_SIZE, stream);
    buf_len = strlen(buf);

    if (buf[0] == '!')
    {
        state_cnt = atoi(buf + 1);
        fsm_states = malloc(state_cnt * sizeof(state_t));
        for (int i = 0; i < state_cnt; ++i)
            fsm_states[i] = i + 1;
    }
    else
    {
        char *seek = buf;
        char *end = NULL;
        state_t pre_states[buf_len];
        while (1)
        {
            pre_states[state_cnt] = strtol(seek, &end, 10);
            if (seek == end)
                break;
            seek = end;
            ++state_cnt;
        }
        fsm_states = malloc(state_cnt * sizeof(state_t));
        for (int i = 0; i < state_cnt; ++i)
            fsm_states[i] = pre_states[i];
    }
    printf("total states found: %d\n", state_cnt);

    // Читаем финальные состояния
    fgets(buf, LINE_SIZE, stream);
    buf_len = strlen(buf);

    {
        char *seek = buf;
        char *end = NULL;
        state_t pre_states[buf_len];
        while (1)
        {
            pre_states[fin_state_cnt] = strtol(seek, &end, 10);
            if (seek == end)
                break;
            seek = end;
            ++fin_state_cnt;
        }
        fsm_fin_states = malloc(fin_state_cnt * sizeof(state_t));
        for (int i = 0; i < fin_state_cnt; ++i)
            fsm_fin_states[i] = pre_states[i];
    }
    printf("total final states found:  %d\n", fin_state_cnt);

    // Читаем начальное состояние
    fgets(buf, LINE_SIZE, stream);
    fsm_init_state = atoi(buf);
    printf("initial state: %d\n", fsm_init_state);

    state_t rules[2 * FILE_MAX_RULES_COUNT];
    char rules_sym[FILE_MAX_RULES_COUNT];
    int rules_cnt = 0;

    // До конца файла ищем правила переходов
    while (fgets(buf, LINE_SIZE, stream))
    {
        char *seek = buf;
        char *end = NULL;
        rules[rules_cnt] = strtol(seek, &end, 10);
        seek = end + 1; // пропуск пробела
        rules_sym[rules_cnt] = *seek - 'a';
        ++seek; // пропуск пробела
        rules[FILE_MAX_RULES_COUNT + rules_cnt] = strtol(seek, &end, 10);
        printf("found rule: (%d, %c) -> %d\n",
               rules[rules_cnt],
               rules_sym[rules_cnt] + 'a',
               rules[FILE_MAX_RULES_COUNT + rules_cnt]);
        ++rules_cnt;
    }

    printf("good.\n");

    spec->alphabet = fsm_alphabet;
    spec->states = fsm_states_create(fsm_states, state_cnt);
    spec->fin_states = fsm_states_create(fsm_fin_states, fin_state_cnt);
    spec->init_state = fsm_init_state;

    for (state_t state; state < FSM_MAX_STATE_NUM; ++state)
        for (int i = 0; i < FSM_ALPHABET_SIZE; ++i)
            spec->output[state][i] = FSM_OUTPUT_STATE_NONE;
    for (int i = 0; i < rules_cnt; ++i)
        spec->output[rules[i]][rules_sym[i]] = rules[FILE_MAX_RULES_COUNT + i];

    return true;
}

#define OUTPUT_DELIM "##########\n"

void fsm_spec_output(FSM_Spec spec)
{
    printf(OUTPUT_DELIM);
    printf("     |");
    char *seek = spec.alphabet;
    while (*seek)
        printf("%c|", *seek++);
    printf("\n");
    for (int i = 0; i < fsm_states_count(spec.states); ++i)
    {
        state_t state = fsm_states_at(spec.states, i);
        bool initial = state == spec.init_state;
        bool final = fsm_spec_check_is_final(spec, state);
        if (initial || final)
        {
            if (initial && final)
                printf("->* ");
            else if (initial)
                printf("->  ");
            else
                printf("*   ");
        }
        else printf("    ");
        printf("%d|", state);
        seek = spec.alphabet;
        while (*seek)
        {
            state_t to_print = spec.output[state][*seek++ - 'a'];
            if (to_print == FSM_OUTPUT_STATE_NONE)
                printf("-");
            else
                printf("%d", to_print);
            printf("|");
        }
        printf("\n");
    }
    printf(OUTPUT_DELIM);
}