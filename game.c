#include <stdio.h>
#include "game.h"
void clear_board(Board *b){
  for (int i = 0; i < sizeof(B->grid); i++) {
    for (int j = 0; j < sizeof(B->grid[0]); j++) {
        b->grid[i][j] = '.';
    }
}
}
int place_ship(Board *b, int row, int col){
  if(b->grid[row][col] == '.'){
    b->grid[row][col] = 'S';
    b->ships_remaining++;
    return 1;
  }
  return 0;
}

int print_board(struct Board *b){
  printf(" A B C\n");
  for(int i = 0;i <3;i++){
    printf("%d ",i+1);
    for(int c = 0; c < 3;c++){
      printf("%c ",b->grid[i][c]);
    }
    printf("\n");
  }
}
