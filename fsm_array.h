#pragma once

struct FSM;
struct FSM_Array;

struct FSM_Array *fsm_array_create();
void fsm_array_free(struct FSM_Array *arr);

void fsm_array_add(struct FSM_Array *arr, struct FSM *aut);
struct FSM *fsm_array_at(const struct FSM_Array *arr, int idx);
int fsm_array_count(const struct FSM_Array *arr);