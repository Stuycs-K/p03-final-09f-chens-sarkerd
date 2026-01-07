#include "networking.h"

int server_socket = -1;

static void sighandler(int signo) {
  printf("Client closed.\n");
  if(server_socket>=0)close(server_socket);
  exit(0);
}

void clientLogic(int server_socket){
  while(1) {
    int id=-1;
    int r = read(server_socket,&id,sizeof(int));
    err(r,"error setting id");
    if(id==2) {
      r = read(server_socket,NULL,256);//this is the waiting system, work from here
    }
    char buffer[256];
    printf("Enter a message:");
    if(!fgets(buffer,sizeof(buffer),stdin))break;
    int w = write(server_socket,buffer,sizeof(buffer));
    err(w,"write error in clientLogic");
    int r = read(server_socket,buffer,sizeof(buffer));
    err(r,"read error in clientLogic");
    if(!r)break;
    printf("Recieved: %s",buffer);
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
  clientLogic(server_socket);

  return 0;
}
