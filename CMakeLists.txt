cmake_minimum_required(VERSION 3.16)

project(feasibility_simplex_solver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(feasibility_simplex_solver main.cpp
    simplex_solver.h simplex_solver.cpp
    constraint_parser.h
    constraint_parser.cpp
)

install(TARGETS feasibility_simplex_solver
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

