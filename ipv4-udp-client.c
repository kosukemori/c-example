#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char **argv) {
    int clientSocket;
    char buffer[1024];
    struct sockaddr_in serverAddress;
    socklen_t address_size;
    // Create UDP socket
    clientSocket = socket(PF_INET, SOCK_DGRAM, 0);
    // Configure settings in address struct
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(33333);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddress.sin_zero, '\0', sizeof serverAddress.sin_zero);
    // Initialize size variable to be used later on
    address_size = sizeof serverAddress;

//    strncpy(buffer, "132,255:0,255,0,255", 1024);
    sendto(clientSocket,buffer,1024,0,(struct sockaddr *)&serverAddress,address_size);
    return 0;
}
