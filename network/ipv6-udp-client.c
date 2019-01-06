// Example of IPv6 UDP Client
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
    struct sockaddr_in6 server_info;
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
    inet_pton(AF_INET6, config.ipv6_server_address, &server_info.sin6_addr);

    strncpy(buffer, "Hello!\0", 6); // NULL文字を抜いて6文字
    if (sendto(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_info, sizeof(server_info)) < 0) {
        perror("'sendto' failed");
        exit(4);
    }

    close(server_socket);
    return 0;
}
