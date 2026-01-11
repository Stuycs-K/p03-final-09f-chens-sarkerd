#include "networking.h"
#include "game.h"

int client_socket1 = -1;
int client_socket2 = -1;

static void sighandler(int signo) {
  if(client_socket1>=0)close(client_socket1);
  if(client_socket2>=0)close(client_socket2);
  exit(0);
}

// server handles everything, client only displays
void receive_ships(int sock, struct Board *b){ //takes initial 3 coordinates and places ships there
  clear_board(b);
  for(int i = 0; i<3;i++){
    int row;
    int col;
    read(sock,&row,sizeof(int));
    read(sock,&col,sizeof(int));
    place_ship(b,row,col);
  }

}

int handle_attack(int att, int def, struct Board *b){ //handle from defender side
  int row,col;
  read(att, &row,sizeof(int));
  read(att,&col,sizeof(int));
  int hit = fire(b,row,col);
  if(game_over(b)){
    write(att,"WIN",4);
    write(def,"LOSE",5);
    return 1;
  }
  if(hit) write(att,"HIT",4);
  else write(att,"MISS",5);
  return 0;
}

int isclosed(int fd) {
  char c;
  int value = read(fd,&c,1);
  if(c<=0 || errno==EBADF) {
    return 1;
  }
  else {
    return 0;
  }
}

void subserver_logic(int client_socket1, int client_socket2){
  struct Board Board1;
  struct Board HiddenBoard1;
  struct Board Board2;
  struct Board HiddenBoard2;
  printf("Forked!\n");
  int writestate = 1;
  int blockstate = 0;
  int readstate = 2;
  //SET BOARDS TO BE THE BOARD

  //turn logic for client1 board set
  int n = write(client_socket1, &writestate,sizeof(int));
  err(n,"error setting turn to 1st client when client1 turn");
  n = write(client_socket2, &blockstate,sizeof(int));
  err(n,"error setting block to 2nd client when client1 turn");

  // Set client1 board
  n = read(client_socket1,&Board1,sizeof(struct Board));
  err(n,"error reading client1s initial board");
  if(!n)exit(9);

  //turn logic for client2 board set
  int n = write(client_socket1, &blockstate,sizeof(int));
  err(n,"error setting turn to 1st client when client2 turn");
  n = write(client_socket2, &writestate,sizeof(int));
  err(n,"error setting block to 2nd client when client2 turn");

  // Set client2 board
  n = read(client_socket1,&Board2,sizeof(struct Board));
  err(n,"error reading client2s initial board");
  if(!n)exit(9);

  //end off by blocking both clients
  n = write(client_socket1, &blockstate,sizeof(int));
  err(n,"error setting turn to 1st client when initial block turn");
  n = write(client_socket2, &blockstate,sizeof(int));
  err(n,"error setting turn to 2nd client when initial block turn");

  while(1) {
    /*
    if(isclosed(client_socket1))close(client_socket2);
    if(isclosed(client_socket2))close(client_socket1);
    */

    //below is the turn logic when client1s turn
    int n = write(client_socket1, &writestate,sizeof(int));
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
    n = write(client_socket2, &writestate,sizeof(int));
    err(n,"error setting turn to 2nd client when client2 turn");

    // read from client2 and rotate response
    n = read(client_socket2,sendtoclient1,sizeof(sendtoclient1));
    err(n,"error reading client2 initial");
    if(!n)break;

    //set turn to write so clients can read the other clients response
    n = write(client_socket1, &readstate,sizeof(int));
    err(n,"error setting turn to 1st client when write turn");
    n = write(client_socket2, &readstate,sizeof(int));
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
    int i = 0;
    int n = write(client_socket1,&i,sizeof(int));
    client_socket2 = server_tcp_handshake(listen_socket);
    printf("Game 1 made with two clients!\n") //make this print out the pids of both clients soon
    i = 1;
    n = write(client_socket1,&i,sizeof(int));
    n = write(client_socket2,&i,sizeof(int));
    printf("Connection made with both clients!\n");
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
