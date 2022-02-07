cmake_minimum_required(VERSION 3.15)
project(c_example C)

set(CMAKE_C_STANDARD 11)

include_directories(network)

add_executable(c_example
        network/config.h
        network/ipv4-tcp-client.c
        network/ipv4-tcp-server.c
        network/ipv4-udp-client.c
        network/ipv4-udp-server.c
        network/ipv6-icmp.c
        network/ipv6-router-solicitation.c
        network/ipv6-tcp-client.c
        network/ipv6-tcp-server.c
        network/ipv6-udp-client.c
        network/ipv6-udp-server.c
        network/ipv4v6-udp-client.c
        padding/struct-padding.c
        thread/pthread.c time/measure-time.c)
