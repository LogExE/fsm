#pragma once

#include "fsm_array.h"
#include "fsm_spec.h"

typedef enum
{
    FSM_NEXT,
    FSM_FAILED,
    FSM_RECOGNIZED
} FSM_Output;

struct FSM;

struct FSM *fsm_create(FSM_Spec *spec);
void fsm_free(struct FSM *aut);

void fsm_reset(struct FSM *aut);
struct FSM_Array *fsm_step(struct FSM *aut, char input);

fsm_state_t fsm_get_state(const struct FSM *aut);
FSM_Output fsm_get_output(const struct FSM *aut);