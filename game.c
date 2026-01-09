#include <stdio.h>
#include "game.h"
void clear_board(struct Board *b){
  b->ships_remaining = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        b->grid[i][j] = '.';
    }
}
}
int place_ship(struct Board *b, int row, int col){
  if(b->grid[row][col] == '.'){
    b->grid[row][col] = 'S';
    b->ships_remaining++;
    return 1;
  }
  return 0;
}

int fire(struct Board *b, int row, int col){
  if(b->grid[row][col] == 'S'){
    b->grid[row][col] = 'X';
     b->ships_remaining--;
     return 1;
  }
  if(b->grid[row][col] == '.'){
    b->grid[row][col]= 'O';
  }
  return 0;
}

int game_over(struct Board *b){
  if(b->ships_remaining == 0){
    return 1;
  }
  return 0;
}


void print_board(struct Board *b){
  printf(" A B C\n");
  for(int i = 0;i <3;i++){
    printf("%d ",i+1);
    for(int c = 0; c < 3;c++){
      printf("%c ",b->grid[i][c]);
    }
    printf("\n");
  }
}
