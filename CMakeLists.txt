cmake_minimum_required(VERSION 3.15)
project(c_example C)

set(CMAKE_C_STANDARD 11)

FILE(GLOB CFiles */*.c)

add_executable(c_example
        ${CFiles})
