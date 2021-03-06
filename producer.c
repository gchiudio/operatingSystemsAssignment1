#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include "shm-01.h"
#include <time.h>
int main(){
	srand(time(NULL));
	int r=rand();
	key_t ShmKEY;
	int ShmID;
	struct Memory *ShmPTR;
	ShmKEY=ftok(".", 'x');//generates shared memory key
	ShmID = shmget(ShmKEY, sizeof(struct Memory), IPC_CREAT | 0666);//generates shared memory id
	if(ShmID<0){//catches error of id not generated
		printf("shmget error (producer) \n");
		exit(1);
	}
	printf("Producer has received a shared memory of 2 integers...\n");
	ShmPTR = (struct Memory *) shmat(ShmID, NULL, 0);//attatches shared memory to struct pointer 
	if((int) ShmPTR==-1){//catches attatchment error
		printf("shmat error (producer)\n");
		exit(1);
	}
	printf("Producer has attatched the shared memory...\n");
	int loop=10;
	printf("Producer prepared to make %d items.\n", (loop*2));
	int index=0;//producer makes 2 items per loop
	ShmPTR -> status = TAKEN;//sets initial status to taken (empty)
	sleep(1);
	for(int i=0; i<loop; i++){
		while(ShmPTR->status != TAKEN)//waits if buffer is full
			;
		r = rand()%50;//generates "item"
		ShmPTR->data[0]=0;
		ShmPTR->data[index]=r;//stores item in buff
		printf("\t\t\t\t\t\tStored two items: %d ", ShmPTR->data[index]);
		r = rand()%50;
		index++;
		ShmPTR->data[1]=0;
		ShmPTR->data[index] = r;//stores item in buff
		printf("and %d.\n", ShmPTR->data[index]);
		index=0;
		ShmPTR->status = FILLED;//flags consumer that buffer is full
		printf("Producer sent FILLED status to consumer.\n");
	}
	shmdt((void *) ShmPTR);//detatches shared memory
	shmctl(ShmID, IPC_RMID, NULL);//removes shared memory id
	printf("Producer cleaned up.\n");
	return 0;
}

