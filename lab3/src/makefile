CC = gcc
CFLAGS = -I. -Wall
LDFLAGS = -lpthread

all: parallel_min_max process_memory zombie_demo psum

parallel_min_max: parallel_min_max.o find_min_max.o utils.o
	$(CC) $(CFLAGS) -o $@ $^

psum: parallel_sum.o sum_lib.o utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

process_memory: process_memory.c
	$(CC) $(CFLAGS) -o $@ $<

zombie_demo: zombie_demo.c
	$(CC) $(CFLAGS) -o $@ $<

parallel_min_max.o: parallel_min_max.c find_min_max.h utils.h
	$(CC) $(CFLAGS) -c $< -o $@

parallel_sum.o: parallel_sum.c sum_lib.h utils.h
	$(CC) $(CFLAGS) -c $< -o $@

sum_lib.o: sum_lib.c sum_lib.h
	$(CC) $(CFLAGS) -c $< -o $@

find_min_max.o: find_min_max.c find_min_max.h utils.h
	$(CC) $(CFLAGS) -c $< -o $@

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o parallel_min_max process_memory zombie_demo psum min_max_*.txt

.PHONY: all clean