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
void gameover_screen(int won){
  printf("\n===========\n");
  if(won) printf("    EZ WIN\n");
  else printf("U GOT SMOKED\n");
  printf("\n===========\n");
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
          int valid = 3;
          for(int i = 0; i < 3; i++){
            char col = ships[i][0];
            int row = ships[i][1];
            if(ships[i][2] != '\0' || col < 'A' || col > 'C' || row < '1' || row >'3'){
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
            char col = ships[i][0];
            int row = ships[i][1];
            if(col == 'A') place_ship(&myBoard, 0, row);
            if(col == 'B') place_ship(&myBoard, 1, row);
            if(col == 'C') place_ship(&myBoard, 2, row);
          }
          break;
    }
    }


void clientLogic(int server_socket){
  struct Board myBoard;
  struct Board enemyBoard;
  clear_board(&myBoard);
  clear_board(&enemyBoard);
  printf("Both clients connected! Game started.\n");
  place_ships(&myBoard);
  int row,col;
  while(1){
    int turn;
    int bytes = read(server_socket, &turn, sizeof(int));
    if(bytes<=0)break;
    if(turn == WRITE){
      char move[8];
      printf("\nYour Board:\n");
      print_board(&myBoard);
      printf("\nEnemy Board:\n");
      print_board(&enemyBoard);
      printf("Your turn! Enter coordinate to hit (ex B3): ");
      fgets(move, sizeof(move), stdin);
      if(move[0] < 'A' || move[0] > 'C' ||move[1] < '0' || move[1] > 'C2'){
        printf("Invald input. Enter exactly one coordinate in bounds.\n");
        continue;
      }
      int final_row = move[0] - 'A';
      int final_row = move[1] - '1';
      write(server_socket, &last_row, sizeof(int));
      write(server_socket, &last_col, sizeof(int));
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
  }
  clientLogic(server_socket);

  return 0;
}
