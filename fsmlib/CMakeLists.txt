cmake_minimum_required(VERSION 3.10)

project("fsmlib")

add_library(fsmlib
    fsm_states.c
    fsm_spec.c
    kda.c
    nda.c
    nda_eps.c
)

include_directories(../bufreadline)
target_link_libraries(fsmlib PUBLIC bufreadline)