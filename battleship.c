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

int prompt(){
  printf("Format using two coordinates as shown: A2 A1\n");  //prompt ship location, parse, put it into a list, add another one to list for second parser
  printf("Location for Ship (Size 2):");
  char buff[256];
  fgets(buff, sizeof(buff), stdin);
  if(strlen(buff)!=5){//prompt again
    printf("Invalid Input\n");
      printf("Location for Ship (Size 2):");
  }
  if (pointer == NULL) {
    exit(0);
  }
}

void place(){

}


int main(int argc, char *argv[]) {
  char buff[256];
  int grid[3][3];
  int opp_grid[3][3];
  signal(SIGINT, sigint_handler);

}
