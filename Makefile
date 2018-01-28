CC=gcc

all: ipv6-udp-server.out ipv6-udp-client.out start

start:
	./ipv6-udp-server.out

ipv6-udp-server.out: FORCE
	$(CC) -w -o ipv6-udp-server.out ipv6-udp-server.c

ipv6-udp-client.out: FORCE
	$(CC) -o ipv6-udp-client.out ipv6-udp-client.c

FORCE:

clean:
	rm -f *.out
