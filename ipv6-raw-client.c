// Example of IPv6 UDP Client
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#define SERVER_ADDRESS "ff02::1"
#define OPTLEN 8

typedef struct _pktinfo6 pktinfo6;
struct _pktinfo6 {
    struct in6_addr ipi6_addr;
    int ipi6_ifindex;
};

// Function prototypes
uint16_t checksum (uint16_t *, int);

int main(int argc, char **argv) {
    int the_socket;
    struct sockaddr_in6 server_info;
    char buffer[1024];
    int RS_HDRLEN = sizeof (struct nd_router_solicit);  // Length of RS message header
    uint8_t options[OPTLEN]; // Option Type(1 byte) + Length (1 byte) + Length of MAC address (6 bytes)
    char *mac = "e8:ea:6a:06:2a:fe";
    int values[6];
    int i;
    uint8_t outpack[IP_MAXPACKET];
    struct nd_router_solicit *rs;
    uint8_t *psdhdr;
    int psdhdrlen;
    struct msghdr msghdr;
    struct iovec iov[2];
    int cmsglen;
    pktinfo6 *pktinfo;
    int hoplimit;
    struct cmsghdr *cmsghdr1, *cmsghdr2;

    // Create UDP socket
    if ((the_socket = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6)) < 0) {
        perror("'socket()' failed");
        exit(1);
    }

    options[0] = 1; // Option Type - "source link layer address" (Section 4.6 of RFC 4861)
    options[1] = OPTLEN / 8;  // Option Length - units of 8 octets (RFC 4861)
    //    uint8_t bytes[6];

    if( 6 == sscanf(mac, "%x:%x:%x:%x:%x:%x%*c",
                    &values[0], &values[1], &values[2],
                    &values[3], &values[4], &values[5] ) ) {
        /* convert to uint8_t */
        for( i = 0; i < 6; ++i )
            options[i+2] = (uint8_t) values[i];
//            bytes[i] = (uint8_t) values[i];
    }

    rs = (struct nd_router_solicit *) outpack;
    memset (rs, 0, sizeof (*rs));

    // Populate icmp6_hdr portion of router solicit struct.
    rs->nd_rs_hdr.icmp6_type = ND_ROUTER_SOLICIT;  // 133 (RFC 4861)
    rs->nd_rs_hdr.icmp6_code = 0;              // zero for router solicitation (RFC 4861)
    rs->nd_rs_hdr.icmp6_cksum = htons(0);      // zero when calculating checksum
    rs->nd_rs_reserved = htonl (0);            // Reserved - must be set to zero (RFC 4861)

    // Append options to end of router solicit struct.
    memcpy (outpack + RS_HDRLEN, options, OPTLEN * sizeof (uint8_t));
    psdhdrlen = 16 + 16 + 4 + 3 + 1 + RS_HDRLEN + OPTLEN;

    // Configure server's address and ports
    memset(&server_info, 0, sizeof(server_info));
    server_info.sin6_family = AF_INET6;
    inet_pton(AF_INET6, SERVER_ADDRESS, &server_info.sin6_addr);

    // Prepare msghdr for sendmsg().
    memset (&msghdr, 0, sizeof (msghdr));
    msghdr.msg_name = &server_info;  // Destination IPv6 address (as struct sockaddr_in6)
    msghdr.msg_namelen = sizeof (server_info);
    memset (&iov, 0, sizeof (iov));
    iov[0].iov_base = (uint8_t *) outpack;  // Point msghdr to buffer outpack
    iov[0].iov_len = RS_HDRLEN + OPTLEN;
    msghdr.msg_iov = iov;                 // scatter/gather array
    msghdr.msg_iovlen = 1;                // number of elements in scatter/gather array

    // Tell msghdr we're adding cmsghdr data to change hop limit and specify interface.
    // Allocate some memory for our cmsghdr data.
    cmsglen = CMSG_SPACE (sizeof (int)) + CMSG_SPACE (sizeof (pktinfo));
    msghdr.msg_control = (uint8_t *) malloc (cmsglen * sizeof (uint8_t));
    msghdr.msg_controllen = cmsglen;

    /*
    // Change hop limit to 255 as required for router solicitation (RFC 4861).
    hoplimit = 255;
    cmsghdr1 = CMSG_FIRSTHDR (&msghdr);
    cmsghdr1->cmsg_level = IPPROTO_IPV6;
    cmsghdr1->cmsg_type = IPV6_HOPLIMIT;  // We want to change hop limit
    cmsghdr1->cmsg_len = CMSG_LEN (sizeof (int));
    *((int *) CMSG_DATA (cmsghdr1)) = hoplimit;

    // Specify source interface index for this packet via cmsghdr data.
    cmsghdr2 = CMSG_NXTHDR (&msghdr, cmsghdr1);
    cmsghdr2->cmsg_level = IPPROTO_IPV6;
    cmsghdr2->cmsg_type = IPV6_PKTINFO;  // We want to specify interface here
    cmsghdr2->cmsg_len = CMSG_LEN (sizeof (pktinfo));
    pktinfo = (pktinfo6 *) CMSG_DATA (cmsghdr2);
    pktinfo->ipi6_ifindex = ifindex;
     */

    // Compute ICMPv6 checksum (RFC 2460).
    // psdhdr[0 to 15] = source IPv6 address, set earlier.
    // psdhdr[16 to 31] = destination IPv6 address, set earlier.
    psdhdr[32] = 0;  // Length should not be greater than 65535 (i.e., 2 bytes)
    psdhdr[33] = 0;  // Length should not be greater than 65535 (i.e., 2 bytes)
    psdhdr[34] = (RS_HDRLEN + OPTLEN)  / 256;  // Upper layer packet length
    psdhdr[35] = (RS_HDRLEN + OPTLEN)  % 256;  // Upper layer packet length
    psdhdr[36] = 0;  // Must be zero
    psdhdr[37] = 0;  // Must be zero
    psdhdr[38] = 0;  // Must be zero
    psdhdr[39] = IPPROTO_ICMPV6;
    memcpy (psdhdr + 40, outpack, (RS_HDRLEN + OPTLEN) * sizeof (uint8_t));
    rs->nd_rs_hdr.icmp6_cksum = checksum ((uint16_t *) psdhdr, psdhdrlen);

    printf ("Checksum: %x\n", ntohs (rs->nd_rs_hdr.icmp6_cksum));

    if (sendto(the_socket, (char *) &rs, sizeof(rs), 0, (struct sockaddr *)&server_info, sizeof(server_info)) < 0) {
        perror("'sendto' failed");
        exit(4);
    }

    //// Send packet.
    //if (sendmsg (the_socket, &msghdr, 0) < 0) {
    //    perror ("sendmsg() failed ");
    //    exit (EXIT_FAILURE);
    //}

    close(the_socket);
    return 0;
}

// Computing the internet checksum (RFC 1071).
// Note that the internet checksum does not preclude collisions.
uint16_t checksum (uint16_t *addr, int len) {
    int count = len;
    register uint32_t sum = 0;
    uint16_t answer = 0;
    // Sum up 2-byte values until none or only one byte left.
    while (count > 1) {
        sum += *(addr++);
        count -= 2;
    }

    // Add left-over byte, if any.
    if (count > 0) {
        sum += *(uint8_t *) addr;
    }

    // Fold 32-bit sum into 16 bits; we lose information by doing this,
    // increasing the chances of a collision.
    // sum = (lower 16 bits) + (upper 16 bits shifted right 16 bits)
    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    // Checksum is one's compliment of sum.
    answer = ~sum;

    return (answer);
}
