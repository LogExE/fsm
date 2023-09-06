#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fda.h"

#define FDA_FILE "input.txt"

int main()
{
    FILE *file = fopen(FDA_FILE, "r");
    if (file == NULL)
    {
        fprintf(stderr, "File %s doesn't exist!\n", FDA_FILE);
        return 1;
    }
    FDA_Spec *spec = fda_spec_read_from(file);
    if (spec == NULL)
    {
        fprintf(stderr, "Failed to read FDA.\n");
        return 1;
    }
    fclose(file);

    FDA *test = fda_create(spec);
    printf("Read automaton:\n");
    fda_output_rules(test);
    fda_reset(test);

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
        res = fda_step(test, *input++);
        printf("step result: %d\n", res);
    } while (*input && res == NEXT);
    if (res == WORD)
        printf("Automaton is in final state, word has been recognized!\n");
    else
        printf("Word has not been recognized!\n");
    fda_free(test);
}