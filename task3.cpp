#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Arguments not given.\n");
		return 0;
	}

	for (int i = 1; i < argc; ++i) {
		if (fork() == 0){
			int time = 0;
			sscanf(argv[i], "%d", &time);
			usleep(5000*time);
			printf("%d\n", time);
			return 0;
		}
	}
	for (int i = 1; i < argc; ++i)
		wait(NULL);
}
