#pragma once

#include "fda_spec.h"

typedef enum
{
    WORD,
    NOT_WORD,
    NEXT
} FDA_Result;

typedef struct
{
    FDA_Spec *spec;
    int cur_state;
} FDA;

FDA *fda_create(FDA_Spec *spec);
void fda_free(FDA *aut);

static state_t fda_get_out_state(const FDA *aut, char input);

void fda_reset(FDA *aut);
FDA_Result fda_step(FDA *aut, char input);
void fda_output_rules(const FDA *aut);