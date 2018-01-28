// Example of IPv4 UDP Server
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 33333

int main(int argc, char **argv) {
    int server_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length;
    char buffer[1024];

    // Create UDP socket
    if ((server_socket = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Creating socket");
        exit(1);
    }

    // Configure server's address and ports
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("'bind' failed");
        exit(1);
    }

    client_address_length = sizeof(client_address); // FIXME: この行必要? recvfromで上書きされそうなもんだけど……
    buffer[0] = '\0';
    if (recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_address, &client_address_length) < 0) {
        perror("'recvfrom' failed");
        exit(4);
    }
    printf("%s\n", buffer);

    close(server_socket);
    return 0;
}
