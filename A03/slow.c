/*----------------------------------------------
 * Author:Pranav Rane
 * Date: Feb 6, 2023
 * Description: Ent speech
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main() {
	// get length and text from user
	int length;
	printf("Pause length: ");
	scanf(" %d",&length);
	printf("\nText: ");
	char buff[32];
	scanf(" %s",buff);
	// get string length
	int strLen;
	strLen = strlen(buff);
	printf("\n\n");
	// memory allocation
	char* entSpeech = NULL;
	entSpeech = malloc(sizeof(char)*strLen + strLen*length + 1);
	// store ent speech
	int i,j,k;
	k = 0;
	for(i=0;k<strLen;i=i+length+1) {
		entSpeech[i] = buff[k];
		//printf("%c",buff[i]);
		for(j=1;j<length+1;j++) {
			//printf(".");
			entSpeech[i+j] = '.';
		}
	k = k+1;
	}
	entSpeech[strLen*length+strLen] = '\0';
	printf("%s \n",entSpeech);
	free(entSpeech);
	return 0;
}
