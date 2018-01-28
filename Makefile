CC=gcc
TARGETS=ipv4-udp-client ipv6-udp-server ipv6-udp-client

all: $(TARGETS)

$(TARGETS):
	$(CC) -o $@.out $@.c

clean: FORCE
	rm -f *.out

FORCE:
