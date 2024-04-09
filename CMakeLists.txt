cmake_minimum_required(VERSION 3.27)
project(C_SIS C)

set(CMAKE_C_STANDARD 11)

# Set the directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main main.c)
add_executable(nicole nicole.c)
add_executable(cy cy.c)

