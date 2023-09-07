#pragma once

#include <stdbool.h>

#include "fda_spec.h"

typedef struct
{
    FDA_Spec *spec;
    int cur_state;
} FDA;

FDA *fda_create(FDA_Spec *spec);
void fda_free(FDA *aut);

void fda_reset(FDA *aut);
void fda_step(FDA *aut, char input);
void fda_output_rules(const FDA *aut);
bool fda_check_final(const FDA *aut);