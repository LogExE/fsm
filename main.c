#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "fda.h"

#define LINE_SIZE 512

#define FDA_FILE "input.txt"

void init_fda_from(FILE *stream, FDA *aut)
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
        fprintf(stderr, "Line was too long!");
        return;
    }
    buf[buf_len - 1] = '\0';

    int alph_cnt = 0;
    if (buf[0] == '!')
    {
        alph_cnt = atoi(buf + 1);
        if (alph_cnt > 26)
        {
            fprintf(stderr, "Expected alphabet size < 26, found %d symbols!", alph_cnt);
            return;
        }
        fda_alphabet = malloc(alph_cnt);
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
                fprintf(stderr, "Expected alphabet to be english alphabet, found symbol '%c'!", *ptr);
                return;
            }
            ++alph_cnt;
            ++ptr;
        }
        fda_alphabet = malloc(alph_cnt);
        memcpy(fda_alphabet, buf, alph_cnt);
    }
    printf("Alphabet: %s\n", fda_alphabet);

    // Читаем возможные состояния автомата
    fgets(buf, LINE_SIZE, stream);
    buf_len = strlen(buf);

    if (buf[0] == '!')
    {
        fda_states.count = atoi(buf + 1);
        fda_states.states = malloc(fda_states.count * sizeof(state_t));
        for (int i = 1; i <= fda_states.count; ++i)
            fda_states.states[i] = i;
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
        printf("found %d states\n", i);
        fda_states.count = i;
        fda_states.states = malloc(fda_states.count * sizeof(state_t));
        for (int i = 0; i < fda_states.count; ++i)
            fda_states.states[i] = pre_states[i];
    }

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
        printf("found %d final states\n", i);
        fda_fin_states.count = i;
        fda_fin_states.states = malloc(fda_fin_states.count * sizeof(state_t));
        for (int i = 0; i < fda_states.count; ++i)
            fda_fin_states.states[i] = pre_states[i];
    }

    // Читаем начальное состояние
    fgets(buf, LINE_SIZE, stream);
    init_state = atoi(buf);
    printf("%d will be initial state\n", init_state);

    // До конца файла ищем правила переходов
    while (fgets(buf, LINE_SIZE, stream))
    {
        char *seek = buf;
        char *end = NULL;
        state_t s1 = strtol(seek, &end, 10);
        seek = end + 1; // пропуск пробела
        char x = *seek;
        ++seek; // пропуск пробела
        state_t s2 = strtol(seek, &end, 10);
        printf("found rule (%d, %c) -> %d\n", s1, x, s2);
        fda_add_rule(aut, s1, x, s2);
    }

    printf("good.\n");

    // Применяем изменения
    fda_set_alphabet(aut, fda_alphabet);
    fda_set_states(aut, fda_states);
    fda_set_final_states(aut, fda_fin_states);
    fda_set_initial_state(aut, init_state);
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
    init_fda_from(file, &test);
    fclose(file);

    fda_reset(&test);

    printf("> ");
    char inp[LINE_SIZE];
    fgets(inp, LINE_SIZE, stdin);

    char *seek = inp;
    FDA_Result res;
    do
    {
        res = fda_step(&test, *seek++);
        printf("res == %d\n", res);
    } while (*inp && res == NEXT);
    fda_free(&test);
}