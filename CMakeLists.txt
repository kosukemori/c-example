cmake_minimum_required(VERSION 3.9)
project(c_networking_example)

set(CMAKE_CXX_STANDARD 11)

add_executable(c_networking_example
        ipv4-tcp-client.c
        ipv4-tcp-server.c
        ipv4-udp-client.c
        ipv4-udp-server.c
        ipv6-udp-client.c
        ipv6-udp-server.c
        ipv6-tcp-client.c
        ipv6-tcp-server.c
        )
