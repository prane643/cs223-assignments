/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Feb 1
 * Description: Make acronym from input
 ---------------------------------------------*/


#include <stdio.h>
#include <string.h>

int main() {
	printf("Enter a phrase: ");
	char buff[1024];
	char acronym[32];
	scanf("%[^\n]%*c",buff);
	printf("\n");
	int i,n,j;
	j = 0;
	n = strlen(buff);
	printf("You acronym is: ");
	for (i=0;i<n;i++) {
		if (buff[i]=='\0'){
			return 0;
		}
		if (buff[i]>=65 && buff[i]<=90){
			acronym[j] = buff[i];
			j = j+1;
		}
	}
	acronym[j+1] = '\0';
	printf("%s \n",acronym);
	return 0;
}
