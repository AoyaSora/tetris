#include <stdio.h>
int main() {
    // printf("\e[%iB\e[%iCHi!\n", 0, 0); // 0だと省略されて1となる．
    printf("\e[2;1HHello!\n");
    return 0;
}

/*
絶対位置はターミナル画面の左上(1,1)が基準
*/