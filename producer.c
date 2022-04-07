//#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>

#include "sharedData.h"

void unlinkSHM(void) {
	shm_unlink("osproj");
}

int main() {
	atexit(unlinkSHM);
	int fd = shm_open("osproj", O_CREAT | O_RDWR, 0666);
	if(fd<0) {
                printf("Failed to initialize shared memory.\n");
                return 0;
        }
	ftruncate(fd, 1024);
	struct data *mem = mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(fd<0) {
		printf("Failed to initialize shared memory.\n");
		return 0;
	}
	mem->table[0] = -1;
        mem->table[1] = -1;
	sem_init(&mem->mutex, 1, 1);
	srand(time(NULL));
	int i = 0;
	int prodCons = 10;
	while(prodCons>0) {
		sem_wait(&mem->mutex);
		printf("Producer producing.\n");
		if(mem->table[i]==-1) {
			int temp = rand();
			mem->table[i] = temp;
			printf("filling cell %d with value %d \n", i, temp);
			i++;
		}
		else {
			printf("Cell %d is filled, checking again in the next critical section\n", i);
		}
		if(i>1) {
			i = 0;
		}
		prodCons--;
		sem_post(&mem->mutex);
		sleep(1);
	}
}
