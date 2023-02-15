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

void printSongs(struct SongInformation s) {
  char seconds[3];
    if (s.durationSec<10){
      sprintf(seconds,"0%d",s.durationSec);
    }
    else {
      sprintf(seconds,"%d",s.durationSec);
    }
    /*
    printf("%-20s  %-35s duration: %d:%s D: %-10f E: %-10f T: %-10f V: %-10f",
      s->artist,s->title,s->durationMin,seconds,s->danceability,s->energy,s->tempo,s->valence);
    */
    printf("%-20s  %-35s duration: %d:%s D: %-10f E: %-10f T: %-10f V: %-10f"
    ,s.artist,s.title,s.durationMin,seconds,s.danceability,s.energy,s.tempo,s.valence);
    printf("\n");
}

int print(struct node* list) {
    int i=0;
    for (struct node* n = list; n != NULL; n = n->next) {
        printf("%d) ",i);
        printSongs(n->val);
        i = i+1;
    }
    return i;
}

struct node* printAndRemoveDance(struct node* list) {
    float highestDanceable=0.0, currentDanceable=0.0;
    int idx,i = 0;
    
    for (struct node* n = list; n != NULL; n = n->next) {
        struct SongInformation k;
        k = n->val;
        currentDanceable = k.danceability;
        if (currentDanceable>highestDanceable){
            highestDanceable = currentDanceable;
            idx = i;
        }
        //printSongs(n->val);
        i = i+1;
    }

    // now go back and print out the song
    i = 0;
    for (struct node* n = list; n != NULL; n = n->next) {
        if (i==idx){
            struct SongInformation k;
            k = n->val;
            printf("\nMax danceable is %f\n",k.danceability);
        }
        i = i+1;
    }

    // now go back and remove from linked list
    i = 0;
    struct node* returnList;
    struct node* temp;
    struct node* n;
    for (n = list; n != NULL; n = n->next) {
        if (idx==0){
            // removing first element of list, just pop the first element
            returnList = n->next;
            free(n);
            return returnList;
        }
        if (i==idx-1){
            // get address of the previous node we want to remove
            temp = n;
            n = n->next;
            temp->next = n->next;
            free(n);
            return list;
        }
        //if (i==idx) {
        //    temp->next = n->next;
        //    free(n);
        //    return temp;
        //}
        i = i+1;
    }
    if (n==NULL){
            // removing end node of list
            free(n);
            return list;
    }

    //return returnList;
}




int main() {
    // read in correct file
    FILE *infile;
	infile = fopen("songlist.csv","r");
	if (infile==NULL) {
		printf("There was an error reading the file");
		exit(1);
	}
    // skip header
    char buff[128];
	fgets(buff,128,infile);
    // define delimeter, token, and first node pointer
    const char delim[2]=",";
    char* token;
    struct node* n;
    int i=0;
    while(fgets(buff,128,infile)!=NULL){
        // create new struct
        struct SongInformation s;
        // input line information into struct 
        token = strtok(buff,delim);
        strcpy(s.title,token);
        token = strtok(NULL,delim);
        strcpy(s.artist,token);
        token = strtok(NULL,delim);
        int durMin = (atoi(token)/1000)/60;
        int durSec = (atoi(token)/1000)%60;
        s.durationMin = durMin;
        s.durationSec = durSec;
        token = strtok(NULL,delim);
        s.danceability = atof(token);
        token = strtok(NULL,delim);
        s.energy = atof(token);
        token = strtok(NULL,delim);
        s.tempo = atof(token);
        token = strtok(NULL,delim);
        s.valence = atof(token);
        if (i==0) {
            n = insert_front(s,NULL);
        }
        else {
            n = insert_front(s,n);
        }
        i = i+1;
    }
    int numOfSongs;

    // print songs and get number of songs
    numOfSongs = print(n);
    // display how many songs there are in dataset
    printf("\nThe dataset contains %d song(s)\n",numOfSongs);

    // ask user if they want to show the most danceable songs
    
    while (1==1) {
        printf("\nEnter 'd' to show the most danceable song (any other "
        "key to quit): ");
        char userInput;
        scanf(" %c",&userInput);
        if (userInput!='d') {
            return 0;
        }
        else {
            // if here need to show most danceable song and remove it
            struct node* t;
            n = printAndRemoveDance(n);
            // print remaining songs and get number of songs
            numOfSongs = print(n);
            // display how many songs there are in dataset
            printf("\nThe dataset contains %d song(s)\n",numOfSongs);
        }
    }


}




/*

    struct SongInformation s;
    strcpy(s.title,"firstSong");
    strcpy(s.artist,"firstArtist");
    s.durationMin = 1;
    s.durationSec = 10;
    s.danceability = 0.5;
    s.energy = 0.5;
    s.tempo = 0.5;
    s.valence = 0.5;
    struct node* n;
    n = insert_front(s,NULL);
    strcpy(s.title,"secondSong");
    strcpy(s.artist,"secondArtist");
    s.durationMin = 1;
    s.durationSec = 10;
    s.danceability = 0.5;
    s.energy = 0.5;
    s.tempo = 0.5;
    s.valence = 0.4;
    //struct node* n;
    n = insert_front(s,n);
    strcpy(s.title,"thirdSong");
    strcpy(s.artist,"thirdArtist");
    s.durationMin = 1;
    s.durationSec = 10;
    s.danceability = 0.5;
    s.energy = 0.5;
    s.tempo = 0.5;
    s.valence = 0.3;
    //struct node* n
    n = insert_front(s,n);

    print(n);
    //print(n2);
    //print(n1);

    //free(n1);
    //free(n2);
    //free(n3);

*/



