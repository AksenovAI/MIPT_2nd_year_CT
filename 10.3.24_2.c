#include <dirent.h>                                                     
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
//для getpwuid getgrgid
#include <sys/types.h>
#include <grp.h> 
#include <pwd.h>



enum STAT_MODE_MASKS{

MS_IFMT     = 0x0170000,   //bit mask for the file type bit field
MS_IFSOCK   = 0x0140000,   //socket
MS_IFLNK    = 0x0120000,   //symbolic link
MS_IFREG    = 0x0100000,   //regular file
MS_IFBLK    = 0x0060000,   //block device
MS_IFDIR    = 0x0040000,   //directory
MS_IFCHR    = 0x0020000,   //character device
MS_IFIFO    = 0x0010000,   //FIFO

MS_ISUID    = 0x0004000,   //set-user-ID bit (see execve(2))
MS_ISGID    = 0x0002000,   //set-group-ID bit (see below)
MS_ISVTX    = 0x0001000,   //sticky bit (see below)

MS_IRWXU    = 0x0000700,   //owner has read, write, and execute permission
MS_IRUSR    = 0x0000400,   //owner has read permission
MS_IWUSR    = 0x0000200,   //owner has write permission
MS_IXUSR    = 0x0000100,   //owner has execute permission

MS_IRWXG    = 0x0000070,   //group has read, write, and execute permission
MS_IRGRP    = 0x0000040,   //group has read permission
MS_IWGRP    = 0x0000020,   //group has write permission
MS_IXGRP    = 0x0000010,   //group has execute permission

MS_IRWXO    = 0x0000007,   //others (not in group) have  read,  write,  and execute permission
MS_IROTH    = 0x0000004,   //others have read permission
MS_IWOTH    = 0x0000002,   //others have write permission
MS_IXOTH    = 0x0000001    //others have execute permission
};


int lsdir(char* path, int tab);
int print_tabs(int n);
int print_stat(struct stat* st);

int main(int argc, char** argv) {
    lsdir(argv[1], 0);
    return 0;
}

int lsdir(char* path, int tab){
    struct dirent* e;
    struct stat    st_buf[1024];    
    struct stat st;
    char           path_buf[1024];
    struct dirent* e_buf[1024]; 
    int i = 0;
    DIR* dp = opendir(path);
    for (i = 0; e = readdir(dp); i++){
        e_buf[i] = e;
    }
    for(int k = 0; k < i; k++){
        print_tabs(tab);
        sprintf(path_buf, "%s/%s", path, e_buf[k]->d_name );
        stat(path_buf, st_buf+k);
        print_stat(st_buf + k);
        printf("%s", e_buf[k]->d_name);
        printf("\n");
    }
    for(int k = 0; k < i; k++){
        sprintf(path_buf, "%s/%s", path, e_buf[k]->d_name );
        lsdir(path_buf, tab+1);
    }

    closedir(dp);
    return 0;
}

int print_tabs(int n){
    for(int i = 0; i < n; i++) printf("%c", 9);
}

int print_stat(struct stat* st){
    printf("[%10.ld]", st->st_ino);
    printf(" [");
    if(st->st_mode & MS_IFDIR == MS_IFDIR){printf("d");} else {printf("-");}
    if(st->st_mode & MS_IRUSR == MS_IRUSR){printf("r");} else {printf("-");}
    if(st->st_mode & MS_IWUSR == MS_IWUSR){printf("w");} else {printf("-");}
    if(st->st_mode & MS_IXUSR == MS_IXUSR){printf("x");} else {printf("-");}
    if(st->st_mode & MS_IRGRP == MS_IRGRP){printf("r");} else {printf("-");}
    if(st->st_mode & MS_IWGRP == MS_IWGRP){printf("w");} else {printf("-");}
    if(st->st_mode & MS_IXGRP == MS_IXGRP){printf("x");} else {printf("-");}
    if(st->st_mode & MS_IROTH == MS_IROTH){printf("r");} else {printf("-");}
    if(st->st_mode & MS_IWOTH == MS_IWOTH){printf("w");} else {printf("-");}
    if(st->st_mode & MS_IXOTH == MS_IXOTH){printf("x");} else {printf("-");}
    printf("] ");
    printf("[%5.ld]", st->st_nlink);
    printf(" ");
    struct passwd* owner_info = getpwuid(st->st_uid);
    struct group*  ow_gr_info = getgrgid(st->st_gid);
    printf("[%s] ", owner_info->pw_name);
    printf("[%s] ", ow_gr_info->gr_name);
    printf("[%10.ld] ", st->st_size);
    printf("[%12.ld] ", st->st_mtime);
}




/*
struct stat {
    dev_t     st_dev;          ID of device containing file 
    ino_t     st_ino;          Inode number 
    mode_t    st_mode;         File type and mode 
    nlink_t   st_nlink;        Number of hard links 
    uid_t     st_uid;          User ID of owner 
    gid_t     st_gid;          Group ID of owner 
    dev_t     st_rdev;         Device ID (if special file) 
    off_t     st_size;         Total size, in bytes 
    blksize_t st_blksize;      Block size for filesystem I/O
    blkcnt_t  st_blocks;       Number of 512B blocks allocated 
}


SYNOPSIS
       #define _GNU_SOURCE             See feature_test_macros(7) 
       #include <sys/types.h>
       #include <pwd.h>

       int getpw(uid_t uid, char *buf);

        DESCRIPTION
       The getpw() function reconstructs the password line entry for the given user ID uid in the buffer buf.  The returned buffer contains a line of for‐
       mat

           name:passwd:uid:gid:gecos:dir:shell

       The passwd structure is defined in <pwd.h> as follows:

           struct passwd {
               char   *pw_name;        username 
               char   *pw_passwd;      user password 
               uid_t   pw_uid;         user ID 
               gid_t   pw_gid;         group ID
               char   *pw_gecos;       user information 
               char   *pw_dir;         home directory 
               char   *pw_shell;       shell program 
           };


*/

