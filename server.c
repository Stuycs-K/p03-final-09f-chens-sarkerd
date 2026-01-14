#include "networking.h"
#include "game.h"

int client_socket1 = -1;
int client_socket2 = -1;
int writestate = WRITE;
int waitstate = WAIT;
int readstate = READ;
int checkstate = CHECK;
struct Board Board1;
struct Board HiddenBoard1;
struct Board Board2;
struct Board HiddenBoard2;
char c1move[2];
char c2move[2];

static void sighandler(int signo) {
  if(client_socket1>=0)close(client_socket1);
  if(client_socket2>=0)close(client_socket2);
  exit(0);
}

void HideBoard(struct Board *Board, struct Board *HiddenBoard) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        HiddenBoard->grid[i][j] = Board->grid[i][j];
        if(HiddenBoard->grid[i][j]=='S')HiddenBoard->grid[i][j]='.';
    }
}
//printf("BOARD:\n");
//print_board(Board);
//printf("HIDDENBOARD:\n");
//print_board(HiddenBoard);
}

void handle_attack(char* move, struct Board *b){
  int row = move[1]-'1';
  int col = move[0]-'A';
  int hit = fire(b,row,col);
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

int isGameOver(struct Board *Board1, struct Board *Board2) {
  int lose = LOSE;
  int win = WIN;
  int none = NONE;
  int n;
  if(game_over(Board1)) {//if client1 lost
    n = write(client_socket1, &checkstate,sizeof(int));
    err(n,"error setting turn to 1st client when client1 loses");
    n = write(client_socket2, &checkstate,sizeof(int));
    err(n,"error setting turn to 2nd client when client1 loses");
    printf("Client 2 Wins.\n");
    write(client_socket1,&lose,sizeof(int));
    write(client_socket2,&win,sizeof(int));
    close(client_socket1);
    close(client_socket2);
    return 1;
  }
  if(game_over(Board2)) {//if client2 lost
    n = write(client_socket1, &checkstate,sizeof(int));
    err(n,"error setting turn to 1st client when client2 loses");
    n = write(client_socket2, &checkstate,sizeof(int));
    err(n,"error setting turn to 2nd client when client2 loses");
    printf("Client 1 Wins.\n");
    write(client_socket1,&win,sizeof(int));
    write(client_socket2,&lose,sizeof(int));
    close(client_socket1);
    close(client_socket2);
    return 1;
  }
  else {//if neither lost
    n = write(client_socket1, &checkstate,sizeof(int));
    err(n,"error setting turn to 1st client when neither loses");
    n = write(client_socket2, &checkstate,sizeof(int));
    err(n,"error setting turn to 2nd client when neither loses");
    write(client_socket1,&none,sizeof(int));
    write(client_socket2,&none,sizeof(int));
    n = write(client_socket1, &waitstate,sizeof(int));
    err(n,"error setting turn to 1st client when block turn | CHECK");
    n = write(client_socket2, &waitstate,sizeof(int));
    err(n,"error setting turn to 2nd client when block turn | CHECK");
    return 0;
  }
}

void gameSetupServer() {
  //turn logic for client1 board set
  int n = write(client_socket1, &writestate,sizeof(int));
  err(n,"error setting turn to 1st client when client1 turn");
  n = write(client_socket2, &waitstate,sizeof(int));
  err(n,"error setting block to 2nd client when client1 turn");

  // Set client1 board
  n = read(client_socket1,&Board1,sizeof(struct Board));
  err(n,"error reading client1s initial board");
  if(!n)exit(9);
  //printf("c1 board rn initial\n");
  //print_board(&Board1);

  //turn logic for client2 board set
  n = write(client_socket1, &waitstate,sizeof(int));
  err(n,"error setting turn to 1st client when client2 turn");
  n = write(client_socket2, &writestate,sizeof(int));
  err(n,"error setting block to 2nd client when client2 turn");
  //printf("GOT HERE\n");

  // Set client2 board
  n = read(client_socket2,&Board2,sizeof(struct Board));
  err(n,"error reading client2s initial board");
  if(!n)exit(9);
  //printf("GOT HERE 2\n");
  //printf("c2 board rn initial\n");
  //print_board(&Board2);

  //NOTE TO SELF INITIALS WORK SO EVERYTHING ABOVE WORKS 100%

  //set turn to read for both so u can write the enemy boards
  n = write(client_socket1, &readstate,sizeof(int));
  err(n,"error setting turn to 1st client when initial board read turn");
  n = write(client_socket2, &readstate,sizeof(int));
  err(n,"error setting turn to 2nd client when initial board read turn");

  //HIDE boards
  HideBoard(&Board1,&HiddenBoard1);
  HideBoard(&Board2,&HiddenBoard2);
  printf("Hidden board1 rn\n");
  print_board(&HiddenBoard1);
  printf("Hidden board2 rn\n");
  print_board(&HiddenBoard2);


  //WRITE HIDDEN BOARD TO RESPECTIVE BOARDS
  n = write(client_socket1,&HiddenBoard2,sizeof(struct Board));
  err(n,"initial subserver_logic write client2 board to client1");
  n = write(client_socket2,&HiddenBoard1,sizeof(struct Board));
  err(n,"initial subserver_logic write client2 board to client1");

  //end off by blocking both clients
  n = write(client_socket1, &waitstate,sizeof(int));
  err(n,"error setting turn to 1st client when initial block turn");
  n = write(client_socket2, &waitstate,sizeof(int));
  err(n,"error setting turn to 2nd client when initial block turn");

  printf("Finish setup\n");
}


void subserver_logic(){

  gameSetupServer();

  while(1) {
    /*
    if(isclosed(client_socket1))close(client_socket2);
    if(isclosed(client_socket2))close(client_socket1);
    */

    //below is the turn logic when client1s turn
    int n = write(client_socket1, &writestate,sizeof(int));
    err(n,"error setting turn to 1st client when client1 turn");
    n = write(client_socket2, &waitstate,sizeof(int));
    err(n,"error setting block to 2nd client when client1 turn");

    // read from client1 and attack
    n = read(client_socket1,c1move,sizeof(c1move));
    err(n,"error reading client1s move");
    if(!n)break;

    handle_attack(c1move,&Board2);

    if(isGameOver(&Board1,&Board2)) break;

    //below is the turn logic when client2s turn
    n = write(client_socket1, &waitstate,sizeof(int));
    err(n,"error setting turn to 1st client when client2 turn");
    n = write(client_socket2, &writestate,sizeof(int));
    err(n,"error setting turn to 2nd client when client2 turn");

    // read from client2 and rotate response
    n = read(client_socket2,c2move,sizeof(c2move));
    err(n,"error reading client2s move");
    if(!n)break;

    handle_attack(c2move,&Board1);
    if(isGameOver(&Board1,&Board2)) break; // maybe we have to read the gameover message from game.c into clients after

    //set turn to read for both so u can write the new boards
    n = write(client_socket1, &readstate,sizeof(int));
    err(n,"error setting turn to 1st client when write turn");
    n = write(client_socket2, &readstate,sizeof(int));
    err(n,"error setting turn to 2nd client when write turn");

    //HIDE boards
    HideBoard(&Board1,&HiddenBoard1);
    HideBoard(&Board2,&HiddenBoard2);
    //printf("Hidden board1 rn\n");
    //print_board(&HiddenBoard1);
    //printf("Hidden board2 rn\n");
    //print_board(&HiddenBoard2);
    //printf("Board1 rn\n");
    //print_board(&Board1);
    //printf("Board2 rn\n");
    //print_board(&Board2);

    //write to both clients
    int byte = write(client_socket1,&Board1,sizeof(struct Board));
    err(byte,"subserver_logic write client1 board to client1");
    byte = write(client_socket1,&HiddenBoard2,sizeof(struct Board));
    err(byte,"subserver_logic write client2 board to client1");
    byte = write(client_socket2,&Board2,sizeof(struct Board));
    err(byte,"subserver_logic write client2 board to client2");
    byte = write(client_socket2,&HiddenBoard1,sizeof(struct Board));
    err(byte,"subserver_logic write client1 board to client2");

    //Ok both wait now
    n = write(client_socket1, &waitstate,sizeof(int));
    err(n,"error setting turn to 1st client when wait turn");
    n = write(client_socket2, &waitstate,sizeof(int));
    err(n,"error setting turn to 2nd client when Wait turn");
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
    printf("One client connected!\n");
    int n = write(client_socket1,&i,sizeof(int));
    client_socket2 = server_tcp_handshake(listen_socket);
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
    subserver_logic();
    continue;
  }
  return 0;
}
