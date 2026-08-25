#include <stdio.h>
#include <unistd.h>
//number of row and colun
#define rows 10
#define colun 80

void makegrid(int grids[rows][colun]){
  for (int r = 0; r < rows; r++){
    for (int c = 0; c < colun; c++){
      if (grids[r][c] == 0){
        printf(".");
      }
      else {
      printf("#");
      }
    }
    printf("\n");
  }
}
int Ncount(int grids[rows][colun],int row,int col){
  int count = 0;
  for (int x = -1;x <= 1; x++){
    for (int y = -1;y <= 1; y++){
      if (x == 0 && y == 0)
        continue;
      int r = row + x;
      int c = col + y;

      if(r >= 0 && r < rows && c >= 0 && c < colun){
        count += grids[r][c];
      }

    }
  }
  return count;
}
void next_gen(int grids[rows][colun], int ngrids[rows][colun]){
  for (int row = 0; row < rows; row++){
    for (int col = 0; col < colun; col++){
      int n = Ncount(grids,row,col);
      int alive = grids[row][col];
      if (alive == 1){
        if (n == 2 || n == 3){
          ngrids[row][col] = 1;
        }
        else{
          ngrids[row][col] = 0;
        }
      }
      else{
        if(n == 3){
          ngrids[row][col] = 1;
        }
        else{
          ngrids[row][col] = 0;
        }
      }
    }
  }
}
void copy_grid(int grid[rows][colun],int ngrid[rows][colun]){
  for (int r = 0;r < rows;r++){
    for (int c = 0;c < colun;c++){
      grid[r][c] = ngrid[r][c];
    }
  }
}
int main(){
  int grid[rows][colun] = {0};
  int ngrid[rows][colun] = {0};
  // Glider 1

grid[0][1] = 1;
grid[1][2] = 1;
grid[2][0] = 1;
grid[2][1] = 1;
grid[2][2] = 1;
  while (1){
     printf("\033[2J\033[H");
     makegrid(grid);
     next_gen(grid,ngrid);
     copy_grid(grid,ngrid);
     usleep(500000);
  }
  return 0;
}
