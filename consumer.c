#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "shm-01.h"

int main()
{
	sleep(1);
	key_t ShmKEY;
	int ShmID;
	struct Memory *ShmPTR;
	//key, ID, and ptr for memory structure
	ShmKEY = ftok(".", 'x'); //generates shared memory key
	ShmID = shmget(ShmKEY, sizeof(struct Memory), 0666);//allocates shared memory
	if(ShmID<0){
		printf("shmget error (consumer)\n");
		exit(1);
	}
	printf("\tConsumer has received a shared memory of 2 integers...\n");
	ShmPTR = (struct Memory *) shmat(ShmID, NULL, 0);//attatches shared memory to ptr
	if((int) ShmPTR == -1){
		printf("shmat error (consumer)\n");
		exit(1);
	}
	printf("\tConsumer has attatched shared memory of 2 integers...\n");
	int loop=10;
	printf("Consumer ready to take %d items.\n",(loop*2));
	int index=0;
	int item[2];//temporary stores ints for printing
	for(int i=0; i<loop; i++){
		while(ShmPTR->status != FILLED)//waits for producer to fill array
			;
		item[index] = ShmPTR->data[index];
		index++;
		item[index] = ShmPTR->data[index];//data taken from shared mem
		printf("\t\t\t\t\t\tReceived data: %d and %d.\n", item[0], item[1]);//prints received data
		index=0;
		ShmPTR->status = TAKEN;//flags producer buffer is empty
		printf("Consumer sent TAKEN status to producer.\n");
	}
	shmdt((void *) ShmPTR);//detatches shared memory
	printf("Consumer cleaned up.\n");
	return 0;
}

