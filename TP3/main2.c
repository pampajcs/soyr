#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

void trapper(int);
void trapper2(int);
int j=0;
int i=0;
int c=0;
int k=0;
int proceso=0;
pid_t var;
pid_t var2;
pid_t pid_padre=0;
pid_t pid_hijo=0;
pid_t pid_nieto=0;

int main(int argc, char *argv[]){
    int status;
	signal (SIGUSR1, trapper);
	signal (SIGUSR2, trapper2);
	pid_padre=getpid();
	printf("soy el padre y mi PID es %d \n",getpid());
	while (1){
		if(getpid() == pid_hijo){
				printf("ID Hijo: %d, ID Padre: %d", getpid(), getppid());
				sleep(5);
		}
		if(getpid() == pid_nieto){
			for(c=0;c<10;c++){
				printf("ID Nieto: %d, ID Padre: %d", getpid(), getppid());
				sleep(1);
			}
		}
	}
}

void trapper(int sig){
	if((var=fork())!=0){
		pid_hijo=getpid();
		printf("PID: %d", getpid());
	}
}

void trapper2(int sig){
	if(getpid()==pid_hijo){
		var2=fork();
		if(var2==0){
			pid_nieto=getpid();
		}
	}
}