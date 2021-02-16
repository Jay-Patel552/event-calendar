#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
int main(){
	int fd[2];
	if(pipe(fd)<0){
		exit(1);
	}
	int pid=fork();
	if(pid==0){
		// in child
		//printf("in child\n");
		close(fd[0]);
		dup2(fd[1],1);
		execl("email_filter","email_filter",NULL);
	}
	else{
		// in parent
		//printf("in parent\n");
		close(fd[1]);
		dup2(fd[0],0);
		execl("calendar_filter","calendar_filter",NULL);
	}
	return 0;
}
