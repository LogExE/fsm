cmake_minimum_required(VERSION 3.10)

project("fsm")

add_executable(fsm
    main.c
)

add_executable(to_dfsm
    to_dfsm.c
)

set(FSM "KDA" CACHE STRING "Type of automata to simulate")

if (FSM STREQUAL "KDA")
    target_compile_definitions(fsm PRIVATE "TYPE_KDA")
elseif(FSM STREQUAL "NDA")
    target_compile_definitions(fsm PRIVATE "TYPE_NDA")
else()
    target_compile_definitions(fsm PRIVATE "TYPE_NDA_EPS")
endif()

add_subdirectory(fsmlib)
add_subdirectory(bufreadline)

target_link_libraries(fsm PUBLIC fsmlib)
target_link_libraries(to_dfsm PUBLIC fsmlib)

include_directories(fsmlib bufreadline)
target_compile_options(fsm PRIVATE -Wall -Wextra -Wpedantic)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputkda.txt  ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputnda.txt  ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputndaeps.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)