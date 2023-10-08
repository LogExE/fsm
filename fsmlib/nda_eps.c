#include "nda_eps.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct NDA_Eps
{
    struct FSM_Spec *spec;
    struct FSM_States *cur_states;
};

struct NDA_Eps *nda_eps_create(struct FSM_Spec *spec)
{
    struct NDA_Eps *ret = malloc(sizeof(struct NDA_Eps));
    ret->spec = spec;
    ret->cur_states = nda_eps_closure(spec->init_state, *spec);
    return ret;
}

void nda_eps_free(struct NDA_Eps *aut)
{
    fsm_states_free(aut->cur_states);
    free(aut);
}

void nda_eps_reset(struct NDA_Eps *aut)
{
    fsm_states_free(aut->cur_states);
    aut->cur_states = nda_eps_closure(aut->spec->init_state, *aut->spec);
}

void nda_eps_step(struct NDA_Eps *aut, char input)
{
    struct FSM_States *next = nda_eps_states_step(aut->cur_states, *aut->spec, input);
    fsm_states_free(aut->cur_states);
    aut->cur_states = next;
}

struct FSM_States *nda_eps_states_step(const struct FSM_States *states, struct FSM_Spec spec, char input)
{
    struct FSM_States *stepped = fsm_states_create();
    for (int i = 0; i < fsm_states_count(states); ++i)
    {
        fsm_state_t state = fsm_states_at(states, i);
        struct FSM_States *to = nda_eps_states_closure(spec.output[state][input - 'a'], spec);
        for (int j = 0; j < fsm_states_count(to); ++j)
        {
            fsm_state_t value = fsm_states_at(to, j);
            fsm_states_add_uniq(stepped, value);
        }
        fsm_states_free(to);
    }
    return stepped;
}

struct FSM_States *nda_eps_closure(fsm_state_t state, struct FSM_Spec spec)
{
    struct FSM_States *queue = fsm_states_create();
    fsm_states_add(queue, state);
    struct FSM_States *res = fsm_states_create();
    while (fsm_states_count(queue) > 0)
    {
        fsm_state_t head = fsm_states_at(queue, 0);
        fsm_states_remove(queue, 0);
        fsm_states_add_uniq(res, head);
        struct FSM_States *to = spec.output[head][FSM_SYMBOL_EPS - 'a'];
        for (int i = 0; i < fsm_states_count(to); ++i)
            fsm_states_add(queue, fsm_states_at(to, i));
    }
    fsm_states_free(queue);
    return res;
}

struct FSM_States *nda_eps_states_closure(const struct FSM_States *states, struct FSM_Spec spec)
{
    struct FSM_States *res = fsm_states_create();
    for (int i = 0; i < fsm_states_count(states); ++i)
    {
        struct FSM_States *clos = nda_eps_closure(fsm_states_at(states, i), spec);
        for (int j = 0; j < fsm_states_count(clos); ++j)
        {
            fsm_state_t clos_state = fsm_states_at(clos, j);
            fsm_states_add_uniq(res, clos_state);
        }
        fsm_states_free(clos);
    }
    return res;
}

struct FSM_States *nda_eps_get_states(const struct NDA_Eps *aut)
{
    return aut->cur_states;
}

bool nda_eps_recognized(const struct NDA_Eps *aut)
{
    for (int i = 0; i < fsm_states_count(aut->cur_states); ++i)
        if (fsm_states_contains(aut->spec->fin_states, fsm_states_at(aut->cur_states, i)))
            return true;
    return false;
}

struct FSM_Spec nda_eps_convert_spec_to_nda(struct FSM_Spec spec)
{
    struct FSM_Spec ret;

    ret.alphabet = malloc(strlen(spec.alphabet));
    strcpy(ret.alphabet, spec.alphabet);
    ret.states = fsm_states_copy(spec.states);
    ret.init_state = spec.init_state;

    ret.fin_states = fsm_states_create();
    for (fsm_state_t state = 0; state < FSM_MAX_STATE_NUM; ++state)
        for (int i = 0; i < FSM_ALPHABET_SIZE; ++i)
        {
            ret.output[state][i] = fsm_states_create();
            struct FSM_States *clos = nda_eps_closure(state, spec);
            for (int j = 0; j < fsm_states_count(clos); ++j)
            {
                fsm_state_t clos_state = fsm_states_at(clos, j);
                if (fsm_spec_check_is_final(spec, clos_state))
                    fsm_states_add_uniq(ret.fin_states, state);
                struct FSM_States *to = spec.output[clos_state][i];
                for (int k = 0; k < fsm_states_count(to); ++k)
                    fsm_states_add_uniq(ret.output[state][i], fsm_states_at(to, k));
            }
            fsm_states_free(clos);
        }

    return ret;
}
