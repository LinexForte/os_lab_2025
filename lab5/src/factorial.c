#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

long long result = 1;
long long mod_value = 1;
pthread_mutex_t result_mutex = PTHREAD_MUTEX_INITIALIZER;

struct ThreadArgs {
  int begin;
  int end;
};

void *ThreadFactorial(void *args) {
  struct ThreadArgs *ta = (struct ThreadArgs *)args;
  long long local = 1;
  for (int i = ta->begin; i <= ta->end; i++) {
    local = (local * i) % mod_value;
  }
  pthread_mutex_lock(&result_mutex);
  result = (result * local) % mod_value;
  pthread_mutex_unlock(&result_mutex);
  return NULL;
}

int main(int argc, char **argv) {
  int k = -1;
  int pnum = -1;
  long long mod = -1;

  static struct option options[] = {
      {"pnum", required_argument, 0, 0},
      {"mod", required_argument, 0, 0},
      {0, 0, 0, 0}};

  int option_index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "k:", options, &option_index)) != -1) {
    switch (c) {
      case 0:
        if (option_index == 0) {
          pnum = atoi(optarg);
        } else if (option_index == 1) {
          mod = atoll(optarg);
        }
        break;
      case 'k':
        k = atoi(optarg);
        break;
      default:
        printf("Usage: %s -k K --pnum=N --mod=M\n", argv[0]);
        return 1;
    }
  }

  if (k <= 0 || pnum <= 0 || mod <= 0) {
    printf("Usage: %s -k K --pnum=N --mod=M\n", argv[0]);
    return 1;
  }

  mod_value = mod;
  pthread_t threads[pnum];
  struct ThreadArgs args[pnum];

  int chunk = k / pnum;
  int remainder = k % pnum;

  for (int i = 0; i < pnum; i++) {
    int begin = i * chunk + (i < remainder ? i : remainder) + 1;
    int end = begin + chunk + (i < remainder ? 1 : 0) - 1;
    args[i].begin = begin;
    args[i].end = end;
    if (pthread_create(&threads[i], NULL, ThreadFactorial, &args[i]) != 0) {
      perror("pthread_create");
      return 1;
    }
  }

  for (int i = 0; i < pnum; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Result: %lld\n", result);
  return 0;
}