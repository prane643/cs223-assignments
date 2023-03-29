/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Mar 22
 * Description: Practicing fork
 ---------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  pid_t pid, mypid;
  mypid = getpid();
  printf("%d] A0\n",mypid);
  pid = fork();
  if (pid==0) {
    // child process
    mypid = getpid();
    printf("%d] B1\n",mypid);
  }
  else {
    // parent process
    mypid = getpid();
    printf("%d] B0\n",mypid);
    pid = fork();
    if (pid==0) {
      // child process
      mypid = getpid();
      printf("%d] C1\n",mypid);
    }
    else {
      //parent process
      mypid = getpid();
      printf("%d] C0\n",mypid);
    }
  }
  mypid = getpid();
  printf("%d] Bye\n",mypid);
  return 0;
}
