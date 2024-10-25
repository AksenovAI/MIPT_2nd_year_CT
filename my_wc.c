#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char** argv) {
    struct timespec t0;
    struct timespec t1;
    char buf[4096];
    int fd[2];
    int n_words = 1;
    int n_lines = 1;
    int read_sate = 1;
    int bytes_count = 0;
    if(pipe(fd) < 0) return 0;
    clock_gettime(CLOCK_MONOTONIC, &t0); 
    pid_t id = fork();
    if(id == 0){
        dup2(fd[1], 1);
        close(fd[1]);
        execvp(argv[1], argv+1);
        return 0;
    }         
    //wait(NULL);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    long delta = (t1.tv_sec - t0.tv_sec)*1000000 + (t1.tv_nsec-t0.tv_nsec)/1000;
    close(fd[1]);
    while(read_sate){
        read_sate = read(fd[0], buf, 4096);
        bytes_count += read_sate;
        for(int i = 0; i < read_sate; i++){
            if(buf[i] == '\n') n_lines++;
            if(buf[i] == ' ' ) n_words++;
        }
    }

    close(fd[0]);
    
    printf("time %ld us\n", delta);
    printf("bytes %d\n", bytes_count);
    printf("words %d\n", n_words);
    printf("lines %d\n", n_lines);

    return 0;
}
