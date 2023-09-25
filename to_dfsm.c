#include "stdio.h"
#include "stdlib.h"

#include "fsm_spec.h"

void convert_write(struct FSM_Spec spec, FILE *file)
{
    fprintf(file, "%s\n", spec.alphabet);
    
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <file_in> <file_out>\n", argv[0]);
        return 1;
    }
    FILE *file_in = fopen(argv[1], "r");
    if (file_in == NULL)
    {
        fprintf(stderr, "file_in doesn't exist!\n");
        return 1;
    }
    struct FSM_Spec spec;
    fsm_spec_read_from(file_in, &spec);
    fclose(file_in);
    FILE *file_out = fopen(argv[2], "w");
    convert_write(spec, file_out);
    fclose(file_out);
    free(spec.alphabet);
    fsm_states_free(spec.states);
    fsm_states_free(spec.fin_states);
    return 0;
}