#include <dirent.h>
#include <getopt.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct msgbuf {
    long mtype;
    long from;
    char mtext[1024];
};

int runner(__id_t mes_id, struct msgbuf* mes_buf, int inner_pid);
int judge( __id_t mes_id, struct msgbuf* mes_buf, int num_of_runners);

int main(void){
    __key_t key = IPC_PRIVATE;
    __id_t  mes_id  = msgget(key, IPC_CREAT | 0700);
    struct msgbuf mes_buf; 
    int num_of_runners = 5;
    pid_t process_id = getpid();
    char perror_buf[1024];

    for(int i = 2; i < num_of_runners + 2; i++){
        process_id = fork();
        if(process_id == 0){
            runner(mes_id, &mes_buf, i);
            return 0;
        }
    }
    judge(mes_id, &mes_buf, num_of_runners);
    for(int i = 2; i < num_of_runners+2; i++) wait(NULL);
    msgctl(mes_id, IPC_RMID, 0);
}

int runner(__id_t mes_id, struct msgbuf* mes_buf, int inner_pid){
    mes_buf->mtype = 1;
    mes_buf->from  = inner_pid;
    msgsnd(mes_id, mes_buf, sizeof(long), 0);
    printf("runner %d: arrival mes out\n", inner_pid-1);
    msgrcv(mes_id, mes_buf, sizeof(long), inner_pid, 0);
    printf("runner %d: got stick\n", inner_pid-1);
    mes_buf->mtype = inner_pid + 1;
    mes_buf->from  = inner_pid;
    printf("runner %d: handed over stick\n", inner_pid-1);
    msgsnd(mes_id, mes_buf, sizeof(long), 0);
    return 0;
}

int judge(__id_t mes_id, struct msgbuf* mes_buf, int num_of_runners){
    for(int i = 1; i < num_of_runners+1; i++){
        msgrcv(mes_id, mes_buf, sizeof(long), 1, 0);
        printf("[judge] : runner %ld arrival mes resived\n", mes_buf->from-1);
    }
    mes_buf->mtype = 2;
    mes_buf->from  = 1;
    printf("[judge] : start\n");
    msgsnd(mes_id, mes_buf, sizeof(long), 0);
    msgrcv(mes_id, mes_buf, sizeof(long), num_of_runners + 2, 0);
    printf("[judge] : finish\n");
    return 0;
}
