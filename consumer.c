#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#include "sharedData.h"

sem_t mutexLock;

void unlinkSHM(void) {
	shm_unlink("osproj");
}

int main(int argc, char *argv[]) {
	atexit(unlinkSHM);
	int fd = shm_open("osproj", O_RDWR, 0666);
	while(fd<0) {
		fd = shm_open("osproj", O_RDWR, 0);
	}
	if(fd < 0) {
                printf("failed to open shared memory\n");
		return 0;
        }
	ftruncate(fd, 1024);
	if(fd < 0) {
		printf("failed to open shared memory\n");
		return 0;
	}
	struct data *mem = mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	int prodCons = 10;
	int i = 1;
	while(prodCons>0) {
		sem_wait(&mem->mutex);
		printf("conumer consuming\n");
		if(mem->table[i]>-1) {
			printf("Cell %d is filled with value %d . Clearing cell\n", i, mem->table[i]);
			mem->table[i] = -1;
			i++;
		}
		else {
			printf("Table element %d is already empty, attempting again in the next critical section\n", i);
		}
		if(i>1) {
			i = 0;
		}
		prodCons--;
		sem_post(&mem->mutex);
		sleep(1);
	}
}

