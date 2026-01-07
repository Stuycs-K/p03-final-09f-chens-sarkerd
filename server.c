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
  int first1 = 1;
  int first2 = 1;
  while(1) {
    int idfor2nd=2;
    int idfor1st=1;
    int n = write(client_socket1, &idfor1st,sizeof(int));
    err(n,"error writing to 1st client");
    n = write(client_socket2, &idfor2nd,sizeof(int));
    err(n,"error writing to 2nd client");

    char res[256];
    n = read(client_socket1, res, sizeof(res));
    printf("1st client 1st input: %s",res);
    err(n,"subserver_logic read error");
    if(!n)break;
    rot13(res);
    int byte = write(client_socket,res,n);
    err(byte,"subserver_logic write error");
    continue;
  }
  printf("Socket closed.\n");
  close(client_socket);
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
    printf("Connection made with client! Forking...\n");
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
