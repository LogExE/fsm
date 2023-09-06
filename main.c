#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "fda.h"

#define LINE_SIZE 512
#define FILE_MAX_RULES_COUNT 512

#define FDA_FILE "input.txt"

bool init_fda_from(FILE *stream, FDA *aut)
{
    // Заготовочка переменных
    char *fda_alphabet;
    FDA_States fda_states;
    fda_states.count = 0;
    FDA_States fda_fin_states;
    fda_fin_states.count = 0;
    int init_state;

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

    int alph_cnt = 0;
    if (buf[0] == '!')
    {
        alph_cnt = atoi(buf + 1);
        if (alph_cnt > FDA_ALPHABET_SIZE)
        {
            fprintf(stderr, "Expected alphabet size <= %d, found %d symbols!\n", FDA_ALPHABET_SIZE, alph_cnt);
            return false;
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
                return false;
            }
            ++alph_cnt;
            ++ptr;
        }
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
        fda_states = fda_states_alloc(atoi(buf + 1));
        for (int i = 0; i < fda_states.count; ++i)
            fda_states.values[i] = i + 1;
    }
    else
    {
        char *seek = buf;
        char *end = NULL;
        state_t pre_states[buf_len];
        int i = 0;
        while (1)
        {
            pre_states[i] = strtol(seek, &end, 10);
            if (seek == end)
                break;
            seek = end;
            ++i;
        }
        fda_states = fda_states_alloc(i);
        for (int j = 0; j < i; ++j)
            fda_states.values[j] = pre_states[j];
    }
    printf("total states found: %d\n", fda_states.count);

    // Читаем финальные состояния
    fgets(buf, LINE_SIZE, stream);
    buf_len = strlen(buf);

    {
        char *seek = buf;
        char *end = NULL;
        state_t pre_states[buf_len];
        int i = 0;
        while (1)
        {
            pre_states[i] = strtol(seek, &end, 10);
            if (seek == end)
                break;
            seek = end;
            ++i;
        }
        fda_fin_states = fda_states_alloc(i);
        for (int j = 0; j < i; ++j)
            fda_fin_states.values[j] = pre_states[j];
    }
    printf("total final states found:  %d\n", fda_fin_states.count);

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
        rules_sym[rules_cnt] = *seek;
        ++seek; // пропуск пробела
        rules[FILE_MAX_RULES_COUNT + rules_cnt] = strtol(seek, &end, 10);
        printf("found rule: (%d, %c) -> %d\n",
               rules[rules_cnt],
               rules_sym[rules_cnt],
               rules[FILE_MAX_RULES_COUNT + rules_cnt]);
        ++rules_cnt;
    }

    printf("good.\n");

    fda_init(aut);
    //  Применяем изменения
    fda_set_alphabet(aut, fda_alphabet);
    fda_set_states(aut, fda_states);
    fda_set_final_states(aut, fda_fin_states);
    fda_set_initial_state(aut, init_state);

    for (int i = 0; i < rules_cnt; ++i)
        fda_add_rule(aut, rules[i], rules_sym[i], rules[FILE_MAX_RULES_COUNT + i]);

    return true;
}

int main()
{
    FDA test;
    FILE *file = fopen(FDA_FILE, "r");
    if (file == NULL)
    {
        fprintf(stderr, "File %s doesn't exist!\n", FDA_FILE);
        return 1;
    }
    if (!init_fda_from(file, &test))
    {
        fprintf(stderr, "Failed to read FDA.\n");
        return 1;
    }
    fclose(file);

    printf("Read automata:\n");
    fda_output_rules(&test);
    fda_reset(&test);

    printf("> ");
    char inp[LINE_SIZE];
    char *input = fgets(inp, LINE_SIZE, stdin);
    if (input == NULL)
    {
        fprintf(stderr, "Got no input.\n");
        return 1;
    }
    inp[strcspn(inp, "\n")] = '\0';

    FDA_Result res;
    do
    {
        res = fda_step(&test, *input++);
        printf("step result: %d\n", res);
    } while (*input && res == NEXT);
    if (res == WORD)
        printf("Automata is in final state, word has been recognized!\n");
    else
        printf("Word has not been recognized!\n");
    fda_free(&test);
}