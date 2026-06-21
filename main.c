#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>

#define COLS 20 // 列
#define ROWS 30 // 行
#define BLOCKTYPES 3
#define BLOCKSIZE 5
void clearBlock(const int block[][BLOCKSIZE], int tetY, int tetX);
void printBlock(const int block[][BLOCKSIZE], int tetY, int tetX);
int isStopTetris(int board[][COLS],const int block[][BLOCKSIZE],int tetY,int tetX);
int startx = COLS / 2 + 2;
int starty = 3;
int main() {
    // Hide cursor
    printf("\e[?25l");

    // Switch to cannoical mode, dsiable echo
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    const int square[BLOCKSIZE][BLOCKSIZE] = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,1,1,0,0},
        {0,1,1,0,0},
        {0,0,0,0,0},
    };
    const int rightL[BLOCKSIZE][BLOCKSIZE] = {
        {0,0,0,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,1,0},
        {0,0,0,0,0},
    };
    const int rocket[BLOCKSIZE][BLOCKSIZE] = {
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,1,1,1,0},
        {0,1,0,1,0},
        {0,1,0,1,0},
    };
    /*
    BLOCKSIZE列の二次元配列へのポインタを格納する配列
    blocks[BLOCKTYPES] -> BLOCKTYPES個の要素を持つ配列
    *blocks[BLOCKTYPES] -> その各要素はポインタ
    (*blocks[BLOCKTYPES])[BLOCKSIZE] -> そのポインタはBLOCKSIZE個のintからなる配列を指す
    */
    
    const int (*blocks[BLOCKTYPES])[BLOCKSIZE] = {square,rightL,rocket};
    
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
        int tetY = starty, tetX = startx; // it's base on EscapeSequene
        int beforeTetY = starty, beforeTetX = startx;
        // top left "·" potition
        printf("\e[%i;%iH❤",starty,2); 

        // randam seed
        srand((unsigned int)time(NULL));
        int idx = rand() % BLOCKTYPES;

        while(!quit && !gameover) {
            const int (*block)[BLOCKSIZE]  = blocks[idx];
            // Clear tetris
            if(tetY!=starty) clearBlock(block,beforeTetY-1,beforeTetX);

            // Draw tetris
            printBlock(block,tetY,tetX);
            
            // Touch floar or under Tetris
            if(isStopTetris(board,block,tetY,tetX)) {
                // add tetris to board
                for(int j = 0; j < BLOCKSIZE; j++) {
                    int y = tetY - 2 -3 + j;
                    for(int i = 0; i < BLOCKSIZE; i++ ) {
                        int x = tetX - 2 -2+ i;
                        if(block[j][i] == 1) {
                            board[y][x] = 1;
                            // printf("\e[%i;%iHx:%d ,y:%d\n",20+c,20,x, y);
                            // c++;
                        }
                    }
                }
                idx = rand() % BLOCKTYPES;
                tetY = starty; tetX = startx;
            }
            tetY++;
            beforeTetY = tetY;

            fflush(stdout);
            usleep(5 * 1000000 / 60);

            // READ keyboard
            struct timeval tv;
            fd_set fds;
            tv.tv_sec = 0;
            tv.tv_usec = 0;

            FD_ZERO(&fds);
            FD_SET(STDIN_FILENO, &fds);
            select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
            if (FD_ISSET(STDIN_FILENO, &fds)) {
                int ch = getchar();
                if (ch == 27 || ch == 'q') {
                quit = 1;
                } else if (ch == 'a' && tetX > 2) {
                beforeTetX = tetX;                
                tetX += -1;
                } else if (ch == 'd' && tetX < COLS) {
                beforeTetX = tetX;             
                tetX += 1;
                } else if (ch == 's' && tetY < ROWS) {
                beforeTetY = tetY;
                tetY += 1;
                } 
            }  
        }
        if ( !quit) {
            // Show game over 
            printf("\e[%i;%iH Game Over! ", ROWS/2, COLS / 2 - 5);
            printf("\e[%iF", ROWS / 2);
            fflush(stdout);
            getchar();
        } 
       
    }
    // Show cursor
    printf("\e[?25h");

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 0;
}

void printBlock(const int block[][BLOCKSIZE], int tetY, int tetX) {
    for(int j = 0; j < BLOCKSIZE; j++) {
        int y = tetY - 2 + j;
        for(int i = 0; i < BLOCKSIZE; i++) {
            int x = tetX - 2 + i;
            if( block[j][i] == 1 && starty <= y) {
                printf("\e[%i;%iH#",y,x);
            }
        }
    }
}
void clearBlock(const int block[][BLOCKSIZE], int tetY, int tetX) {
    for(int j = 0; j < BLOCKSIZE; j++) {
        int y = tetY - 2 + j;
        for(int i = 0; i < BLOCKSIZE; i++) {
            int x = tetX - 2 + i;
            if( block[j][i] == 1 && starty <= y) {
                printf("\e[%i;%iH❤",y,x);
            }
        }
    }
}

int isStopTetris(int board[][COLS],const int block[][BLOCKSIZE],int tetY,int tetX) {
    // touch floar
    for(int j = 0; j < BLOCKSIZE; j ++) {
        int y = tetY - 2 -3 + j;
        for(int i = 0; i < BLOCKSIZE; i ++) {
            int x = tetX - 2 -2 + i;
            if(block[j][i] == 1) {
                if(y > ROWS - 2 || board[y+1][x] == 1) { // || board[y+1][x] == 1
                    return 1;
                }
            }
        }
    }
    // compare board and block
    return 0;
}