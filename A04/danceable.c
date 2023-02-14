/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Feb 13
 * Description
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define struct to be used in node
struct SongInformation {
  char artist[128];
  char title[128];
  int durationMin;
  int durationSec;
  float danceability;
  float energy;
  float tempo;
  float valence;
};

// function to print songs in struct SongInformation
void printSongs(struct SongInformation* s,int numOfSongs) {
  int i;
  char seconds[3];
  for(i=0;i<numOfSongs;i++) {
    if (s[i].durationSec<10){
      sprintf(seconds,"0%d",s[i].durationSec);
    }
    else {
      sprintf(seconds,"%d",s[i].durationSec);
    }
    printf("%d)  %-20s  %-35s duration: %d:%s D: %-10f E: %-10f T: %-10f V: %-1>
    i,s[i].artist,s[i].title,s[i].durationMin,seconds,s[i].danceability,s[i].en>
  }
}

struct node {
  struct SongInformation val;
  struct node* next;
};

struct node* insert_front(struct SongInformation val, struct node* head) {
  struct node* n = malloc(sizeof(struct node));
  if (n == NULL) {
    printf("ERROR: Out of space!\n");
    exit(1);
  }
  n->val = val;
  n->next = head;
  return n;
}

void print(struct node* list) {
  for (struct node* n = list; n != NULL; n = n->next) {
    printf("Val: %d\n", n->val);
  }
}

int main() {
  struct node* list = NULL;
  list  = insert_front(2, NULL);
  list = insert_front(1,list);
  list = insert_front(0,list);
