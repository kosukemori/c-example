#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 33333

int main(int argc, char **argv) {
    int clientSocket;
    char buffer[1024];
    struct sockaddr_in serverAddress;
    socklen_t address_size;

    // Create UDP socket
    clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

    // Configure sockaddr_in
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    memset(serverAddress.sin_zero, '\0', sizeof serverAddress.sin_zero);

    // Initialize size variable to be used later on
    address_size = sizeof serverAddress;

    strncpy(buffer, "Hello!", 1024);
    sendto(clientSocket, buffer, 1024, 0, (struct sockaddr *)&serverAddress,address_size);
    return 0;
}
