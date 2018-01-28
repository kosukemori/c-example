// Example of IPv4 TCP Client
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define SERVER_ADDRESS "::1"
#define SERVER_PORT 33333

int main(int argc, char **argv) {
    int client_socket;
    struct sockaddr_in6 server_address;
    char buffer[1024];

    // Create TCP socket
    if ((client_socket = socket(PF_INET6, SOCK_STREAM, 0)) < 0) {
        perror("Creating socket");
        exit(1);
    }

    // Configure server's address and ports
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin6_family = AF_INET6;
    server_address.sin6_port = htons(SERVER_PORT);
    inet_pton(AF_INET6, SERVER_ADDRESS, &server_address.sin6_addr);

    // Begin hand-shaking
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("'connect' failed");
        exit(4);
    }

    strncpy(buffer, "Hello!\0", 1024);
    if (send(client_socket, buffer, strlen(buffer) + 1, 0) < 0) {
        // send(client_socket, buffer, strlen(buffer) + 1, 0) は
        // sendto(client_socket, buffer, strlen(buffer) + 1, 0, NULL, 0) と同じ
        // write(client_socket, buffer, strlen(buffer) + 1) もほぼ同じ
        perror("'sendto' failed");
        exit(4);
    }

    close(client_socket);
    return 0;
}
