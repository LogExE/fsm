#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fsm_spec.h"
#include "nda.h"
#include "nda_eps.h"

enum Conv_Mode
{
    CONV_MODE_EPS,
    CONV_MODE_NDA,
    CONV_MODE_UKWN
};

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "usage: %s (--eps | --nda) <file_in> <file_out>\n", argv[0]);
        return 1;
    }

    const char *mode = argv[1];
    enum Conv_Mode conv_mode;
    if (strcmp(mode, "--eps") == 0)
        conv_mode = CONV_MODE_EPS;
    else if (strcmp(mode, "--nda") == 0)
        conv_mode = CONV_MODE_NDA;
    else
        conv_mode = CONV_MODE_UKWN;
    if (conv_mode == CONV_MODE_UKWN)
    {
        fprintf(stderr, "Unknown mode: \"%s\"!\n", mode);
        return 1;
    }
    printf("Mode ");
    if (conv_mode == CONV_MODE_EPS)
        printf("EPS: nda_eps -> nda");
    else
        printf("NDA: nda -> kda");
    printf("\n");

    const char *in = argv[2], *out = argv[3];
    printf("Reading %s...\n", in);
    FILE *file_in = fopen(in, "r");
    if (file_in == NULL)
    {
        fprintf(stderr, "file_in doesn't exist!\n");
        return 1;
    }
    struct FSM_Spec spec;
    fsm_spec_read_from(file_in, &spec);
    fclose(file_in);

    struct FSM_Spec new_spec;
    if (conv_mode == CONV_MODE_NDA)
        new_spec = nda_convert_spec_to_kda(spec);
    else
        new_spec = nda_eps_convert_spec_to_nda(spec);
    fsm_spec_free(spec);
    FILE *file_out = fopen(out, "w");
    fsm_spec_write_to(file_out, new_spec);
    fclose(file_out);

    fsm_spec_free(new_spec);
    return 0;
}