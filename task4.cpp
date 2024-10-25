#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char** argv) {
	struct timespec t0;
    struct timespec t1;
	clock_gettime(CLOCK_MONOTONIC, &t0);	
	pid_t cp = fork();		
        if (cp == 0){        
		printf("child PID: %d\n", getpid());
        	printf("child PPID: %d\n\n", getppid());
		execvp(argv[1], argv+2); 
		return 0;
        }
	printf("parent PID: %d\n",  getpid());
        printf("parent PPID: %d\n\n",  getppid());
        wait(NULL);
	clock_gettime(CLOCK_MONOTONIC, &t1);
	long delta = (t1.tv_sec - t0.tv_sec)*1000000 + (t1.tv_nsec-t0.tv_nsec)/1000;
	printf("time %ld us", delta);
	return 0;
}

