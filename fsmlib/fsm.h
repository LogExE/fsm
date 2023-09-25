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
void fsm_step(struct FSM *aut, char input);

struct FSM_States_Set *fsm_get_states(const struct FSM *aut);
enum FSM_Output fsm_get_output(const struct FSM *aut);