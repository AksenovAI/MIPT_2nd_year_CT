#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char** argv) {
    char str_buf[4096];           // куда считывам строку
    char* token_pointers[4096];   // здесь храним разделение ||
    char* execvp_buf[1024];       // здесь каждый из подпроцессов хранит аргументы
    //char ch_process_name[1024];
    int fd_buf[1024];                // сюда будут добавляться все fd
    int fd_counter = 2;
    int symbols_read = 0;         // здесь будет число считанных символов
    int read_state = 1;           // это служебная переменная в которую read возвращает число считанных ща раз символов или NULL
    int tok_ct = 0;               // это чило блоков на которые была разделенна строка
    int arg_ct = 0;               // счетчик аргументов для вызова
    int children_counter = 0;     // счетчик дочерних процессов
    int child_type = 0;        // переменная определяющая тип потомка
    pid_t children[1024];         // массив куда будем записывать дочерние процессы
    char* s;
    fd_buf[0] = 0;
    fd_buf[1] = 1;
    printf("ready to read\n");
    while(read_state){
        read_state = read(0, str_buf + symbols_read, 4096);
        symbols_read += read_state;
    }
    s = strtok(str_buf, "|");
    for(tok_ct = 0; s != NULL; tok_ct++){
        token_pointers[tok_ct] = s;
        s = strtok(NULL, "|");
    }
    for(children_counter = 0; children_counter < tok_ct; children_counter++){
        if     (tok_ct           == 1)        child_type = -1;
        else if(children_counter == 0)        child_type = 0;
        else if(children_counter == tok_ct-1) child_type = 2;
        else                                  child_type = 1;
        pipe(fd_buf + fd_counter);
        fd_counter += 2;
        children[children_counter] = fork();
        if(children[children_counter] == 0){
            s = strtok(token_pointers[children_counter], " ");
            for(arg_ct = 0; s != NULL; arg_ct++){
                execvp_buf[arg_ct] = s;
                s = strtok(NULL, " ");
            }
            printf("\n"); 
            switch(child_type){
            case 0:
                dup2 (fd_buf[fd_counter - 1], 1);
                break;
            case 1:
                dup2 (fd_buf[fd_counter - 4], 0);
                dup2 (fd_buf[fd_counter - 1], 1);
                break;
            case 2:
                dup2 (fd_buf[fd_counter - 4], 0);
                break;
            default:
                break;
            }
            for (int i = 2; i < fd_counter; i++) close(fd_buf[i]);
            execvp(execvp_buf[0], execvp_buf);
        }
    }
    for (int i = 2; i < fd_counter; i++) close(fd_buf[i]);
    for (int i = 0; i < children_counter; ++i) wait(NULL);
    return 0;     
}    

                                                         