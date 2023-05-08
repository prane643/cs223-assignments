/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Feb 1
 * Description: replace lowercase letter with symbol
 ---------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main() {
	printf("Enter a word: ");
	char inputStr[32];
	scanf(" %s", inputStr);
	printf(" \n OMG! ");
	const char* symbolList = "@!#*";
	int i=0;
	// set seed for random numbers
	time_t t;
	srand((unsigned) time(&t));
	// loop through input string
	int randNum;
	while (inputStr[i]!='\0'){
		if (inputStr[i]>=97 && inputStr[i]<=122){
			randNum = rand()%4;
			printf("%c",symbolList[randNum]);
			i = i+1;
		}
		else {
			printf("%c",inputStr[i]);
			i = i+1;
		}
	}
	printf("\n");
	return 0;
}
