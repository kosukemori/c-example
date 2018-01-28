// Example of IPv4 TCP Server
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 33333

int main(int argc, char **argv) {
    // 複数のクライアントと同時接続するときはthe_new_socket, client_info, client_info_lengthはクライアントの数だけ必要
    int the_socket, the_new_socket;
    struct sockaddr_in server_info, client_info;
    int sockopt_enable = 1;
    socklen_t client_info_length;
    char buffer[1024];
    ssize_t recv_size;
    int total_recv_size;

    // Create TCP socket
    if ((the_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("'socket' failed");
        exit(1);
    }

    // Configure server's address and ports
    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(SERVER_PORT);
    server_info.sin_addr.s_addr = htonl(INADDR_ANY);

    // Avoid 'Address already in use'
    if (setsockopt(the_socket, SOL_SOCKET, SO_REUSEADDR, &sockopt_enable, sizeof(int)) < 0) {
        perror("'setsockopt' failed");
        exit(1);
    }

    if (bind(the_socket, (struct sockaddr *)&server_info, sizeof(server_info)) < 0) {
        perror("'bind' failed");
        exit(1);
    }

    // Create  the queue of pending connections
    if (listen(the_socket, 10) < 0) { // 第2引数は接続要求をストックできるキューの数
        perror("'listen' failed");
        exit(1);
    }

    //// 複数クライアントと同時接続する場合は、この先をforkなり別スレッド化なりする必要がある
    client_info_length = sizeof(client_info); // FIXME: この行必要? acceptで上書きされそうなもんだけど……

    // Begin hand-shaking
    if ((the_new_socket = accept(the_socket, (struct sockaddr *)&client_info, &client_info_length)) < 0) {
        perror("'accept' failed");
        exit(1);
    }

    total_recv_size = 0;
    while (1) {
        buffer[0] = '\0';
        if ((recv_size = recv(the_new_socket, buffer, sizeof(buffer), 0)) < 0) {
            // recv(the_new_socket, buffer, sizeof(buffer), 0) は
            // recvfrom(the_new_socket, buffer, sizeof(buffer), 0, NULL, 0) と同じ
            // read(the_new_socket, buffer, sizeof(buffer))もほぼ同じ
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

        // FIXME: 本当は定期的にデータの送受信がないと切断と判断する仕様にするなど、TCPの仕組みに頼らない形で切断検知をしないといけない
    }

    close(the_new_socket);
    //// 複数クライアントとの通信する場合、forkや別スレッド化などする必要があるのはここまで。

    close(the_socket);
    return 0;
}
