#include <stdbool.h>
#include <stdio.h>
#include <ncurses.h>

#define row 8
#define colum 8

void drawBoard(char board[row][colum]){
  clear();
  int a = true;
  for(int x = 0; x < row; x++){
    a = a == true ? false : true;
    for (int y = 0; y < colum; y++){
      if (a == true){
        attron(COLOR_PAIR(0));
        mvprintw(x, y, "%c", board[x][y]);
        attroff(COLOR_PAIR(0));
          a = false;
      }
      else{
        attron(COLOR_PAIR(1));
        mvprintw(x, y, "%c", board[x][y]);
        attroff(COLOR_PAIR(1));
        a = true;
      }
          }
  }
  refresh();
}

void initBoard(char board[row][colum]){

  char bpeice[8] = {'5','4','3','9','2','3','4','5'};
  char wpeice[8] = {'r','k','b','q','K','b','k','r'};
  for (int x = 0; x < row; x++){
    board[1][x] = '1';
    board[6][x] = 'p';
    board[0][x] = bpeice[x];
    board[7][x] = wpeice[x];
  }
  for (int x = 2; x < 6; x++){
    for (int y = 0; y < colum; y++){
      board[x][y] = '0';
    }
  }
}

int main(){
  initscr();
  start_color();
  echo();
  init_pair(0, COLOR_WHITE, COLOR_BLACK);
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  char board[row][colum] = {0};
  initBoard(board);
  drawBoard(board);
  int a;
  printf("\n%s\n","move: ");
  scanw("%d",&a);
  while (false){
    drawBoard(board);
  }
  return 0;
}
