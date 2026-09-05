#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
  int MaxAttempt = 3;
  int Input;
  unsigned int seed = time(0);
  int max = 1,min = 3;
  int Bscore = 0,Pscore = 0;
  int random = rand_r(&seed) % (max - min) + max;
  for (int i = 0; i < MaxAttempt;){
   printf("Enter 1:Rock, 2:Paper, 3:Sicor\n");

  scanf("%d",&Input);
  if (Input ==  1){
    i++;
    //Rock
    printf("Player: Rock\n");
    if(random == 1){
      //Rock

      printf("Bot: Rock\n");
       printf("---------------\n");

      printf("Draw\n");
    }
    else if (random == 2){
      printf("Bot: Paper\n");
       printf("---------------\n");

      printf("Bot wins\n");
      Bscore += 1;
    }
    else if (random ==  3){
      printf("Bot: Sicor\n");
       printf("---------------\n");
      printf("Player win\n");
      Pscore += 1;
    }
  }
  else if (Input == 2){
    i++;
    //paper;
    printf("Player: Paper\n");
    if(random == 1){
      //Rock

      printf("Bot: Rock\n");
       printf("---------------\n");

      printf("Player win\n");
      Pscore += 1;
    }
    else if (random == 2){
      printf("Bot: Paper\n");
       printf("---------------\n");

      printf("Draw\n");
    }
    else if (random ==  3){
      printf("Bot: Sicor\n");
       printf("---------------\n");

      printf("Bot win\n");
      Bscore += 1;
    }

  }
  else if (Input == 3){
    i++;
    //sicore;
    //
    printf("Player: Sicor\n");
    if(random == 1){
      //Rock

      printf("Bot: Rock\n");
      printf("---------------\n");
      printf("Bot wins\n");
      Bscore += 1;

    }
    else if (random == 2){
      printf("Bot: Paper\n");
       printf("---------------\n");

      printf("Player wins\n");
      Pscore += 1;
    }
    else if (random ==  3){
      printf("Bot: Sicor\n");
       printf("---------------\n");

      printf("Draw\n");
    }

  }
  else {
    printf("invalid input");
  }}
  if (Pscore > Bscore){
    printf("Player wins the game");

  }
  else if (Pscore < Bscore){
    printf("Bot wins the game");
  }
  else{
  printf("game draw");
}
  return 0;
}
