// Example of IPv4 TCP Server
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define SERVER_ADDRESS "::1"
#define SERVER_PORT 33333

int main(int argc, char **argv) {
    // 複数のクライアントと同時接続するときはnew_server_socket, client_address, client_address_lengthクライアントの数だけ必要
    int server_socket, new_server_socket;
    struct sockaddr_in6 server_address, client_address;
    int sockopt_enable = 1;
    socklen_t client_address_length;
    char buffer[1024];
    ssize_t recv_size;
    int total_recv_size;

    // Create TCP socket
    if ((server_socket = socket(PF_INET6, SOCK_STREAM, 0)) < 0) {
        perror("Creating socket");
        exit(1);
    }

    // Configure server's address and ports
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin6_family = AF_INET6;
    server_address.sin6_port = htons(SERVER_PORT);
    server_address.sin6_addr = in6addr_any;

    // Avoid 'Address already in use'
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &sockopt_enable, sizeof(int)) < 0) {
        perror("'setsockopt' failed");
        exit(1);
    }

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("'bind' failed");
        exit(1);
    }

    // Create  the queue of pending connections
    if (listen(server_socket, 10) < 0) { // 第2引数は接続要求をストックできるキューの数
        perror("'listen' failed");
        exit(1);
    }

    //// 複数クライアントと同時接続する場合は、この先をforkなり別スレッド化なりする必要がある
    client_address_length = sizeof(client_address); // FIXME: この行必要? acceptで上書きされそうなもんだけど……

    // Begin hand-shaking
    if ((new_server_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_length)) < 0) {
        perror("'accept' failed");
        exit(1);
    }

    total_recv_size = 0;
    while (1) {
        buffer[0] = '\0';
        if ((recv_size = recv(new_server_socket, buffer, sizeof(buffer), 0)) < 0) {
            // recv(new_server_socket, buffer, sizeof(buffer), 0) は
            // recvfrom(new_server_socket, buffer, sizeof(buffer), 0, NULL, 0) と同じ
            // read(new_server_socket, buffer, sizeof(buffer))もほぼ同じ
            perror("'recv failed");
            exit(4);
        }
        // 一度recvに成功すると、以降ノンブロッキングでrecvし続ける点に注意
        total_recv_size += recv_size;
        printf("%s", buffer);
        if (total_recv_size >= 7) { // 今送ってきているのは文字列は"Hello!\0"、長さ7なので
            break;
        }

        // 複数のデータが[データ1][データ2]...と送られてくる場合、recvした所がちょうどデータの区切れ目である保証はない
        // recvを常にループさせてバイト列を受け取るたびにどこか大きめのバッファに蓄積しつつ、
        // データの区切り目をバッファの中から見つけ出してはデータ1個分を取り出す操作を繰り返す必要がある

        // データそれぞれが固定長なら良いが、可変長の場合はデータ内の先頭にヘッダ領域を用意しデータ長を記入し、
        // それをrecv側で読んでデータの区切れ目を自分で見つける必要がある
    }

    close(new_server_socket);
    //// 複数クライアントとの通信する場合、forkや別スレッド化などする必要があるのはここまで。

    close(server_socket);
    return 0;
}
