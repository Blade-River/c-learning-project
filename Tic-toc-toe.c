#include <stdio.h>
#define x  3
#define y 3
void makegrid(int grid[x][y]){
  for (int i = 0; i < x; i++){
    for (int j = 0; j < y; j++){
        if (grid[i][j] == 0){
          printf("0");
        }
        else if (grid[i][j] == 2){
          printf("$");
        }
        else{
          printf("*");
        }
    }
    printf("\n");
  }
}
int countside(int grid[x][y], int row, int colu){
  int count = 0;
  for (int i = -1; i <= 1; i++){
    for (int j = -1; j <= 1; j++){
      if (i == 0 && j == 0){
        continue;
      }
      int r = row + i;
      int c = colu + j;
      if ( r >= 0 && r < row && c >= 0 && c < colu){
       count += grid[r][c]; 
      }
    }
  }
  return count;
}
void checkwins(int grid[x][y]){
  for (int i = 0; i<x; i++){
    int count1 = 0;
    int count2 = 0;
    for (int j = 0; j<y; j++){
      if (grid[i][j] == 1){
        count1 += 1;
      }
      if (grid[i][j] == 2){
        count2 += 2;
      }
    }
    if (count1 == 3){
      printf("player win");
    }
    if (count2 == 3){
      printf("bot win");
    }
  }

  for (int j = 0; j<y; j++){
    int count1 = 0;
    int count2 = 0;
    for (int i = 0; i<y; i++){
      if(grid[i][j] == 1){
        count1 += 1;
      }
      if (grid[i][j] == 2){
        count2 += 1;
      }
    }
    if (count1 == 3){
      printf("player win");
    }
    if (count2 == 3){
      printf("bot win");
    }
  }
  int count1 = 0;
  int count2 = 0;
  for (int i = 0; i < x; i++){
      if (grid[i][i] == 1){
        count1 += 1;
      }
      if (grid[i][i] == 2){
        count2 += 1;
      }
  }
  if (count1 == 3){
    printf("player win");
  }
  if (count2 == 3){
    printf("bot win");
  }

  count1 = 0;
  count2 = 0;
  for (int i = 0; i < x; i++){
    int j = 2 - i;
    if (grid[i][j] == 1){
      count1 += 1;
    }
    if (grid[i][j] == 2){
      count2 += 1;
    }
  }
  if (count1 == 3){
    printf("player win");
  }
  if (count2 == 3){
    printf("bot win");
  }



}
int *nextmove(int grid[x][y]){
  //vertical
  int *pointer = NULL;
  for (int i = 0; i < x; i++){
    int count1 = 0;
    int empty = -1;
    int count2 = 0;
    for (int j = 0; j < y; j++){
      if (grid[i][j] == 1){
        count1 += grid[i][j];
      }
      if (grid[i][j] == 2){
        count2 += grid[i][j];
      }
      if (grid[i][j] == 0){
        empty = j;
      }
      
    }
    if (count2 == 4 && empty != -1){
      return &grid[i][empty];
    }
    if (count1 == 2 && empty != -1){
      pointer = &grid[i][empty];
    }
  }
  //horizontal
  for (int j = 0; j <y; j++){
    int count1 = 0;
    int empty = -1;
    int count2 = 0;
    for (int i = 0; i < x; i++){
      if (grid[i][j] == 1){
        count1 += grid[i][j];
      }
      if (grid[i][j] == 2){
        count2 += grid[i][j];
      }
      if (grid[i][j] == 0){
        empty = i;
      }
     
    }
    if (count2 == 4 && empty != -1){
      return &grid[empty][j];
    }
    if (count1 == 2 && empty != -1){
      pointer = &grid[empty][j];
    }
  }

  //diagnoal-left
  int count1 = 0;
  int empty = -1;
  int count2 = 0;
  for (int i = 0; i < x; i++){
    if (grid[i][i] == 1){
      count1 += 1;
    }
    if (grid[i][i] == 2){
      count2 += 2;
    }
    if (grid[i][i] == 0){
      empty = i;
    }
  }
  if (count2 == 4 && empty != -1){
    return &grid[empty][empty];
  }
  if (count1 == 2 && empty != -1){
    pointer = &grid[empty][empty];
  }
  //diagonal-right
  count1 = 0;
  empty = -1;
  count2 = 0;
  for (int i = 0;i < x; i++){
    int j = 2 - i; 
    if (grid[i][j] == 1){
      count1 += 1;
    }
    if (grid[i][j] == 2){
      count2 += 2;
    }
    if (grid[i][j] == 0){
      empty = i;
    }
  }
  if (count2 == 4 && empty != -1){
    return &grid[empty][2-empty];
  }
  if (count1 == 2 && empty != -1){
    pointer = &grid[empty][2-empty];
  }
  if (pointer != NULL){
    return pointer;
  }
  if (grid[1][1] == 0){
    return &grid[1][1];
  }
  for (int i = 0; i < x; i++){
    for (int j = 0; j < y; j++){
      if (grid[i][j] == 0){
        return &grid[i][j];
      }
    }
  }
    return NULL;
}
int *inputconv(int grid[x][y], int *inp){

    if (*inp < 1 || *inp > 9)
        return NULL;

    int position = *inp - 1;

    int row = position / 3;
    int col = position % 3;

    if (grid[row][col] != 0)
        return NULL;

    return &grid[row][col];
}
int main(){
  
  int grid[x][y] = {0};
  while(1){
    printf("\033[2J\033[H");
     int *p = nextmove(grid);
    if (p != NULL){
      *p = 2;
    } 
    makegrid(grid);
    checkwins(grid);
    int input = 0;
    scanf("%d",&input);
    int *fptr = inputconv(grid, &input);
    if (fptr != NULL)
      *fptr = 1;
    else{
      printf(":=\n");
    }
     }
  return 0;
  
}
