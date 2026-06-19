#include <stdio.h>
#include <math.h>
#define SIZE 5
#define PI 3.1415926535
int main() {
    int tmpMatrix[SIZE][SIZE] = {0};
    int matrix[SIZE][SIZE] = {
        {0,0,0,0,0},
        {0,1,0,0,0},
        {0,1,1,1,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };

    // 90 degree Rotate matrix
    for(int j = 0; j < SIZE; j++) {
        for(int i = 0; i < SIZE; i++) {
            // ここでifが出てくる時点でindex i,jを変えて回転させたほうが早いかも
            if(matrix[j][i] == 1) {
                double x = i - 2;
                double y = j - 2;
                double nx = x*cos(PI/2) - y*sin(PI/2);
                double ny = x * sin(PI/2) + y*cos(PI/2);
                printf("x: %d", x);
                printf("y: %d", y);
                tmpMatrix[y][x] = 1;
            }
        }
    }
    
    for(int j = 0; j < SIZE; j++) {
        for(int i = 0; i < SIZE; i++) {
            printf("%d",tmpMatrix[j][i]);
        }
        printf("\n");
    }

    return 0;
}

// 計算結果が少数になり丸められて0になっている