#include "networking.h"

int client_socket1 = -1;
int client_socket2 = -1;

static void sighandler(int signo) {
  if(client_socket1>=0)close(client_socket1);
  if(client_socket2>=0)close(client_socket2);
  exit(0);
}

char* rot13(char*s) {
  for(int i=0; s[i]&&s[i]!=10;i++) {
    //printf("%d: %d\n",i,s[i]);
    if(s[i]==' ')continue;
    int lower = s[i] >= 97 ? 1 : 0;
    int j = (int)s[i]+13;
    if(lower && j>'z') {
      s[i]=j-'z'+'a';
    } else if (!lower && j>'Z') {
      s[i]=j-'Z'+'A';
    }
    else {
      s[i]=j;
    }
  }
  return s;
}

void subserver_logic(int client_socket1, int client_socket2){
  printf("Forked!\n");
  char sendtoclient1[256];
  char sendtoclient2[256];
  int readstate = 1;
  int blockstate = 0;
  int writestate = 2;
  while(1) {
    //below is the turn logic when client1s turn
    int n = write(client_socket1, &readstate,sizeof(int));
    err(n,"error setting turn to 1st client when client1 turn");
    n = write(client_socket2, &blockstate,sizeof(int));
    err(n,"error setting block to 2nd client when client1 turn");

    // read from client1 and rotate response
    n = read(client_socket1,sendtoclient2,sizeof(sendtoclient2));
    err(n,"error reading client1 initial");
    if(!n)break;

    //below is the turn logic when client2s turn
    n = write(client_socket1, &blockstate,sizeof(int));
    err(n,"error setting block to 1st client when client2 turn");
    n = write(client_socket2, &readstate,sizeof(int));
    err(n,"error setting turn to 2nd client when client2 turn");

    // read from client2 and rotate response
    n = read(client_socket2,sendtoclient1,sizeof(sendtoclient1));
    err(n,"error reading client2 initial");
    if(!n)break;

    //set turn to write so clients can read the other clients response
    n = write(client_socket1, &writestate,sizeof(int));
    err(n,"error setting turn to 1st client when write turn");
    n = write(client_socket2, &writestate,sizeof(int));
    err(n,"error setting turn to 2nd client when write turn");

    //write to both clients
    int byte = write(client_socket1,sendtoclient1,sizeof(sendtoclient1));
    err(byte,"subserver_logic write to client 1 error");
    byte = write(client_socket2,sendtoclient2,sizeof(sendtoclient2));
    err(byte,"subserver_logic write to client 2 error");

    //end off by blocking both clients
    n = write(client_socket1, &blockstate,sizeof(int));
    err(n,"error setting turn to 1st client when block turn");
    n = write(client_socket2, &blockstate,sizeof(int));
    err(n,"error setting turn to 2nd client when block turn");
    continue;
  }
  printf("Subserver socket closed.\n");
  close(client_socket1);
  close(client_socket2);
  exit(99);
}

int main(int argc, char *argv[] ) {
  signal(SIGINT,sighandler);
  int listen_socket = server_setup();
  printf("Main Server set up!\n");


  while(1) {
    printf("Main server waiting for connection...\n");
    client_socket1 = server_tcp_handshake(listen_socket);
    client_socket2 = server_tcp_handshake(listen_socket);
    printf("Game 1 made with two clients!\n") //make this print out the pids of both clients soon
    pid_t p = fork();
    if(p<0){//error
      perror("fork");
      close(client_socket1);
      close(client_socket2);
      exit(99);
    }
    if(p) {//parent
      close(client_socket1);
      client_socket1 = -1;
      close(client_socket2);
      client_socket2 = -1;
      continue;
    }
    //child/subserver does
    subserver_logic(client_socket1,client_socket2);
    continue;
  }

}
