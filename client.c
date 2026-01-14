#include "networking.h"
#include "game.h"
int server_socket = -1;
int otherclientconnected=0;

//problem is that turn is setting to 555 when its supposed to be gamestate, so i think that is making it a bit wonky also need to test win/lose con but the game is working

static void sighandler(int signo) {
  printf("Client closed.\n");
  if(server_socket>=0)close(server_socket);
  exit(0);
}
void mark_enemyboard(struct Board *b, int row, int col, int hit){
  if(hit) b->grid[row][col] = 'X';
  else b->grid[row][col] = 'O';
}

void place_ships(struct Board *b){
  char buff[64];
  char ships[3][3];
  while(1) {
        printf("Coordinates are letters for columns and numbers for rows, from A-C and 1-3.\n");
        printf("Enter 3 Ship Coordinates(seperate by space) ex. A1 B2 C3: ");
        if(fgets(buff, sizeof(buff), stdin) == NULL){
          printf("Input error, try again.\n");
          continue;
        }
        int parsed = sscanf(buff,"%2s %2s %2s",ships[0],ships[1],ships[2]); //%2 limits it to two chars in the str
          if(parsed != 3){
            printf("Invalid input. Enter exactly 3 unique coordinates in bounds separated by spaces.\n");
            continue;
          }
          //WHOLE THING UP TILL 2ND FOR LOOP IS FOR CHECKING VALIDITY
          int valid = 3;
          for(int i = 0; i < 3; i++){
            char col = ships[i][0]-'A';
            int row = ships[i][1]-'1';
            if(ships[i][2] != '\0' || col < 0 || col > 2 || row < 0 || row > 2){
              valid--;
              break;
            }
          }
          if(valid != 3){
            printf("Invald input. Enter exactly 3 unique coordinates in bounds separated by spaces.\n");
            continue;
          }
          if( strcmp(ships[0],ships[1]) == 0
            || strcmp(ships[2],ships[1]) == 0
            ||strcmp(ships[0],ships[2]) == 0 ){
            printf("Invald input. Enter exactly 3 unique coordinates in bounds separated by spaces.\n");
            continue;
          }
          for(int i = 0; i < 3; i++){
            char col = ships[i][0]-'A';
            int row = ships[i][1]-'1';
            place_ship(b, row, col);
          }
          break;
    }
    //printf("board rn placeship\n");
    //print_board(b);
    }


void clientLogic(int server_socket){
  printf("Game started!\n");
  int turn;
  int gameState;
  struct Board myBoard;
  struct Board enemyBoard;
  clear_board(&myBoard);
  clear_board(&enemyBoard);
  int bytes = read(server_socket,&turn,sizeof(int));
  err(bytes,"idk starting read\n");
  if(turn==WAIT)printf("Wait til it is your turn to set ships\n");
  while(turn==WAIT) {
    bytes = read(server_socket,&turn,sizeof(int));
    err(bytes,"idk starting read loop\n");
  }
  place_ships(&myBoard);
  printf("Ships set!\n");
  print_board(&myBoard);
  bytes = write(server_socket,&myBoard,sizeof(struct Board));
  turn=WAIT;
  printf("Successfuly sent initial board!\n");

  bytes = read(server_socket,&turn,sizeof(int));
  err(bytes,"problem reading enemy board\n");
  while(turn==WAIT) {
    bytes = read(server_socket,&turn,sizeof(int));
    err(bytes,"problem storing enemy board \n");
  }
  bytes = read(server_socket,&enemyBoard,sizeof(struct Board));
  //printf("enemy board rn:");
  //print_board(&enemyBoard);

  //NOTE: EVERYTHING ABOVE WORKS

  while(1){
    bytes = read(server_socket, &turn, sizeof(int));
    err(bytes,"ERROR SETTING TURN");
    if(!bytes)break;
    //printf("turn: %d\n",turn);
    if(turn==WAIT){
      //printf("wait loop\n");
      continue;
    }
    if(turn == WRITE){
      char move[3];
      printf("\nEnemies Board:\n");
      print_board(&enemyBoard);
      printf("Your turn! Enter coordinate to hit (ex B3): ");
      scanf("%2s",move);
      if(move[0] < 'A' || move[0] > 'C' ||move[1] < '1' || move[1] > '3'){
        printf("Invald input. Enter exactly one coordinate in bounds.\n");
        continue;
      }

      bytes = write(server_socket, &move, sizeof(move));
      err(bytes,"ERROR WRITING MOVE TO SERVER");
    }

    if(turn==READ) {
      bytes = read(server_socket,&myBoard,sizeof(struct Board));
      err(bytes,"ERROR READING MYBOARD");
      bytes = read(server_socket,&enemyBoard,sizeof(struct Board));
      err(bytes,"ERROR READING ENEMYBOARD");

      printf("Here is your board now:\n");
      print_board(&myBoard);
      continue;
    }
  }

  if(turn==CHECK) {
      bytes = read(server_socket,&gameState,sizeof(int));
      err(bytes,"ERROR CHECKING");
      if(gameState==LOSE) {
        printf("You lost!\n");
      }
      if(gameState==WIN) {
        printf("You won!\n");
      }
  }

  printf("Game end.\n");
  close(server_socket);
  exit(0);
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
    if(!n)printf("BRUH.\n");
  }
  clientLogic(server_socket);

  return 0;
}
