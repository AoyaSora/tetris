#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>

#define COLS 10 // 列
#define ROWS 20 // 行
#define SCORECOLS 10
#define SCOREROWS 1
#define NEXTCOLS 5
#define NEXTROWS 5
#define BLOCKTYPES 6
#define BLOCKSIZE 5
const int square[BLOCKSIZE][BLOCKSIZE] = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,1,1,0},
        {0,0,1,1,0},
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
        {0,0,0,0,0},
        {0,0,1,0,0},
        {0,1,1,1,0},
        {0,1,0,1,0},
        {0,0,0,0,0},
    };
    const int leftL[BLOCKSIZE][BLOCKSIZE] = {
        {0,0,0,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,1,1,0,0},
        {0,0,0,0,0},
    };
    const int n[BLOCKSIZE][BLOCKSIZE] = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,1,1,0},
        {0,1,1,0,0},
        {0,0,0,0,0},
    };
    const int inverseN[BLOCKSIZE][BLOCKSIZE] = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,1,1,0,0},
        {0,0,1,1,0},
        {0,0,0,0,0},
    };
    const int l[BLOCKSIZE][BLOCKSIZE] = {
        {0,0,0,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,0,0,0},
    };
const int (*blocks[BLOCKTYPES])[BLOCKSIZE] = {square,rightL,leftL,n,inverseN,l};

