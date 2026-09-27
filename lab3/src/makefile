CC = gcc
CFLAGS = -I. -Wall

all: sequential_min_max parallel_min_max run_sequential

sequential_min_max: sequential_min_max.o find_min_max.o utils.o
	$(CC) $(CFLAGS) -o $@ $^

parallel_min_max: parallel_min_max.o find_min_max.o utils.o
	$(CC) $(CFLAGS) -o $@ $^

run_sequential: run_sequential.o
	$(CC) $(CFLAGS) -o $@ $^

sequential_min_max.o: sequential_min_max.c find_min_max.h utils.h
	$(CC) $(CFLAGS) -c $< -o $@

parallel_min_max.o: parallel_min_max.c find_min_max.h utils.h
	$(CC) $(CFLAGS) -c $< -o $@

find_min_max.o: find_min_max.c find_min_max.h utils.h
	$(CC) $(CFLAGS) -c $< -o $@

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c $< -o $@

run_sequential.o: run_sequential.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o sequential_min_max parallel_min_max run_sequential min_max_*.txt

.PHONY: all clean