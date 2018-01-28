#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 33333

int main(int argc, char **argv) {
    int client_socket;
    char buffer[1024];
    struct sockaddr_in server_address;

    // Create UDP socket
    if ((client_socket = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Creating socket");
        exit(1);
    }

    // Configure server's address and ports
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

    strncpy(buffer, "Hello!", 1024);
    if (sendto(client_socket, buffer, 1024, 0, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("'sendto' failed");
        exit(4);
    }

    close(client_socket);
    return 0;
}
