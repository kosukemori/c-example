// Example of IPv6 UDP Client
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define SERVER_ADDRESS "::1"
#define SERVER_PORT 33333

int main(int argc, char **argv) {
    int the_socket;
    struct sockaddr_in6 server_info;
    char buffer[1024];

    // Create UDP socket
    if ((the_socket = socket(PF_INET6, SOCK_DGRAM, 0)) < 0) {
        perror("'socket' failed");
        exit(1);
    }

    // Configure server's address and ports
    memset(&server_info, 0, sizeof(server_info));
    server_info.sin6_family = AF_INET6;
    server_info.sin6_port = htons(SERVER_PORT);
    inet_pton(AF_INET6, SERVER_ADDRESS, &server_info.sin6_addr);

    strncpy(buffer, "Hello!", 1024);
    if (sendto(the_socket, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&server_info, sizeof(server_info)) < 0) {
        perror("'sendto' failed");
        exit(4);
    }

    close(the_socket);
    return 0;
}
