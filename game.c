#include <stdio.h>
#include "game.h"
void clear_board(Board *b){
  for (int i = 0; i < sizeof(B->grid); i++) {
    for (int j = 0; j < sizeof(B->grid[0]); j++) {
        B->grid[i][j] = 0;
    }
}
}
int place_ship(Board *b, int row, int col){
  B->grid[row][col] = 1; // one is ship 
}
