#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <pty.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>
#include <wait.h>

int main(void){
  int master;
  pid_t pid = forkpty(&master,NULL,NULL,NULL);
  if (pid == 0){
    execl("bin/bash","bash",NULL);
    exit(1);
  }
  char buffer[4000];
  while (1){
    ssize_t readb = read(master,buffer,sizeof(buffer) - 1);
    buffer[readb] = '\0';
    printf("%s",buffer);
    fflush(stdout);
  }
  waitpid(pid,NULL,0);
  close(master);
  return 0;
}
