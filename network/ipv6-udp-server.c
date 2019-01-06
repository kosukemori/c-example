// Example of IPv6 UDP Server
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "config.h"

int main(int argc, char **argv) {
    read_config();
    int server_socket;
    struct sockaddr_in6 server_info, client_info;
    socklen_t client_info_len;
    char buffer[1024];

    // Create UDP socket
    if ((server_socket = socket(PF_INET6, SOCK_DGRAM, 0)) < 0) {
        perror("'socket' failed");
        exit(1);
    }

    // Configure server's address and ports
    memset(&server_info, 0, sizeof(server_info));
    server_info.sin6_family = AF_INET6;
    server_info.sin6_port = htons(config.ipv6_server_port);
    server_info.sin6_addr = in6addr_any;

    if (bind(server_socket, (struct sockaddr *)&server_info, sizeof(server_info)) < 0) {
        perror("'bind' failed");
        exit(1);
    }

    client_info_len = sizeof(client_info);
    buffer[0] = '\0';
    if (recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_info, &client_info_len) < 0) {
        perror("'recvfrom' failed");
        exit(4);
    }
    printf("%s\n", buffer);
    // この後client_infoを使ってclientに返信も可能

    close(server_socket);
    return 0;
}
