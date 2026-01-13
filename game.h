#ifndef GAME_H
#define GAME_H
#define LOSE 999
#define WIN 888
#define NONE 555
#define WRITE 1
#define WAIT 0
#define READ 2
#define CHECK 3
struct Board{
    int ships_remaining;
    char grid[3][3];
};__attribute__((packed));
void clear_board(struct Board *b);
int place_ship(struct Board *b, int row, int col); // row a b c col 1 2 3
int fire(struct Board*b, int row, int col);
int game_over(struct Board *b);
void print_board(struct Board *b);

#endif

