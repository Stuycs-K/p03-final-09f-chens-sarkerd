#ifndef GAME_H
#define GAME_H
struct Board{
    int ships_remaining;
    char grid[3][3];
}
void clear_board(struct Board *b);
int place_ship(struct Board *b, int row, int col); // row a b c col 1 2 3 
int fire(struct Board*b, int row, int col);
int bomb(struct Board *b, int row, int col);
int game_over(struct Board *b);
void print_board(struct Board *b);                                                                                     

#endif