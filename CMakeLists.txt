cmake_minimum_required(VERSION 3.10)

project("fsm")

add_executable(fsm
    main.c
    fsm.c
    fsm_states.c
    fsm_spec.c
    fsm_array.c
    mydefs.c
)

target_compile_options(fsm PRIVATE -Wall -Wextra -Wpedantic)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/input.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)