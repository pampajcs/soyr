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

int main(int argc, char *argv[])
	{
    int status;
	signal (SIGUSR1, trapper);
	signal (SIGUSR2, trapper2);
	pid_padre=getpid();
	printf("soy el padre y mi PID es %d \n",getpid());
	while (1)
		{
			pause();
			proceso=i+j*10;
			switch(proceso){
				case 1:
					var=fork();
					if(getpid()!=pid_padre){
						while(1){
							printf("Soy un hijo del proceso: %d y mi PID es: %d\n",getppid(), getpid());
							sleep(5);
							if(j==1){
								break;
							}
						}
					}
					else{
						i=0;
					}
					break;
				case 11:
					var=fork();
					printf("VAR= %d\n",var);
					if(getpid() != pid_hijo && getppid() != pid_padre){
						for(c=0;c<10;c++){
							printf("Soy un hijo del proceso hijo: %d y mi PID es: %d\n",getppid(), getpid());
							sleep(1);
						}
					}
					else{
						
					}
					break;
			}
		}
	return 0;
	}

	void trapper(int sig)
	{
		i=1;
	}
	
	void trapper2(int sig)
	{
		j=1;
	}
