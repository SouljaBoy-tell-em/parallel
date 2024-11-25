cmake_minimum_required(VERSION 3.29)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled
        input_tasks/task1.c
        input_tasks/task2.c
        input_tasks/task3.c
        input_tasks/add_tack.c
        lab/ethalon/ethalon.c
        lab/3b/3b.c
        lab/general.h
        lab/3b/run.c
)
