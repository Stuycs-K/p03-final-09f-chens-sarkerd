#include "networking.h"
int main(int argc, char *argv[] ) {
  char* IP = "127.0.0.1";
  if(argc>1){
    IP=argv[1];
  }
  int server_socket = client_tcp_handshake(IP);
  printf("client connected.\n");
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
    int parsed = sscanf(buff,"%2s %2s %2s",ships[0],ships[1],ships  [2]); //%2 limits it to two chars in the str
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
      break;
  }
}
