// Example of IPv4/IPv6 UDP Client
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "config.h"

int main(int argc, char **argv) {
    read_config();
    int server_socket;
    char buffer[1024];
    struct addrinfo hints, *res;
    char port[20];
    sprintf(port, "%d", config.ipv6_server_port);

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_UNSPEC; // IPv4/IPv6両対応
    if (getaddrinfo(config.ipv6_server_address, port, &hints, &res) != 0) {
        perror("getaddrinfo");
        exit(1);
    }

    // Create UDP socket
    if ((server_socket = socket(res->ai_family, res->ai_socktype, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    strncpy(buffer, "Hello!\0", 6); // NULL文字を抜いて6文字
    if (sendto(server_socket, buffer, sizeof(buffer), 0, res->ai_addr, res->ai_addrlen) < 0) {
        perror("sendto");
        exit(4);
    }

    close(server_socket);
    return 0;
}
