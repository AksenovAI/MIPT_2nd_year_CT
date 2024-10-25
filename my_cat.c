#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define buf_sz 1024

int my_write(int fd, char* buf, size_t n);
int fd_copy(int a, int b);

int main(int argc, char** argv){св
    if (argc == 1){
        fd_copy(0, 1);
    } else {
        int fd = 0;
        char* buf = malloc(buf_sz);
        for(int k = 0; k < argc-1; k++){
            fd = open(argv[1+k], O_RDONLY);
            fd_copy(fd, 1);
        }
    }
    return 0;
}

int my_write(int fd, char* buf, size_t n){
    int ct = 0;
    while(ct < n){
        ct += write(fd, buf, n-ct);   
    }
    return ct;
}

int fd_copy(int a, int b){
    char* buf = malloc(buf_sz);
    int s = 1;
    int c = 0;
    while(s){
        c = read(a, buf, buf_sz);
        if (c < 0) return -1;
        if (c == 0) s = 0;
        my_write(b, buf, c);
    }
}
