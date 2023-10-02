cmake_minimum_required(VERSION 3.10)

project("bufreadline")

add_library(bufreadline
    bufreadline.c
)

target_compile_options(bufreadline PRIVATE -Wall -Wextra -Wpedantic)