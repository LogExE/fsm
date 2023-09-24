#pragma once

#include "fsm_spec.h"

enum FSM_Output
{
    FSM_NEXT,
    FSM_FAILED,
    FSM_RECOGNIZED
};

struct FSM;
struct FSM_Array;

struct FSM *fsm_create(struct FSM_Spec *spec);
void fsm_free(struct FSM *aut);

void fsm_reset(struct FSM *aut);
struct FSM_Array *fsm_step(struct FSM *aut, char input);

fsm_state_t fsm_get_state(const struct FSM *aut);
enum FSM_Output fsm_get_output(const struct FSM *aut);