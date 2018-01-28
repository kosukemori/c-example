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
    int the_socket;
    struct sockaddr_in server_info, client_info;
    socklen_t client_info_length;
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
    server_info.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(the_socket, (struct sockaddr *)&server_info, sizeof(server_info)) < 0) {
        perror("'bind' failed");
        exit(1);
    }

    client_info_length = sizeof(client_info); // FIXME: この行必要? recvfromで上書きされそうなもんだけど……
    buffer[0] = '\0';
    if (recvfrom(the_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_info, &client_info_length) < 0) {
        perror("'recvfrom' failed");
        exit(4);
    }
    printf("%s\n", buffer);

    close(the_socket);
    return 0;
}
