// Example of IPv4 TCP Client
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
    struct sockaddr_in server_info;
    char buffer[1024];

    // Create TCP socket
    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("'socket' failed");
        exit(1);
    }

    // Configure server's address and ports
    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(config.ipv4_server_port);
    server_info.sin_addr.s_addr = inet_addr(config.ipv4_server_address);

    // Begin hand-shaking
    if (connect(server_socket, (struct sockaddr *)&server_info, sizeof(server_info)) < 0) {
        perror("'connect' failed");
        exit(4);
    }

    strncpy(buffer, "Hello!\0", 6); // NULL文字を抜いて6文字
    if (send(server_socket, buffer, sizeof(buffer), 0) < 0) {
        // send(server_socket, buffer, sizeof(buffer), 0) は
        // sendto(server_socket, buffer, sizeof(buffer), 0, NULL, 0) と
        // write(server_socket, buffer, sizeof(buffer)) と同じ
        perror("'sendto' failed");
        exit(4);
    }
    // FIXME: 切断検知を厳密にやるには、TCPの切断検知に加えて、定期的にデータの送受信がないと切断と判断する仕様が必要

    close(server_socket);
    return 0;
}
