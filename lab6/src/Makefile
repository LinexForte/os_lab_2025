CC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -pthread

all: client server

client: client.o common.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

server: server.o common.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

client.o: client.c common.h
	$(CC) $(CFLAGS) -c $< -o $@

server.o: server.c common.h
	$(CC) $(CFLAGS) -c $< -o $@

common.o: common.c common.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o client server

.PHONY: all clean