#pragma once

#include <stdbool.h>

#include "fda_spec.h"

typedef enum
{
    FDA_NEXT,
    FDA_FAILED,
    FDA_RECOGNIZED
} FDA_Output;

typedef struct FDA* fda_t;

fda_t fda_create(FDA_Spec *spec);
void fda_free(fda_t aut);

void fda_reset(fda_t aut);
void fda_step(fda_t aut, char input);

void fda_output_rules(const fda_t aut);
state_t fda_get_state(const fda_t aut);
FDA_Output fda_get_output(const fda_t aut);