void clearBlock(int block[][BLOCKSIZE], int tetY, int tetX, int newRotaion, int oldRotaion);
void printBlock(const int block[][BLOCKSIZE], int tetY, int tetX);
int isStopTetris(int board[][COLS],const int block[][BLOCKSIZE],int tetY,int tetX);
int clearRow(int(* board)[COLS]);
int startx = COLS / 2 + 2;
int starty = 3;
int main() {
    // Clear terminal
    printf("\033[2J\033[H"); 
    // Hide cursor
    printf("\e[?25l");

    // Switch to cannoical mode, dsiable echo
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    /*
    BLOCKSIZE列の二次元配列へのポインタを格納する配列
    blocks[BLOCKTYPES] -> BLOCKTYPES個の要素を持つ配列
    *blocks[BLOCKTYPES] -> その各要素はポインタ
    (*blocks[BLOCKTYPES])[BLOCKSIZE] -> そのポインタはBLOCKSIZE個のintからなる配列を指す
    */
    
    
    int quit = 0;
    int tetris[BLOCKSIZE][BLOCKSIZE];
    int board[ROWS][COLS] = {0};
    while (!quit)
    {
        printf("\033[H"); 
        int point = 0;
        // Render table
        printf("\e[1B┌");
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

        // Render Score
        printf("\e[%i;%iH┌",3,COLS+3);
        for(int i = 0; i < SCORECOLS; i ++)
        printf("-");
        printf("┐\n");
        
        for (int j =0; j < SCOREROWS; j++) {
        printf("\e[%iC|",COLS+2);
        for(int i = 0; i < SCORECOLS; i++) 
            printf(" ");
        printf("|\n");
        }

        printf("\e[%iC└",COLS+2);
        for(int i = 0; i < SCORECOLS; i++)
            printf("-");
        printf("┘\n");
        printf("\e[%i;%iHSCORE:",2,COLS+4);

        // Render next block 
        printf("\e[%i;%iH┌",6+SCOREROWS,COLS+4);
        for(int i = 0; i < NEXTCOLS; i ++)
        printf("-");
        printf("┐\n");
        
        for (int j =0; j < NEXTROWS; j++) {
        printf("\e[%iC|",COLS+3);
        for(int i = 0; i < NEXTCOLS; i++) 
            printf(" ");
        printf("|\n");
        }

        printf("\e[%iC└",COLS+3);
        for(int i = 0; i < NEXTCOLS; i++)
            printf("-");
        printf("┘\n");
        printf("\e[%i;%iHNEXT:",5+SCOREROWS,COLS+5);


        // Move cursor back to top
        printf("\e[%iA", ROWS + 2);

        int gameover = 0;
        int newTetY = starty, newTetX = startx; // it's base on EscapeSequene
        int oldTetY = starty, oldTetX = startx;
        int oldRotation = 0, newRotation = 0;
        int addScore = 0, newScore = 0;
        // top left "·" potition
        printf("\e[%i;%iH❤",starty,2); 

        // randam seed
        srand((unsigned int)time(NULL));
        int idx = rand() % BLOCKTYPES;
        int nextIdx = rand() % BLOCKTYPES;
        int rotationCount=0;
        int block[BLOCKSIZE][BLOCKSIZE];
        int isRotation = 0;

        // Game loop
        while(!quit && !gameover) {
            for(int j = 0; j < BLOCKSIZE; j++) {
                for(int i = 0; i < BLOCKSIZE; i++ ) {
                    if(newRotation == 0) {
                        block[j][i] = blocks[idx][j][i];
                    }
                    else if(newRotation == 1) {
                        block[j][i] = blocks[idx][BLOCKSIZE - 1 - i][j];
                    }
                    else if(newRotation == 2) {
                        block[j][i] = blocks[idx][BLOCKSIZE - 1 - j][BLOCKSIZE -1 - i];
                    }
                    else if(newRotation == 3) {
                        block[j][i] = blocks[idx][i][BLOCKSIZE - 1 - j];
                    }
                }
            }
            // based on block 
            int tetLeft = 2,tetRight = -2,tetUpper = -2,tetUnder = 2;
            for(int j = 0; j < BLOCKSIZE; j++) {
                int tmpH = j-2;
                for(int i = 0; i < BLOCKSIZE; i++) {
                    if(block[j][i] == 1) {
                        int tmpW = i - 2;
                        if(tmpW < tetLeft) tetLeft = tmpW;
                        if(tmpW > tetRight) tetRight = tmpW;
                        if(tmpH < tetUnder) tetUnder = tmpH;
                        if(tmpH > tetUpper) tetUpper = tmpH;
                    }
                }
            }
            // printf("\e[%i;%iHtetX:%02d, tetLeft:%02d ,tetRight:%02d\n",20,20, tetX, tetLeft, tetRight);
            // Render next block
            for(int j = 0; j < BLOCKSIZE; j++) {
                printf("\e[%i;%iH",SCOREROWS+7+j,COLS+5); // set left top
                for(int i = 0; i < BLOCKSIZE; i++) {
                    if(blocks[nextIdx][j][i] == 1) printf("#");
                    else printf("·");
                }
            }

            // Clear tetris
            if(newTetY!=starty) clearBlock(block,oldTetY,oldTetX,newRotation, oldRotation);
            // printf("\e[%i;%iHtetX:%02d, tetY:%02d, oldTetX:%02d, oldTetY:%02d",20,20,tetX,tetY,oldTetX,oldTetY);

            // Draw tetris
            printBlock(block,newTetY,newTetX);
            int c = 0;
            // Touch floar or under Tetris
            if(isStopTetris(board,block,newTetY,newTetX)) {
                // add tetris to board
                for(int j = 0; j < BLOCKSIZE; j++) {
                    int y = newTetY - 2 -3 + j;
                    for(int i = 0; i < BLOCKSIZE; i++ ) {
                        int x = newTetX - 2 -2+ i;
                        if(block[j][i] == 1) {
                            board[y][x] = 1;
                            c++;
                        }
                    }
                }
                printf("\e[%i;%iHboard[%d][%d]:%d\n",19,20,0,COLS/2 - 1,board[0][COLS/2 - 1]);
                if(board[0][COLS/2 - 1] == 1 || board[0][COLS/2] == 1 ) {
                    gameover = 1; 
                    break;
                }

                // Clear tetris Rows
                addScore = clearRow(board);

                // show score
                addScore *= 1000;
                if(addScore!=0) {
                    newScore += addScore;
                    printf("\e[%i;%iH%010d",4,COLS+4,newScore);
                    addScore = 0;
                }
                
                // set next 
                idx = nextIdx;
                nextIdx = rand() % BLOCKTYPES;
                newTetY = starty; newTetX = startx;
                oldTetY = starty; oldTetX = startx;
                rotationCount = 0; isRotation = 0;
                newRotation = 0; oldRotation = 0;

            }
            oldTetY = newTetY;
            oldTetX = newTetX;
            newTetY++;

            /*-----------------time-----------------*/
            fflush(stdout);
            usleep(12 * 1000000 / 60);

            // READ keyboard
            struct timeval tv;
            fd_set fds;
            tv.tv_sec = 0;
            tv.tv_usec = 0;

            FD_ZERO(&fds);
            FD_SET(STDIN_FILENO, &fds);
            select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
            oldRotation = newRotation;
            // printf("\e[%i;%iH newTetX:%02d, newTetY:%02d, tetUdner:%02d\n",22,22,newTetX-2,newTetY-2,tetUnder);
            if (FD_ISSET(STDIN_FILENO, &fds)) {
                int ch = getchar();
                tcflush(STDIN_FILENO, TCIFLUSH);
                if (ch == 27 || ch == 'q') {
                quit = 1;
                } else if (ch == 'a' && tetLeft + newTetX - 2 > 0) {
                newTetX += -1;
                } else if (ch == 'd' && tetRight + newTetX - 2 < COLS -1) {
                newTetX += 1;
                } 
                else if (ch == 's' && tetUnder + newTetY < ROWS-1) {
                newTetY += 1;
                } else if (ch == 27 || ch == 'q') {
                    quit = 1;
                }
                else if (ch == 'r' && (tetUnder + newTetX - 2 > 0) && (tetRight + newTetY  < ROWS) && (tetUpper + newTetX - 2 < COLS -1) ) { 
                    oldRotation = newRotation;
                    newRotation++;
                    if(newRotation == 4) newRotation = 0;
                }
            }   
        }
        if ( !quit) {
            // Show game over 
            printf("\e[%i;%iH Game Over! ", ROWS/2, COLS / 2 - 5);
            printf("\e[%i;%iH",1,1);
            fflush(stdout);
            // reset matrix
            for(int j = 0; j< ROWS; j++) {
                for(int i = 0; i < COLS; i++) {
                    board[j][i] = 0;
                }
            }
            int ch = getchar();
            tcflush(STDIN_FILENO, TCIFLUSH);
            // printf("\e[%i;%iH|···········|", ROWS/2, COLS / 2 - 5);
            if (ch == 27 || ch == 'q') {
                quit = 1;
            }
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
void clearBlock(int block[][BLOCKSIZE], int tetY, int tetX, int newRotaion, int oldRotation) {
    if(newRotaion != oldRotation) {
        for(int j = 0; j < BLOCKSIZE; j++) {
            int y = tetY - 2 + j;
            for(int i = 0; i < BLOCKSIZE; i++) {
                // int x = tetX - 2 + i;
                if( block[j][i] == 1 && starty < y) {
                    printf("\e[%i;%iH·",tetY - 2 + BLOCKSIZE - 1 - i, tetX - 2 + j);
                }
            }
        }
    }else {
        for(int j = 0; j < BLOCKSIZE; j++) {
            int y = tetY - 2 + j;
            for(int i = 0; i < BLOCKSIZE; i++) {
                int x = tetX - 2 + i;
                if( block[j][i] == 1 && starty <= y) {
                    printf("\e[%i;%iH·",y,x);
                }
            }
        }
    } 
}

int isStopTetris(int board[][COLS],const int block[][BLOCKSIZE],int tetY,int tetX) {
    // touch floar or block
    for(int j = 0; j < BLOCKSIZE; j ++) {
        int y = tetY - 2 -3 + j;
        for(int i = 0; i < BLOCKSIZE; i ++) {
            int x = tetX - 2 -2 + i;
            if(block[j][i] == 1) {
                if(y >= 0) {
                    if(y > ROWS - 2 || board[y+1][x] == 1) { // || board[y+1][x] == 1
                    return 1;
                }
                }
                
            }
        }
    }
    return 0;
}

/*
function name: clearRow
argement: *board[][BLOCKSIZE]
return: clear row num

investigation board matrix under to top until find "0".
store row number for how many rows delete. 

flow: 
check how many rows complete. 
clear rows from the screen.
change board matrix.
*/
int clearRow(int(* board)[COLS]) {
    int isAlloneRow = 0;
    int clearRow[ROWS] = {0};
    int rowNum = 0;
    // search
    for(int j = 0; j < ROWS; j++) {
        isAlloneRow = 0;
        for(int i = 0; i < COLS; i++) {
            if(board[j][i] == 0) isAlloneRow = 1;
        }
        if(isAlloneRow != 1) {
            clearRow[j] = 1;
            rowNum ++; 
        }
    }
        // clear screen
        // for(int j = 0; j < ROWS; j++) {
        //     if(clearRow[j] == 1) {
        //         for(int i = 0; i < COLS; i++) {
        //         printf("\e[%i;%iH❤",3+j,2+i);
        //     }
        //     }
        // }
        // change board matrix under rowNum
        // for(int j = ROWS - rowNum -1; j >= 0 ; j --) {
        //     for(int i = 0; i < COLS; i++) {
        //         board[j+rowNum][i] = board[j][i];
        //     }
        // }
        // change board from under
        for(int j = ROWS-1; j >= 0; j--) {
            if(clearRow[j]) {
                for(int l = j; l > 0; l--){
                    for(int m = 0; m < COLS; m++) {
                        board[l][m] = board[l-1][m];
                    }
                }
                // clearRowが変わっていないからダメ
                /*
                clearRow= {0,0,0,0,...0,1,1}
                clearRow= {0,0,0,0,...0,0,1}
                要素番号の指定とそれより小さい値を右にずらす
                */
                for(int i = ROWS-1; i > 0; i--) {
                    clearRow[i] = clearRow[i-1];
                }
                j++;
            }
        }
        if(rowNum) {
            for(int j = 0; j < ROWS; j++) {
                for(int i = 0; i < COLS; i++) {
                    if(board[j][i]) printf("\e[%i;%iH#",3+j,2+i);
                    else printf("\e[%i;%iH·",3+j,2+i);
                }
            }
        }
    return rowNum;
}
