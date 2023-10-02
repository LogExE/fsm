#include "fsm_spec.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "bufreadline.h"

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

bool fsm_spec_check_is_final(struct FSM_Spec spec, fsm_state_t state)
{
    for (int i = 0; i < fsm_states_count(spec.fin_states); ++i)
        if (fsm_states_at(spec.fin_states, i) == state)
            return true;
    return false;
}

bool fsm_spec_read_from(FILE *stream, struct FSM_Spec *spec)
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
    do
        buf_len = buf_readline(buf, stream);
    while (buf_len == 0);
    if (buf_len == BUF_RL_ERR)
    {
        fprintf(stderr, "Error while reading alphabet!\n");
        goto alpha_fail;
    }
    else if (buf_len == BUF_RL_TOOMANY)
    {
        fprintf(stderr, "Alphabet line was too long!\n");
        goto alpha_fail;
    }
    else if (buf_len == BUF_RL_EOF)
    {
        fprintf(stderr, "Alphabet line was EOF!\n");
        goto alpha_fail;
    }

    if (buf[0] == '!')
    {
        alph_cnt = atoi(buf + 1);
        if (alph_cnt > 26)
        {
            fprintf(stderr, "Expected alphabet size <= 26, found %d symbols!\n", alph_cnt);
            goto alpha_fail;
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
                goto alpha_fail;
            }
            ++ptr;
        }
        alph_cnt = ptr - buf;
        fsm_alphabet = malloc(alph_cnt + 1);
        memcpy(fsm_alphabet, buf, alph_cnt);
    }
    fsm_alphabet[alph_cnt] = '\0';

    // Читаем возможные состояния автомата
    do
        buf_len = buf_readline(buf, stream);
    while (buf_len == 0);
    if (buf_len == BUF_RL_ERR)
    {
        fprintf(stderr, "Error while reading states!\n");
        goto other_fail;
    }
    else if (buf_len == BUF_RL_TOOMANY)
    {
        fprintf(stderr, "States line was too long!\n");
        goto other_fail;
    }
    else if (buf_len == BUF_RL_EOF)
    {
        fprintf(stderr, "States line was EOF!\n");
        goto other_fail;
    }

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

    // Читаем финальные состояния
    do
        buf_len = buf_readline(buf, stream);
    while (buf_len == 0);
    if (buf_len == BUF_RL_ERR)
    {
        fprintf(stderr, "Error while reading final states!\n");
        goto other_fail;
    }
    else if (buf_len == BUF_RL_TOOMANY)
    {
        fprintf(stderr, "Final states line was too long!\n");
        goto other_fail;
    }
    else if (buf_len == BUF_RL_EOF)
    {
        fprintf(stderr, "Final states line was EOF!\n");
        goto other_fail;
    }

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

    // Читаем начальное состояние
    do
        buf_len = buf_readline(buf, stream);
    while (buf_len == 0);
    if (buf_len == BUF_RL_ERR)
    {
        fprintf(stderr, "Error while reading initial state!\n");
        goto other_fail;
    }
    else if (buf_len == BUF_RL_TOOMANY)
    {
        fprintf(stderr, "Initial state line was too long!\n");
        goto other_fail;
    }
    else if (buf_len == BUF_RL_EOF)
    {
        fprintf(stderr, "Initial state line was EOF!\n");
        goto other_fail;
    }

    fsm_init_state = atoi(buf);

    fsm_state_t rules_in[FILE_MAX_RULES_COUNT];
    char rules_sym[FILE_MAX_RULES_COUNT];
    struct FSM_States *rules_out[FILE_MAX_RULES_COUNT];
    int rules_cnt = 0;

    // До конца файла ищем правила переходов

    while ((buf_len = buf_readline(buf, stream)) >= 0)
    {
        if (buf_len == 0)
            continue;
        char *seek = buf;
        char *end = NULL;
        while (*seek && isblank(*seek))
            ++seek; // пропуск пробелов
        // состояние до
        rules_in[rules_cnt] = strtol(seek, &end, 10);
        if (seek == end)
        {
            fprintf(stderr, "Bad rule number %d! In-state was in wrong format!\n", rules_cnt + 1);
            goto other_fail;
        }
        seek = end;
        while (*seek && isblank(*seek))
            ++seek; // пропуск пробелов
        // вход
        int input_str_size = 0;
        while (isalpha(seek[input_str_size]))
            ++input_str_size;
        if (input_str_size == 0)
        {
            fprintf(stderr, "Bad rule number %d! Input was abscent!\n", rules_cnt + 1);
            goto other_fail;
        }
        else if (input_str_size == 1)
        {
            rules_sym[rules_cnt] = *seek;
        }
        else if (input_str_size == 3 && seek[0] == 'e' && seek[1] == 'p' && seek[2] == 's')
        {
            rules_sym[rules_cnt] = FSM_SYMBOL_EPS;
        }
        else
        {
            fprintf(stderr, "Bad rule number %d! Input was in wrong format!\n", rules_cnt + 1);
            goto other_fail;
        }
        seek += input_str_size;
        while (*seek && isblank(*seek))
            ++seek; // пропуск пробелов
        // следующие состояния
        rules_out[rules_cnt] = fsm_states_create();
        const char *delim = " ,";
        seek = strtok(seek, delim);
        while (seek != NULL)
        {
            int state = strtol(seek, &end, 10);
            if (seek == end)
            {
                fprintf(stderr, "Bad rule number %d! Some out-state was in wrong format!\n", rules_cnt + 1);
                goto other_fail;
            }
            fsm_states_add(rules_out[rules_cnt], state);
            seek = strtok(NULL, delim);
        }
        ++rules_cnt;
    }

    if (buf_len == BUF_RL_ERR)
    {
        fprintf(stderr, "Error while reading rule!\n");
        goto other_fail;
    }
    else if (buf_len == BUF_RL_TOOMANY)
    {
        fprintf(stderr, "Rule line was too long!\n");
        goto other_fail;
    }

    spec->alphabet = fsm_alphabet;
    spec->states = fsm_states;
    spec->fin_states = fsm_fin_states;
    spec->init_state = fsm_init_state;

    for (fsm_state_t state = 0; state < FSM_MAX_STATE_NUM; ++state)
        for (int i = 0; i < FSM_ALPHABET_SIZE; ++i)
            spec->output[state][i] = fsm_states_create();
    for (int i = 0; i < rules_cnt; ++i)
    {
        int idx = rules_sym[i] - 'a';
        fsm_states_free(spec->output[rules_in[i]][idx]);
        spec->output[rules_in[i]][idx] = rules_out[i];
    }
    return true;
