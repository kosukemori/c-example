// Example of IPv6 UDP Client
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define SERVER_ADDRESS "::1"
#define SERVER_PORT 33333
#define OPTLEN 8

int main(int argc, char **argv) {
    int the_socket;
    struct sockaddr_in6 server_info;
    char buffer[1024];
    uint8_t options[OPTLEN]; // Option Type(1 byte) + Length (1 byte) + Length of MAC address (6 bytes)

    // Create UDP socket
    if ((the_socket = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6)) < 0) {
        perror("'socket()' failed");
        exit(1);
    }

    options[0] = 1; // Option Type - "source link layer address" (Section 4.6 of RFC 4861)
    options[1] = OPTLEN / 8;  // Option Length - units of 8 octets (RFC 4861)
    options[2] = ;
    uint8_t bytes[6];
    int values[6];
    int i;

    if( 6 == sscanf(mac, "%x:%x:%x:%x:%x:%x%*c",
                &values[0], &values[1], &values[2],
                &values[3], &values[4], &values[5] ) )
    {
        /* convert to uint8_t */
        for( i = 0; i < 6; ++i )
            bytes[i] = (uint8_t) values[i];
    }


    // Configure server's address and ports
    memset(&server_info, 0, sizeof(server_info));
    server_info.sin6_family = AF_INET6;
    server_info.sin6_port = htons(SERVER_PORT);
    inet_pton(AF_INET6, SERVER_ADDRESS, &server_info.sin6_addr);

    strncpy(buffer, "Hello!\0", 1024);
    if (sendto(the_socket, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&server_info, sizeof(server_info)) < 0) {
        perror("'sendto()' failed");
        exit(4);
    }

    close(the_socket);
    return 0;
}
