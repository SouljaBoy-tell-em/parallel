cmake_minimum_required(VERSION 3.29)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled
        input_tasks/task1.c
        input_tasks/task2.c
        input_tasks/task3.c
        input_tasks/add_tack.c
        lab/3b/OMP/3b.c
        lab/3b/OMP/general.h
        lab/3b/OMP/ethalon/ethalon.c
        lab/3b/OMP/run.c
        lab/3b/MPI/general.h
        lab/3b/MPI/1c_par.c
        lab/3b/MPI/1c.c
        lab/3b/MPI/ethalon/ethalon.c
        lab/3b/MPI/ethalon/ethalon_par.c
        lab/3b/MPI/run.c
        test.c
        lab/3b/MPI/1c_par.c
        lab/2/lab.c
)
