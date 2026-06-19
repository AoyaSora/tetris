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
  double theta = PI/90;

    for(int j = 0; j < SIZE; j++) {
        for(int i = 0; i < SIZE; i++) {

            if(matrix[j][i]) {

                double x = i - 2;
                double y = j - 2;

                double nx = x*cos(theta) - y*sin(theta);
                double ny = x*sin(theta) + y*cos(theta);

                int rx = (int)round(nx);
                int ry = (int)round(ny);

                tmpMatrix[ry + 2][rx + 2] = 1;
            }
        }
    }
    
    // Draw matrix
    for(int j = 0; j < SIZE; j++) {
        for(int i = 0; i < SIZE; i++) {
            printf("%d",tmpMatrix[j][i]);
        }
        printf("\n");
    }

    return 0;
}

// 計算結果が少数になり丸められて0になっている