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
    Board myBoard;
    clear_board(&myBoard);
    printf("Place your ships!\n");
    while(1){
      printf("Coordinates are letters for columns and numbers for rows, from A-C and 1-3.\n");
      printf("Enter 3 Ship Coordinates(seperate by space) ex. A1 B2 C3: ");
      char buff[64];
      if(fgets(buff, sizeof(buff), stdin) == NULL){
        printf("Input error, try again.\n");
        continue;
      }
      char ships[3][3];
      int parsed = sscanf(buff,"%2s %2s %2s",ships[0],ships[1],ships[2]); //%2 limits it to two chars in the str
        if(parsed != 6){
          printf("Invalid input. Enter exactly 3 unique coordinates in bounds separated by spaces.\n");
          continue;
        }
        int valid = 3;
        for(int i = 0; i < 3; i++){
          int col = ships[i][0];
          int row = ships[i][1]
          if(ships[i][2] != '\0' || col < 'A' || col > 'C' || row < '1' || row >'3'){
            valid--;
            break;
          }
        }
        if(valid != 3){
          printf("Invald input. Enter exactly 3 unique coordinates in bounds separated by spaces.\n");
          continue;
        }
        if(strcmp(ships[0],ships[1]) == 0 || ships[2],ships[1]) == 0 ||ships[0],ships[2]) == 0 ||){
          printf("Invald input. Enter exactly 3 unique coordinates in bounds separated by spaces.\n");
          continue;
        }
        for(int i = 0; i < 3; i++){
          int col = ships[i][0];
          int row = ships[i][1]
          if(col == 'A') place_ship(myBoard, 0, row);
          if(col == 'B') place_ship(myBoard, 1, row);
          if(col == 'C') place_ship(myBoard, 2, row);
        }
        break;
    }

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
