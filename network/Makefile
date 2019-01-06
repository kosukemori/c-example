CC=gcc
TARGETS=ipv4-tcp-client ipv4-tcp-server ipv4-udp-client ipv4-udp-server ipv6-udp-client ipv6-udp-server ipv6-tcp-client ipv6-tcp-server

all: $(TARGETS)

$(TARGETS):
	$(CC) -o $@.out $@.c

clean: FORCE
	rm -f *.out

FORCE:
