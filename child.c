#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
void Set_priority(pid_t pid, int priority, int ID){
	struct sched_param param;
	param.sched_priority = priority;
	fprintf(stderr, "[child %d] switch to parent\n", ID + 1);
	//fprintf(stderr, "[child] being set to %d (pid = %d)\n", priority, pid);
	sched_setscheduler(pid, SCHED_FIFO, &param);
	//fprintf(stderr, "[child] back to child (pid = %d)\n", getpid());
}
void Run_a_clock_time(int iter){
	volatile unsigned long i;
	while(iter--){
		for(i = 0; i < 1000000UL; i++);
	}
}
int* Get_message(int ID){
	int fd;
	//fprintf(stderr, "here\n");
	if((fd = shm_open("_shmEE", O_RDWR, 0777)) < 0) assert(1 == 0);
	int* ptr = (int*)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	//fprintf(stderr, "in cycle %d\n", *ptr);
	close(fd);
	return ptr;
}
int main(int argc, char **argv){
	assert(argc == 3);
	int iter = atoi(argv[1]);
	int ID = atoi(argv[2]);
	int *t= Get_message(ID);
	fprintf(stderr, "[child %d] first in, total iter = %d\n", ID + 1, iter);
	//getnstimeofday();
	Set_priority(getpid(), 1, ID);
	//sched_yield();
	//fprintf(stderr, "[child] back to child...\n");
	
	while(iter > 0){
		//Run_a_clock_time(*t);
		iter -= *t;
		//fprintf(stderr, "in cycle2\n");
		fprintf(stderr, "[child %d] got message: %d, remain iter: %d\n", ID+1, *t, iter);
		
		if(iter <= 0){
			//getnstimeofday();
			//printk();
			fprintf(stderr, "[child %d] finish!\n", ID+1);
			exit(0);
		}
		
		Set_priority(getpid(), 1, ID);
		//sched_yield();
	
	}
}