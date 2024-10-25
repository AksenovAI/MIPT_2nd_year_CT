#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

#define DB_ if(0)

int main(int argc, char** argv) {
DB_ perror("[start]");
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
DB_ perror("[initialisation complited]");

    while(read_state){
DB_     perror("[reading]");
        read_state = read(0, str_buf + symbols_read, 4096);
        symbols_read += read_state;
    }
DB_ perror("[read complited]");
DB_ perror(str_buf);
DB_ perror("[string printed]");
    s = strtok(str_buf, "|");
DB_ perror("[s initialized]");
DB_ perror(s);
DB_ perror("[s printed]");
    for(tok_ct = 0; s != NULL; tok_ct++){
DB_     perror("[parsing1 in process]");
        token_pointers[tok_ct] = s;
        s = strtok(NULL, "|");
    }
DB_ perror("[first parsing complete]");
DB_ printf("[number of tokens [%d]]\n", tok_ct);
    for(int q = 0; q < tok_ct; q++){
DB_     printf("%s\n", token_pointers[q]);
    }
DB_ perror("tokens printed");
    for(children_counter = 0; children_counter < tok_ct; children_counter++){
DB_     perror("main cycle start");
        if     (tok_ct           == 1)        child_type = -1;
        else if(children_counter == 0)        child_type = 0;
        else if(children_counter == tok_ct-1) child_type = 2;
        else                                  child_type = 1;
DB_     printf("type of child is %d\n", child_type);
DB_     perror("current child type printed");

        pipe(fd_buf + fd_counter);
        fd_counter += 2;

        children[children_counter] = fork();
        if(children[children_counter] == 0){
DB_         perror("[child_root_in]");

            s = strtok(token_pointers[children_counter], " ");
DB_         perror("[child_root -> s initialized]");
DB_         printf("[token {%s}]\n", token_pointers[children_counter]);
            for(arg_ct = 0; s != NULL; arg_ct++){
DB_         perror("[child_root -> parsing2 in process]");
                execvp_buf[arg_ct] = s;
                s = strtok(NULL, " ");
            }
            
DB_         perror("[child_root -> parsing complete]");
DB_         printf("[child_root -> number of tokens [%d]]\n", arg_ct);
            for(int q = 0; q < arg_ct; q++){
DB_             printf("%s\n", execvp_buf[q]);
            }
DB_         perror("[child_root -> tokens printed]");
DB_         printf("[child_root -> gd_ct = %d       file descriptors]\n", fd_counter);
DB_         for(int k = 0; k < fd_counter; k++){ printf("[%d]", fd_buf[k]);}
            printf("\n"); 
            switch(child_type){
            case 0:
DB_             perror("[child_root -> case 0]");
                dup2 (fd_buf[fd_counter - 1], 1);
                break;
            case 1:
DB_             perror("[child_root -> case 1]");
                dup2 (fd_buf[fd_counter - 4], 0);
                dup2 (fd_buf[fd_counter - 1], 1);
                break;
            case 2:
DB_             perror("[child_root -> case 2]");
                dup2 (fd_buf[fd_counter - 4], 0);
                break;
            default:
DB_             perror("[child_root -> case def]");
                break;
            }

            for (int i = 2; i < fd_counter; i++) close(fd_buf[i]);
DB_         perror("[child_rot -> execvp]");
DB_         printf("[child_root -> process id %d]\n", getpid());
DB_         perror("[child_root -> execvp]");
        
            execvp(execvp_buf[0], execvp_buf);

        }
            usleep(500000);
    }
DB_ perror("[main cycle ended]");
    for (int i = 2; i < fd_counter; i++) close(fd_buf[i]);
    for (int i = 0; i < children_counter; ++i) wait(NULL);
DB_ perror("[wait_cycle ended]");
    return 0;     
}    

                                                         