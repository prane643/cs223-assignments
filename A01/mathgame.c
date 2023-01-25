/***************************************************
 * mathgame.c
 * Author: Pranav Rane 
 * Implements a math game
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
 printf("Welcome to the Math Game! \n");
 printf("How many rounds do you want to play? \n");
 int numberOfGames;
 scanf(" %d",&numberOfGames);
 printf("You chose: %d\n",numberOfGames);

 // set seed
 time_t t;
 srand((unsigned) time(&t));


 int i,correct,incorrect;
 correct = 0;
 incorrect = 0;
 for(i=0;i<numberOfGames;i++) {
  int a,b,c,response;
  a = rand()%10;
  b = rand()%10;
  c = a+b;
  printf("%d + %d =?  ",a,b);
  scanf(" %d",&response);
  if (response==c) {
   printf("Correct! \n");
   correct = correct+1;
  }
  if (response!=c) {
   printf("Incorrect \n");
   incorrect = incorrect+1;
  }
 }
 printf(" \n You answered %d/%d correctly. \n",correct,correct+incorrect);
  return 0;
}
