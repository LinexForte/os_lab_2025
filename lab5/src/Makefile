CC = gcc
CFLAGS = -Wall
LDFLAGS = -lpthread

all: mutex_nolock mutex_lock factorial deadlock

mutex_nolock: mutex.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

mutex_lock: mutex_lock.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

factorial: factorial.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

deadlock: deadlock.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f mutex_nolock mutex_lock factorial deadlock

.PHONY: all clean