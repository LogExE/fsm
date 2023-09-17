#include "fsm_spec.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool fsm_spec_check_is_final(FSM_Spec spec, fsm_state_t state)
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
    struct FSM_States *fsm_states = fsm_states_create();
    struct FSM_States *fsm_fin_states = fsm_states_create();
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
        int state_cnt = atoi(buf + 1);
        for (int i = 1; i <= state_cnt; ++i)
            fsm_states_add(fsm_states, i);
    }
    else
    {
        char *seek = buf;
        char *end = NULL;
        while (1)
        {
            fsm_state_t state = strtol(seek, &end, 10);
            if (seek == end)
                break;
            seek = end;
            fsm_states_add(fsm_states, state);
        }
    }
    printf("total states found: %d\n", fsm_states_count(fsm_states));

    // Читаем финальные состояния
    fgets(buf, LINE_SIZE, stream);
    buf_len = strlen(buf);

    {
        char *seek = buf;
        char *end = NULL;
        while (1)
        {
            fsm_state_t fin_state = strtol(seek, &end, 10);
            if (seek == end)
                break;
            seek = end;
            fsm_states_add(fsm_fin_states, fin_state);
        }
    }
    printf("total final states found:  %d\n", fsm_states_count(fsm_fin_states));

    // Читаем начальное состояние
    fgets(buf, LINE_SIZE, stream);
    fsm_init_state = atoi(buf);
    printf("initial state: %d\n", fsm_init_state);

    fsm_state_t rules_in[FILE_MAX_RULES_COUNT];
    char rules_sym[FILE_MAX_RULES_COUNT];
    struct FSM_States *rules_out[FILE_MAX_RULES_COUNT];
    int rules_cnt = 0;

    // До конца файла ищем правила переходов
    while (fgets(buf, LINE_SIZE, stream))
    {
        char *seek = buf;
        char *end = NULL;
        // состояние до
        rules_in[rules_cnt] = strtol(seek, &end, 10);
        seek = end + 1; // пропуск пробела
        // вход
        rules_sym[rules_cnt] = *seek - 'a';
        ++seek;
        // следующие состояния
        rules_out[rules_cnt] = fsm_states_create();
        seek = strtok(seek, ",");
        while (seek != NULL)
        {
            fsm_states_add(rules_out[rules_cnt], atoi(seek));
            seek = strtok(NULL, ",");
        }
        printf("found rule: (%d, %c) -> ",
               rules_in[rules_cnt],
               rules_sym[rules_cnt] + 'a');
        for (int i = 0; i < fsm_states_count(rules_out[rules_cnt]); ++i)
            printf("%d ", fsm_states_at(rules_out[rules_cnt], i));
        printf("\n");
        ++rules_cnt;
    }

    printf("good.\n");

    spec->alphabet = fsm_alphabet;
    spec->states = fsm_states;
    spec->fin_states = fsm_fin_states;
    spec->init_state = fsm_init_state;

    for (fsm_state_t state = 0; state < FSM_MAX_STATE_NUM; ++state)
        for (int i = 0; i < FSM_ALPHABET_SIZE; ++i)
            spec->output[state][i] = NULL;
    for (int i = 0; i < rules_cnt; ++i)
        spec->output[rules_in[i]][rules_sym[i]] = rules_out[i];

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
        fsm_state_t state = fsm_states_at(spec.states, i);
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
        else
            printf("    ");
        printf("%d|", state);
        seek = spec.alphabet;
        while (*seek)
        {
            struct FSM_States *to_print = spec.output[state][*seek++ - 'a'];
            if (to_print == NULL)
                printf("-");
            else
                for (int j = 0; j < fsm_states_count(to_print); ++j)
                    printf("%d ", fsm_states_at(to_print, j));
            printf("|");
        }
        printf("\n");
    }
    printf(OUTPUT_DELIM);
}