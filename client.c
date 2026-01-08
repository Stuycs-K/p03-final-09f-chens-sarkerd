#include "networking.h"
#define WRITE 1
#define WAIT 0
#define READ 2
int server_socket = -1;
int otherclientconnected=0;

static void sighandler(int signo) {
  printf("Client closed.\n");
  if(server_socket>=0)close(server_socket);
  exit(0);
}

void clientLogic(int server_socket){
  printf("Both clients connected! Game started.\n");
  int turn;
  char buffer[256];
  while(1) {
    int curTurn; //idea for printing that its not your turn
    int r = read(server_socket,&turn,sizeof(int));
    err(r,"error reading turn in client");

    if(turn==WRITE) {
      printf("Enter a message:");
      if(!fgets(buffer,sizeof(buffer),stdin))break;
      int w = write(server_socket,buffer,sizeof(buffer));
      err(w,"write error in clientLogic");
    }

    else if(turn==WAIT) {
      continue;
    }

    else if(turn==READ) {
      int r = read(server_socket,buffer,sizeof(buffer));
      err(r,"read error in clientLogic");
      if(!r)break;
      printf("Recieved: %s",buffer);
    }
    else {
      printf("TURN ISNT ONE OF THE CHOICES.\n");
      break;
    }
  }
  printf("Client closed.\n");
  close(server_socket);
  exit(99);
}

int main(int argc, char *argv[] ) {
  signal(SIGINT,sighandler);
  char* IP = "127.0.0.1";
  if(argc>1){
    IP=argv[1];
  }
  server_socket = client_tcp_handshake(IP);
  printf("Connection made with server!\n");
  int n = read(server_socket,&otherclientconnected,sizeof(int));
  err(n,"initial otherclientconnected setup");
  if(!otherclientconnected)printf("Waiting for other client to connect...\n");
  while(!otherclientconnected) {
    n = read(server_socket,&otherclientconnected,sizeof(int));
    err(n,"loop otherclientconnected setup");
  }
  clientLogic(server_socket);

  return 0;
}
