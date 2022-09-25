#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#define NMAX 256
#include <signal.h>
#include <math.h>
void sighd(int s){
	printf("I got sigint\n");
	signal(SIGINT,SIG_DFL);
}

int main ( int argc, char ** argv )
{	
	signal(SIGINT,sighd);
	key_t key; 
	int semid, shmid;
	struct sembuf sops ;
	char *shmaddr;
	char str[NMAX],str1[NMAX];
	key=ftok("/usr/ter/exmpl",'S');
	semid=semget(key,1,0666|IPC_CREAT);
	shmid=shmget(key,NMAX,0666|IPC_CREAT);
	shmaddr=shmat(shmid,NULL,0);
	semctl(semid,0,SETVAL,(int)0);
	sops.sem_num=0;
	sops.sem_flg=0;
	do{
		if(fgets(str,NMAX,stdin)==NULL)
		{
			strcpy(str,"Q");
		}
		strcpy(shmaddr,str);
		sops.sem_op=3;
		semop(semid,&sops,1);
		sops.sem_op=0;
		semop(semid,&sops,1);


		//sops.sem_op=-2;
		//semop(semid,&sops,1);
		strcpy(str1,shmaddr);
		printf("%s\n",str1);
		if(str[0]=='Q')
		{
			shmdt(shmaddr);
		}
		//sops.sem_op=-1;
		//semop(semid,&sops,1);
	
	}while(str[0]!='Q');
	shmdt(shmaddr);
	shmctl(shmid,IPC_RMID,NULL);
	semctl(semid,0,IPC_RMID,(int)0);
	return 0 ;
}


