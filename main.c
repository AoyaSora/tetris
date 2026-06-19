#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#define COLS 50
#define ROWS 30

int main() {
    // Hide cursor
    // printf("\e[?25l");


    int quit = 0;
    int x[5], y[5];
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
        int startx = (COLS -1) / 2 + 1;
        int starty = 3;
        
        x[0] = startx;
        y[0] = starty;

        while(!quit && !gameover) {
            // Clear tetris
            printf("\e[%iB\e[%iC·", y[0], x[0]);
            printf("\e[%i;%iH",1,1);

            y[0]++;

            // 
            if(y[0] == ROWS + 3) {
                // Draw tetris
                printf("\e[%iB\e[%iC#", y[0] - 1, x[0]);
                printf("\e[%i;%iH",1,1);
                board[y[0]][x[0]] = 1;
                y[0] = starty;
                x[0] = startx;
            }
            // Draw tetris
            printf("\e[%iB\e[%iC#", y[0], x[0]);
            printf("\e[%i;%iH",1,1);

            fflush(stdout);
            usleep(5 * 1000000 / 60);
        }
       
    }
    // Show cursor
    printf("\e[?25h");
    return 0;
}