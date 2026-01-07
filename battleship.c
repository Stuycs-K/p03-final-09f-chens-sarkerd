#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>

void sigint_handler(int sig) {
    printf("\n");
    fflush(stdout);
	pprompt();
}

int ship_location(char* pos){
  
}


int main(int argc, char *argv[]) {
  char buff[256];
  signal(SIGINT, sigint_handler);

}
