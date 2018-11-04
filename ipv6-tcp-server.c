// Example of IPv6 TCP Server
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "config.h"


int main(int argc, char **argv) {
    read_config();
    // 複数のクライアントと同時接続するときはclient_socket, client_info, client_info_lenはクライアントの数だけ必要
    int server_socket, client_socket;
    struct sockaddr_in6 server_info, client_info;
    int sockopt_enable = 1;
    socklen_t client_info_len;
    char buffer[1024];
    ssize_t recv_size;
    int total_recv_size;

    // Create TCP socket
    if ((server_socket = socket(PF_INET6, SOCK_STREAM, 0)) < 0) {
        perror("'socket' failed");
        exit(1);
    }

    // Configure server's address and ports
    memset(&server_info, 0, sizeof(server_info));
    server_info.sin6_family = AF_INET6;
    server_info.sin6_port = htons(config.ipv6_server_port);
    server_info.sin6_addr = in6addr_any;

    // Avoid 'Address already in use'
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &sockopt_enable, sizeof(int)) < 0) {
        perror("'setsockopt' failed");
        exit(1);
    }

    if (bind(server_socket, (struct sockaddr *)&server_info, sizeof(server_info)) < 0) {
        perror("'bind' failed");
        exit(1);
    }

    // Create  the queue of pending connections
    if (listen(server_socket, 10) < 0) { // 第2引数は接続要求をストックできるキューの数
        perror("'listen' failed");
        exit(1);
    }

    //// 複数クライアントと同時接続する場合は、この先をforkなり別スレッド化なりする必要がある
    // Begin hand-shaking
    client_info_len = sizeof(client_info);
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_info, &client_info_len)) < 0) {
        perror("'accept' failed");
        exit(1);
    }

    total_recv_size = 0;
    while (1) {
        if ((recv_size = recv(client_socket, buffer, sizeof(buffer), 0)) < 0) {
            // recv(client_socket, buffer, sizeof(buffer), 0) は
            // recvfrom(client_socket, buffer, sizeof(buffer), 0, NULL, 0) と
            // read(client_socket, buffer, sizeof(buffer)) と同じ
            perror("'recv failed");
            exit(4);
        }
        // 一度recvに成功すると、以降ノンブロッキングでrecvし続ける点に注意
        total_recv_size += recv_size;
        printf("%s\n", buffer);
        if (total_recv_size >= 1024) { // クライアント側で1024Byteのバッファを送るという前提
            break;
        }

        // 複数のデータが[データ1][データ2]...と送られてくる場合、データそれぞれが固定長なら良いが、
        // 可変長の場合はデータ内の先頭にヘッダ領域を用意しデータ長を記入し、
        // それをrecv側で読んでデータの区切れ目を自分で見つける必要がある

        // FIXME: 切断検知を厳密にやるには、TCPの切断検知に加えて、定期的にデータの送受信がないと切断と判断する仕様が必要
    }

    close(client_socket);
    //// 複数クライアントとの通信する場合、forkや別スレッド化などする必要があるのはここまで。

    close(server_socket);
    return 0;
}