other_fail:
    free(fsm_alphabet);
alpha_fail:
    fsm_states_free(fsm_states);
    fsm_states_free(fsm_fin_states);
    return false;
}

void fsm_spec_write_to(FILE *stream, struct FSM_Spec spec)
{
    // Алфавит
    fprintf(stream, "%s\n", spec.alphabet);
    // Множество состояний
    for (int i = 0; i < fsm_states_count(spec.states); ++i)
        fprintf(stream, "%d ", fsm_states_at(spec.states, i));
    fprintf(stream, "\n");
    // Финальные состояния
    for (int i = 0; i < fsm_states_count(spec.fin_states); ++i)
        fprintf(stream, "%d ", fsm_states_at(spec.fin_states, i));
    fprintf(stream, "\n");
    // Начальное состояние
    fprintf(stream, "%d\n", spec.init_state);
    // Переходы
    for (int i = 0; i < fsm_states_count(spec.states); ++i)
        for (char *alpha = spec.alphabet; *alpha != '\0'; ++alpha)
        {
            fsm_state_t state = fsm_states_at(spec.states, i);
            int idx = *alpha - 'a';
            struct FSM_States *to = spec.output[state][idx];
            if (fsm_states_count(to) > 0)
            {
                fprintf(stream, "%d ", state);
                if (*alpha == FSM_SYMBOL_EPS)
                    fprintf(stream, "eps ");
                else
                    fprintf(stream, "%c ", *alpha);
                int cnt = fsm_states_count(to);
                for (int j = 0; j < cnt; ++j)
                {
                    fprintf(stream, "%d", fsm_states_at(to, j));
                    if (j < cnt - 1)
                        fprintf(stream, ", ");
                }
                fprintf(stream, "\n");
            }
        }
}

