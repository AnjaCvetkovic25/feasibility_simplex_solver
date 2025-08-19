cmake_minimum_required(VERSION 3.16)

project(simplex_procedure LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(simplex_procedure main.cpp
    simplex_solver.h simplex_solver.cpp
    constraint_parser.h
    constraint_parser.cpp
)

include(GNUInstallDirs)
install(TARGETS simplex_procedure
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

configure_file(constraints.txt constraints.txt COPYONLY)
