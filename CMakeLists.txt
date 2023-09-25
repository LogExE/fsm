cmake_minimum_required(VERSION 3.10)

project("fsm")

add_executable(fsm
    main.c
)

add_executable(to_dfsm
    to_dfsm.c
)

add_subdirectory(fsmlib)
add_subdirectory(bufreadline)

target_link_libraries(fsm PUBLIC fsmlib)
target_link_libraries(to_dfsm PUBLIC fsmlib)

include_directories(fsmlib bufreadline)
target_compile_options(fsm PRIVATE -Wall -Wextra -Wpedantic)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/input.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)