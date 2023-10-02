#include "stdio.h"
#include "stdlib.h"

#include "fsm_spec.h"
#include "nda.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <file_in> <file_out>\n", argv[0]);
        return 1;
    }
    printf("Reading %s...\n", argv[1]);
    FILE *file_in = fopen(argv[1], "r");
    if (file_in == NULL)
    {
        fprintf(stderr, "file_in doesn't exist!\n");
        return 1;
    }
    struct FSM_Spec spec;
    fsm_spec_read_from(file_in, &spec);
    fclose(file_in);
    struct FSM_Spec new_spec = nda_convert_spec_to_kda(spec);
    fsm_spec_free(spec);
    FILE *file_out = fopen(argv[2], "w");
    fsm_spec_write_to(file_out, new_spec);
    fclose(file_out);
    fsm_spec_free(new_spec);
    return 0;
}