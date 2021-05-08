#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
pid_t child_id;
int status, status2, fp1[2], fp2[2];

if (pipe(fp1)==-1){
  fprintf(stderr, "Pipe Failed" );
  return 1;
}

if (pipe(fp2)==-1){
  fprintf(stderr, "Pipe Failed" );
  return 1;
}

child_id = fork();
 if (child_id < 0) {
  exit(EXIT_FAILURE);
}

if (child_id == 0) {
  close(fp1[0]); // read ditutup
  dup2(fp1[1], STDOUT_FILENO);
  char *argv[] = {"ps", "-aux", NULL};
  execv("/bin/ps", argv);
} else {  // this is parent
  while ((wait(&status)) > 0);
  child_id = fork();
   if (child_id < 0) {
    exit(EXIT_FAILURE);
      }
   if (child_id == 0){
    close(fp1[1]); // write ditutup
    dup2(fp1[0], STDIN_FILENO);
    close(fp2[0]); // read ditutup
    dup2(fp2[1], STDOUT_FILENO);
    char *argv1[] = {"sort", "-nrk", "3,3", NULL};
	execv("/usr/bin/sort", argv1);
  }else{
    close(fp1[1]); // write ditutup
    while ((wait(&status2)) > 0);
    close(fp2[1]); // write ditutup
    dup2(fp2[0], STDIN_FILENO);
    char *argv2[] = {"head", "-5", NULL};
	execv("/usr/bin/head", argv2);
  }
} return 0;
}
