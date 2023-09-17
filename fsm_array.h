#pragma once

#include "fsm.h"

typedef struct FSM_Array fsm_array_t;

fsm_array_t *fsm_array_create();
void fsm_array_free(fsm_array_t *arr);

void fsm_array_add(fsm_array_t *arr, fsm_t *aut);
fsm_t *fsm_array_at(const fsm_array_t *arr, int idx);
int fsm_array_count(const fsm_array_t *arr);