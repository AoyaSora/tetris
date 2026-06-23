### About 
Make Tetris by C language and Escape Sequence.
### How to use 
- 'a' key : move the tetris to left
- 'd' key : move the tetris to right
- 'r' key : rotate the tetris piece clockwise
- 'q' key : finish this game.

### Files
| File name | explain |
| --------- | ------- |
| main.c | tetris file |
| tetris | tetris execute file |



### Compile process 
```
$ gcc main.c -o tetris
$ ./tetris
```
### 概要
前ncursesライブラリを使って画面描画を行なったが，エスケープシーケンスを使うことでprnitfだけで画面描画できるのを見つけた．これを使ってテトリスを作成する．ロジックを考えるのを目的としているのでAIはなるべく使っていない
### 感想
- 現在終了判定が雑．
- 時間制御をusleepだけで行っているため，操作性が悪い．一度落ちる間に一回しか動けない
- 設置してあるブロックの横からめり込ませれる．また，sキーで下に行く時めり込むことがある．壁にめり込まないように回転できる範囲に制限をかけてあるため動かしずらい．