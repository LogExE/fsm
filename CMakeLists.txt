cmake_minimum_required(VERSION 3.10)

project("fsm")

add_executable(fsm
    main.c
    fsm.c
    fsm_states.c
    fsm_spec.c
)