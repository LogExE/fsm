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
    FDA_Spec spec;
    bool read = fda_spec_read_from(file, &spec);
    if (!read)
    {
        fprintf(stderr, "Failed to read FDA.\n");
        return 1;
    }
    fclose(file);

    printf("Read automaton:\n");
    fda_spec_output(spec);
    fda_t test = fda_create(&spec);
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

    while (*input)
    {
        fda_step(test, *input);
        state_t cur = fda_get_state(test);
        printf("Passed %c to automaton, current state is %d\n", *input, cur);
        ++input;
    }
    if (fda_get_output(test) == FDA_RECOGNIZED)
        printf("Word has been recognized!\n");
    else
        printf("Word has not been recognized!\n");
    fda_free(test);
}