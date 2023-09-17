#pragma once

#include "fsm_spec.h"

typedef enum
{
    FSM_NEXT,
    FSM_FAILED,
    FSM_RECOGNIZED
} FSM_Output;

typedef struct FSM fsm_t;

fsm_t *fsm_create(FSM_Spec *spec);
void fsm_free(fsm_t *aut);

void fsm_reset(fsm_t *aut);
void fsm_step(fsm_t *aut, char input);

state_t fsm_get_state(const fsm_t *aut);
FSM_Output fsm_get_output(const fsm_t *aut);