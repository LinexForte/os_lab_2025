#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
  pid_t pid = fork();

  if (pid < 0) {
    perror("fork");
    return 1;
  }

  if (pid == 0) {
    printf("Child: PID = %d, exiting...\n", getpid());
    exit(0);
  }

  printf("Parent: PID = %d, child PID = %d\n", getpid(), pid);
  printf("Parent: sleeping 10 seconds without wait()...\n");
  printf("Run in another terminal: ps aux | grep %d\n", pid);
  sleep(20);

  printf("Parent: now calling wait()...\n");
  int status;
  waitpid(pid, &status, 0);
  printf("Parent: child reaped, status = %d\n", WEXITSTATUS(status));

  return 0;
}