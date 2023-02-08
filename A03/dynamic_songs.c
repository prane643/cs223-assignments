/*----------------------------------------------
 * Author:Pranav Rane
 * Date: Feb 6, 2023
 * Description: Read files
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    printf("%d)  %-20s  %-35s duration: %d:%s D: %-10f E: %-10f T: %-10f V: %-10f\n",
    i,s[i].artist,s[i].title,s[i].durationMin,seconds,s[i].danceability,s[i].energy,s[i].tempo,s[i].valence);
  }
}

int main() {
	printf("Welcome to Dynamic Donna's Danceability Directory\n\n");
	// read in file
	FILE *infile;
	infile = fopen("songlist.csv","r");
	if (infile==NULL) {
		printf("There was an error reading the file");
		exit(1);
	}
	// get first line of file
	char buff[128];
	char* token;
	fgets(buff,128,infile);
	const char delim[2]=",";
	token = strtok(buff,delim);
	int numOfSongs = atoi(token);
	// memory allocate space for struct of size numOfSongs
	struct SongInformation t;
	struct SongInformation *s = malloc(sizeof(t)*numOfSongs);

	// skip header
	fgets(buff,128,infile);
	// continue to get songs and store them in struc
	int i;
	for(i=0;i<numOfSongs;i++){
		fgets(buff,128,infile);
		token = strtok(buff,delim);
		strcpy(s[i].title,token);
		token = strtok(NULL,delim);
		strcpy(s[i].artist,token);
    token = strtok(NULL,delim);
		int durMin = (atoi(token)/1000)/60;
		int durSec = (atoi(token)/1000)%60;
    s[i].durationMin = durMin;
		s[i].durationSec = durSec;
    token = strtok(NULL,delim);
    s[i].danceability = atof(token);
    token = strtok(NULL,delim);
    s[i].energy = atof(token);
    token = strtok(NULL,delim);
    s[i].tempo = atof(token);
    token = strtok(NULL,delim);
    s[i].valence = atof(token);
		//while (token != NULL) {
			//printf("%s\n",token);
		//	token = strtok(NULL,s);
		//}
	}
	printSongs(s,numOfSongs);
	free(s);
	fclose(infile);
	return 0;
}
