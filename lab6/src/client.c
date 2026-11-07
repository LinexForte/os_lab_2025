#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "common.h"

struct Server {
  char ip[255];
  int port;
};

struct ThreadArgs {
  struct Server *server;
  uint64_t begin;
  uint64_t end;
  uint64_t mod;
  uint64_t result;
};

void *ThreadConnect(void *args) {
  struct ThreadArgs *targs = (struct ThreadArgs *)args;
  struct hostent *hostname = gethostbyname(targs->server->ip);
  if (hostname == NULL) {
    fprintf(stderr, "gethostbyname failed with %s\n", targs->server->ip);
    return NULL;
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(targs->server->port);
  server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr);

  int sck = socket(AF_INET, SOCK_STREAM, 0);
  if (sck < 0) {
    fprintf(stderr, "Socket creation failed!\n");
    return NULL;
  }

  if (connect(sck, (struct sockaddr *)&server, sizeof(server)) < 0) {
    fprintf(stderr, "Connection failed\n");
    close(sck);
    return NULL;
  }

  char task[sizeof(uint64_t) * 3];
  memcpy(task, &targs->begin, sizeof(uint64_t));
  memcpy(task + sizeof(uint64_t), &targs->end, sizeof(uint64_t));
  memcpy(task + 2 * sizeof(uint64_t), &targs->mod, sizeof(uint64_t));

  if (send(sck, task, sizeof(task), 0) < 0) {
    fprintf(stderr, "Send failed\n");
    close(sck);
    return NULL;
  }

  char response[sizeof(uint64_t)];
  if (recv(sck, response, sizeof(response), 0) < 0) {
    fprintf(stderr, "Recieve failed\n");
    close(sck);
    return NULL;
  }

  memcpy(&targs->result, response, sizeof(uint64_t));
  close(sck);
  return NULL;
}

int main(int argc, char **argv) {
  uint64_t k = -1;
  uint64_t mod = -1;
  char servers[255] = {'\0'};

  while (true) {
    int current_optind = optind ? optind : 1;
    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"servers", required_argument, 0, 0},
                                      {0, 0, 0, 0}};
    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);
    if (c == -1)
      break;
    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        ConvertStringToUI64(optarg, &k);
        break;
      case 1:
        ConvertStringToUI64(optarg, &mod);
        break;
      case 2:
        memcpy(servers, optarg, strlen(optarg) < 255 ? strlen(optarg) : 255);
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;
    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (k == -1 || mod == -1 || !strlen(servers)) {
    fprintf(stderr, "Using: %s --k 1000 --mod 5 --servers /path/to/file\n", argv[0]);
    return 1;
  }

  FILE *f = fopen(servers, "r");
  if (!f) {
    perror("fopen");
    return 1;
  }

  struct Server *to = malloc(sizeof(struct Server) * 100);
  unsigned int servers_num = 0;
  char line[256];
  while (fgets(line, sizeof(line), f) && servers_num < 100) {
    char *colon = strchr(line, ':');
    if (colon) {
      *colon = '\0';
      strcpy(to[servers_num].ip, line);
      to[servers_num].port = atoi(colon + 1);
      servers_num++;
    }
  }
  fclose(f);

  if (servers_num == 0) {
    fprintf(stderr, "No servers found in file\n");
    free(to);
    return 1;
  }

  pthread_t threads[servers_num];
  struct ThreadArgs args[servers_num];

  uint64_t chunk = k / servers_num;
  uint64_t remainder = k % servers_num;
  uint64_t current = 1;

  for (unsigned int i = 0; i < servers_num; i++) {
    uint64_t part = chunk + (i < remainder ? 1 : 0);
    args[i].server = &to[i];
    args[i].begin = current;
    args[i].end = current + part - 1;
    args[i].mod = mod;
    args[i].result = 0;
    current += part;

    if (pthread_create(&threads[i], NULL, ThreadConnect, &args[i]) != 0) {
      fprintf(stderr, "pthread_create failed\n");
      return 1;
    }
  }

  uint64_t total = 1;
  for (unsigned int i = 0; i < servers_num; i++) {
    pthread_join(threads[i], NULL);
    total = MultModulo(total, args[i].result, mod);
  }

  printf("Total: %llu\n", total);

  free(to);
  return 0;
}