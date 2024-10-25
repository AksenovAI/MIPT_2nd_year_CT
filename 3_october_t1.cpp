#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define buf_sz 1024


int main(int argc, char** argv){
    char buf[1024];
    int fd[2];
    if(pipe(fd) < 0) return 0;
    pid_t id = fork();
    if(id == 0){
        printf("\n[1_child id %d]\n", getpid());
        strcpy(buf, "Hello world");
        write(fd[1], buf, strlen(buf));
        printf("\n[2_child id %d]\n", getpid());
    } else {
        printf("\n[1_parent id %d]\n", getpid());
        int r = read(fd[0], buf, 1024);
        write(1, buf, r);
        printf("\n[2_parent id %d]\n", getpid());
    }
    close(fd[0]);
    close(fd[1]);
    return 0;
}
