#include "fda_spec.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

FDA_Spec *fda_spec_create(char *alphabet, FDA_States *allowed_states, FDA_States *final_states, state_t initial_state)
{
    FDA_Spec *res = malloc(sizeof(FDA_Spec));
    res->alphabet = alphabet;
    res->states = allowed_states;
    res->fin_states = final_states;
    res->init_state = initial_state;
    for (int i = 0; i < FDA_MAX_STATE_NUM; ++i)
        for (int j = 0; j < FDA_ALPHABET_SIZE; ++j)
            res->output[i][j] = FDA_OUTPUT_STATE_NONE;
    return res;
}

void fda_spec_add_rule(FDA_Spec *spec, state_t state, char input, state_t res_state)
{
    spec->output[state][input] = res_state;
}

void fda_spec_free(FDA_Spec *spec)
{
    free(spec->alphabet);
    fda_states_free(spec->states);
    fda_states_free(spec->fin_states);
    free(spec);
}

FDA_Spec *fda_spec_read_from(FILE *stream)
{
    // Заготовочка переменных
    char *fda_alphabet = NULL;
    int alph_cnt = 0;
    state_t *fda_states = NULL;
    int state_cnt = 0;
    state_t *fda_fin_states = NULL;
    int fin_state_cnt = 0;
    int init_state = -1;

    // Буфер для считывания строк потока
    char buf[LINE_SIZE];
    int buf_len;

    // Читаем алфавит
    fgets(buf, LINE_SIZE, stream);
    buf_len = strlen(buf);
    if (buf[buf_len - 1] != '\n')
    {
        fprintf(stderr, "Line was too long!\n");
        return NULL;
    }
    buf[buf_len - 1] = '\0';

    if (buf[0] == '!')
    {
        alph_cnt = atoi(buf + 1);
        if (alph_cnt > FDA_ALPHABET_SIZE)
        {
            fprintf(stderr, "Expected alphabet size <= %d, found %d symbols!\n", FDA_ALPHABET_SIZE, alph_cnt);
            return NULL;
        }
        fda_alphabet = malloc(alph_cnt + 1);
        for (int i = 0; i < alph_cnt; ++i)
            fda_alphabet[i] = 'a' + i;
    }
    else
    {
        char *ptr = buf;
        while (*ptr)
        {
            if (!isalpha(*ptr))
            {
                fprintf(stderr, "Expected alphabet to be english alphabet, found symbol '%c'!\n", *ptr);
                return NULL;
            }
            ++ptr;
        }
        alph_cnt = ptr - buf + 1;
        fda_alphabet = malloc(alph_cnt + 1);
        memcpy(fda_alphabet, buf, alph_cnt);
    }
    fda_alphabet[alph_cnt] = '\0';
    printf("Alphabet: \"%s\", size %d\n", fda_alphabet, alph_cnt);

    // Читаем возможные состояния автомата
    fgets(buf, LINE_SIZE, stream);
    buf_len = strlen(buf);

    if (buf[0] == '!')
    {
        state_cnt = atoi(buf + 1);
        fda_states = malloc(state_cnt * sizeof(state_t));
        for (int i = 0; i < state_cnt; ++i)
            fda_states[i] = i + 1;
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
        fda_states = malloc(state_cnt * sizeof(state_t));
        for (int i = 0; i < state_cnt; ++i)
            fda_states[i] = pre_states[i];
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
        fda_fin_states = malloc(fin_state_cnt * sizeof(state_t));
        for (int i = 0; i < fin_state_cnt; ++i)
            fda_fin_states[i] = pre_states[i];
    }
    printf("total final states found:  %d\n", fin_state_cnt);

    // Читаем начальное состояние
    fgets(buf, LINE_SIZE, stream);
    init_state = atoi(buf);
    printf("initial state: %d\n", init_state);

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

    FDA_States *st = fda_states_create(fda_states, state_cnt);
    FDA_States *st_fin = fda_states_create(fda_fin_states, fin_state_cnt);
    FDA_Spec *spec = fda_spec_create(fda_alphabet, st, st_fin, init_state);

    for (int i = 0; i < rules_cnt; ++i)
        fda_spec_add_rule(spec, rules[i], rules_sym[i], rules[FILE_MAX_RULES_COUNT + i]);

    return spec;
}