void fsm_spec_free(struct FSM_Spec spec)
{
    for (fsm_state_t state = 0; state < FSM_MAX_STATE_NUM; ++state)
        for (int i = 0; i < FSM_ALPHABET_SIZE; ++i)
            fsm_states_free(spec.output[state][i]);
    fsm_states_free(spec.fin_states);
    fsm_states_free(spec.states);
    free(spec.alphabet);
}

#define OUTPUT_DELIM "##########\n"

static int int_digits(int x)
{
    int res = 1;
    while (x >= 10)
    {
        x /= 10;
        ++res;
    }
    return res;
}

static int get_max_len(struct FSM_Spec spec)
{
    int res = 0;
    for (int i = 0; i < fsm_states_count(spec.states); ++i)
    {
        fsm_state_t state = fsm_states_at(spec.states, i);
        char *seek = spec.alphabet;
        while (*seek)
        {
            struct FSM_States *to_print = spec.output[state][*seek++ - 'a'];
            if (to_print == NULL)
            {
                res = MAX(res, 1);
            }
            else
            {
                int sum = int_digits(fsm_states_at(to_print, 0));
                for (int j = 1; j < fsm_states_count(to_print); ++j)
                    sum += int_digits(fsm_states_at(to_print, j)) + 1;
                res = MAX(res, sum);
            }
        }
    }
    return res;
}

void fsm_spec_output(struct FSM_Spec spec)
{
    int spaces = get_max_len(spec) + 1;
    bool eps = fsm_spec_eps(spec);
    printf(OUTPUT_DELIM);
    printf("     |");
    char alpha_copy[FSM_ALPHABET_SIZE + 1];
    int alpha_copy_size = strlen(spec.alphabet);
    strcpy(alpha_copy, spec.alphabet);
    if (eps)
    {
        alpha_copy[alpha_copy_size] = FSM_SYMBOL_EPS;
        ++alpha_copy_size;
    }
    alpha_copy[alpha_copy_size] = '\0';
    char *seek = alpha_copy;
    while (*seek)
        printf("%*c|", spaces, *seek++);
    printf("\n");
    for (int i = 0; i < fsm_states_count(spec.states); ++i)
    {
        fsm_state_t state = fsm_states_at(spec.states, i);
        bool initial = state == spec.init_state;
        bool final = fsm_spec_check_is_final(spec, state);
        if (initial && final)
            printf("->* ");
        else if (initial)
            printf("->  ");
        else if (final)
            printf("*   ");
        else
            printf("    ");
        printf("%d|", state);
        seek = alpha_copy;
        while (*seek)
        {
            struct FSM_States *to_print = spec.output[state][*seek++ - 'a'];
            if (fsm_states_count(to_print) == 0)
            {
                printf("%*c", spaces, '-');
            }
            else
            {
                char target_buf[LINE_SIZE];
                char *target = target_buf;
                target += sprintf(target, "%d", fsm_states_at(to_print, 0));
                for (int j = 1; j < fsm_states_count(to_print); ++j)
                    target += sprintf(target, " %d", fsm_states_at(to_print, j));
                *target = '\0';
                printf("%*s", spaces, target_buf);
            }
            printf("|");
        }
        printf("\n");
    }
    printf(OUTPUT_DELIM);
}

bool fsm_spec_nondeterministic(struct FSM_Spec spec)
{
    for (int i = 0; i < fsm_states_count(spec.states); ++i)
        for (char *alpha = spec.alphabet; *alpha; ++alpha)
            if (fsm_states_count(spec.output[fsm_states_at(spec.states, i)][*alpha - 'a']) > 1)
                return true;
    return false;
}

bool fsm_spec_eps(struct FSM_Spec spec)
{
    for (int i = 0; i < fsm_states_count(spec.states); ++i)
        if (fsm_states_count(spec.output[fsm_states_at(spec.states, i)][FSM_SYMBOL_EPS - 'a']) > 0)
            return true;
    return false;
}
