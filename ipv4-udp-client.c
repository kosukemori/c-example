// Example of IPv4 UDP Client
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 33333

int main(int argc, char **argv) {
    int the_socket;
    struct sockaddr_in server_info;
    char buffer[1024];

    // Create UDP socket
    if ((the_socket = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("'socket' failed");
        exit(1);
    }

    // Configure server's address and ports
    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(SERVER_PORT);
    server_info.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

    strncpy(buffer, "Hello!\0", 1024);
    if (sendto(the_socket, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&server_info, sizeof(server_info)) < 0) {
        perror("'sendto' failed");
        exit(4);
    }

    close(the_socket);
    return 0;
}
