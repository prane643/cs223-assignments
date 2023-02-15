/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Feb 13
 * Description
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define struct to be used in node val
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
void printSongs(struct SongInformation* s) {
  //int i;
  char seconds[3];
  //for(i=0;i<numOfSongs;i++) {
    if (s->durationSec<10){
      sprintf(seconds,"0%d",s->durationSec);
    }
    else {
      sprintf(seconds,"%d",s->durationSec);
    }
    printf("%-20s  %-35s duration: %d:%s D: %-10f E: %-10f T: %-10f V: %-10f",
      s->artist,s->title,s->durationMin,seconds,s->danceability,s->energy,s->tempo,s->valence);
    printf("\n");
  //}
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
  head = n;
  return n;
}

void print(struct node* list) {
  for (struct node* n = list; n != NULL; n = n->next) {
    printSongs(&list->val);
  }
}

int main() {
  FILE *infile;
	infile = fopen("songlist.csv","r");
	if (infile==NULL) {
		printf("There was an error reading the file");
		exit(1);
	}
  // skip header
  char buff[128];
	fgets(buff,128,infile);
  // define delimeter, token, and iteration value
  const char delim[2]=",";
  char* token;
  //int i=0;
  // start linked list
  struct node* head = NULL;
  //list  = insert_front(2, NULL);
	while(fgets(buff,128,infile)!=NULL){
    // create new struct
    struct SongInformation *s;
    // read in current line
		fgets(buff,128,infile);
    // input line information into struct 
		token = strtok(buff,delim);
		strcpy(s->title,token);
		token = strtok(NULL,delim);
		strcpy(s->artist,token);
		token = strtok(NULL,delim);
		int durMin = (atoi(token)/1000)/60;
		int durSec = (atoi(token)/1000)%60;
		s->durationMin = durMin;
		s->durationSec = durSec;
		token = strtok(NULL,delim);
		s->danceability = atof(token);
		token = strtok(NULL,delim);
		s->energy = atof(token);
		token = strtok(NULL,delim);
		s->tempo = atof(token);
		token = strtok(NULL,delim);
 		s->valence = atof(token);
    // add new node to linked list
    insert_front(*s,head);
	}

print(head);



}
