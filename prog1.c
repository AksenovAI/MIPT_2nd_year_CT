#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <grp.h>
#include <pwd.h>


int main(int argc, char* argv[]){
    //int cur_pr = (int)getpid();
    //int par_pr = (int)getppid();

    //printf("processes\ncurrent %d\nparent  %d\n\n", cur_pr, par_pr);
    
    uid_t cur_uid;
    //uid_t cur_euid = geteuid();
    gid_t list[32];
    
    struct passwd* usr;
    int num_of_groups = 32;
    
    if(argc < 2){
        cur_uid = getuid();
        usr = getpwuid(cur_uid);
        num_of_groups = getgroups(32, list);
    } else{
        usr = getpwnam(argv[1]);
        if(usr == NULL){
            printf("no user found\n");
            return 0;
        } 
        getgrouplist(argv[1], usr->pw_gid, list, &num_of_groups);
    }
    
    printf("%s\n%15s%20s\n%15s%20s\n%15s%10d\n%15s%10d\n%15s%20s\n%15s%20s\n%15s%20s\n\n\n",
    "user", 
    "username",           usr->pw_name,
    "user password",      usr->pw_passwd,
    "user ID",            usr->pw_uid,
    "group ID",           usr->pw_gid,
    "user information",   usr->pw_gecos,
    "home directory",     usr->pw_dir,
    "shell program",      usr->pw_shell);
    
    struct group* str;
    printf("%15s %10s %10s\n", "gr name", "pw", "gid");
    for(int i = 0; i < num_of_groups; i++){
        str = getgrgid(list[i]);
        printf("%15s %10s %10d\n", str->gr_name, str->gr_passwd, str->gr_gid);
    }
    return 0;
}