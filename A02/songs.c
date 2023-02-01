#include <stdio.h>
#include <string.h>

struct SongInformation {
	char artist[64];
	char title[64];
	int durationMin;
	int durationSec;
	float danceability;
};

void printSongs(struct SongInformation s[3],int numOfSongs) {
	int i;
	for(i=0;i<numOfSongs;i++) {
		printf("%d) %-20s  %-35s duration: %d:%d danceability: %-10f \n",
		i,s[i].artist,s[i].title,s[i].durationMin,s[i].durationSec,s[i].danceability);
  }
}

int main() {
	int n=3;
	struct SongInformation song[3];

	strcpy(song[0].title,"Solitude");
	strcpy(song[0].artist,"M83");
	song[0].durationMin = 3;
	song[0].durationSec = 42;
	song[0].danceability = 0.1;

	strcpy(song[1].title,"Everybody Wants To Rule The World");
	strcpy(song[1].artist,"Tears For Fears");
	song[1].durationMin = 4;
	song[1].durationSec = 11;
	song[1].danceability = 0.7;

	strcpy(song[2].title,"Borderline");
	strcpy(song[2].artist,"Tame Impala");
	song[2].durationMin = 3;
	song[2].durationSec = 57;
	song[2].danceability = 0.4;

	// print out initialized songs
	printf("\n Welcome to the song list. \n");
	printSongs(song,n);
	printf("\n\n =========================\n\n");

	printf("Enter a song id to edit [0,1,2]: ");
	int songId;
	scanf(" %d",&songId);
	// only allow valid song ID
	if (songId<0 || songId>(n-1)){
		printf("Error. Invalid song Id\n");
		return 0;
	}

	printf("\n Which attribute would you like to edit? [artist,title,duration,danceability]: \n");
	char attribute[20];
	scanf(" %s",attribute);
	if (strcmp(attribute,"artist")==0){
		printf("\n Enter %s:",attribute);
		char newArtist[64];
		scanf(" %[^\n]%*c",newArtist);
		strcpy(song[songId].artist,newArtist);
	}
	else if (strcmp(attribute,"title")==0){
		printf("\n Enter %s:",attribute);
		char newTitle[64];
		scanf(" %[^\n]%*c",newTitle);
		strcpy(song[songId].title,newTitle);
  }
	else if (strcmp(attribute,"duration")==0){
		printf("\n Enter the duration (min): ");
		int newDurationMin;
		scanf(" %d%*c",&newDurationMin);
		printf("\n Enter the duration (sec): ");
		int newDurationSec;
		scanf(" %d%*c",&newDurationSec);
		song[songId].durationMin = newDurationMin;
		song[songId].durationSec = newDurationSec;
	}
	else if (strcmp(attribute,"danceability")==0){
		printf("\n Enter the danceability: ");
		float newDanceability;
		scanf(" %f",&newDanceability);
		song[songId].danceability = newDanceability;
	}
	else {
		printf("\n Error: not a valid attribute \n");
		return 0;
	}
	printSongs(song,n);
	return 0;
}


