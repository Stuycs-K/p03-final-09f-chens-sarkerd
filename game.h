#ifndef GAME_H
#define GAME_H
struct Board{
    int ships_remaining;
    char grid[3][3];
}
void clear_board(Board *b);
int place_ship(Board *b, int row, int col); // row a b c col 1 2 3 
int fire(Board*b, int row, int col);
int bomb(Board *b, int row, int col);
int game_over(Board *b);
void print_board(Board *b);                                                                                     

#endif