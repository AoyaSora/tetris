#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#define COLS 50
#define ROWS 30
#define BLOCKSIZE 5

void printBlock(const int block[][BLOCKSIZE], int tetY, int tetX);
int startx = (COLS -1) / 2 + 1;
int starty = 3;
int main() {
    // Hide cursor
    // printf("\e[?25l");
        const int square[BLOCKSIZE][BLOCKSIZE] = {
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,1,1,0},
            {0,0,1,1,0},
            {0,0,0,0,0},
        };
  

    int quit = 0;
    int tetris[BLOCKSIZE][BLOCKSIZE];
    int board[ROWS][COLS] = {0};
    while (!quit)
    {
        // Render table
        printf("┌");
        for(int i = 0; i < COLS; i ++)
        printf("-");
        printf("┐\n");

        for (int j =0; j < ROWS; j++) {
        printf("|");
        for(int i = 0; i < COLS; i++) 
            printf("·");
        printf("|\n");
        }

        printf("└");
        for(int i = 0; i < COLS; i++)
        printf("-");
        printf("┘\n");
        
        // Move cursor back to top
        printf("\e[%iA", ROWS + 2);

        int gameover = 0;
        int xdir = 0, ydir = 1;
        int tetY = starty, tetX = startx; // it's base on EscapeSequene
        // top left "·" potition
        printf("\e[%i;%iH❤",starty,2); 

        while(!quit && !gameover) {
            // Clear tetris
            if(tetY!=starty) printf("\e[%i;%iH·",tetY-1, tetX);

            // Draw tetris
            printBlock(square,tetY,tetX);
            // printf("\e[%i;%iH#",tetY, tetX);

            // Touch floar or under Tetris
            if(tetY == 2 + ROWS || board[tetY-2][tetX-2] == 1) {
                board[tetY-3][tetX-2] = 1;
                tetY = starty; tetX = startx;
            }
            // tetY++;

            fflush(stdout);
            usleep(5 * 1000000 / 60);
        }
       
    }
    // Show cursor
    printf("\e[?25h");
    return 0;
}

void printBlock(const int block[][BLOCKSIZE], int tetY, int tetX) {
    for(int j = 0; j < BLOCKSIZE; j++) {
        int y = tetY - 2 + j;
        for(int i = 0; i < BLOCKSIZE; i++) {
            int x = tetX - 2 + i;
            if( block[j][i] == 1 || starty< y) {
                printf("\e[%i;%iH#",y,x);
            }
        }
    }
}