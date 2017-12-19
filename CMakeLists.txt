cmake_minimum_required(VERSION 3.9)
project(c_networking_example)

set(CMAKE_CXX_STANDARD 11)

add_executable(c_networking_example
        ipv4-udp-client.c)
