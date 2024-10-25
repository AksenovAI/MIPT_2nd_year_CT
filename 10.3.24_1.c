#include <dirent.h>                                                         
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int lsdir(char* path, int tab);
int print_tabs(int n);

int main(int argc, char** argv) {
    lsdir(argv[1], 0);
    return 0;
}

int lsdir(char* path, int tab){
    DIR* dp = opendir(path);
    struct dirent* e;
    char buf[1024];
    struct dirent* e_buf[1024]; 

    if(dp == NULL){
        perror(path);
        exit(-1);
    }    
    
    while (e = readdir(dp)){
        print_tabs(tab);
        printf("[%10.ld] ", e->d_ino);
        switch (e->d_type){
        case DT_BLK     : printf("[%s] ", "DT_BLK    "); break;
        case DT_CHR     : printf("[%s] ", "DT_CHR    "); break;
        case DT_DIR     : printf("[%s] ", "DT_DIR    "); break;
        case DT_FIFO    : printf("[%s] ", "DT_FIFO   "); break;
        case DT_LNK     : printf("[%s] ", "DT_LNK    "); break;
        case DT_REG     : printf("[%s] ", "DT_REG    "); break;
        case DT_SOCK    : printf("[%s] ", "DT_SOCK   "); break;
        case DT_UNKNOWN : printf("[%s] ", "DT_UNKNOWN"); break;
        default: break;
        }
        printf("[%s]\n", e->d_name);
        if( strcmp(e->d_name, ".") != 0 
            && strcmp(e->d_name, "..") != 0 
            && e->d_type == DT_DIR)
            {
                sprintf(buf,"%s/%s",path, e->d_name);
                lsdir(buf, tab++);
            }
    }
    closedir(dp);
    return 0;
}

int print_tabs(int n){
    for(int i = 0; i < n; i++) printf("%c", 9);
}