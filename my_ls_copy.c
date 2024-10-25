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
#include <getopt.h>

const char* opt_string = "liatdR";
struct option long_options[] = {
    { "long"     , 0, NULL, 'l'},
    { "inode"    , 0, NULL, 'i'},
    { "recursive", 0, NULL, 'R'},
    { "all"      , 0, NULL, 'a'},
    { "tree"     , 0, NULL, 't'},
    { "directory", 0, NULL, 'd'},
    {NULL}
};

struct op {
    unsigned op_long      :1;
    unsigned op_inode     :1; 
    unsigned op_recursive :1; 
    unsigned op_all       :1; 
    unsigned op_tree      :1; 
    unsigned op_directory :1; 
};

struct args{
    char*  op_long_arg    ;
    char*  op_inode_arg     ; 
    char*  op_recursive_arg ; 
    char*  op_all_arg       ; 
    char*  op_tree_arg      ; 
    char*  op_directory_arg ; 
};

int get_options(int argc, char** argv, struct op* options, struct args* arguments, int* longindex);
int lsdir(char* path, struct op* options, struct args* arguments, int sub_level);
int print_stat(const char* full_path, struct op* opt);

int main(int argc, char** argv) {
    int             long_index;
    struct op       options   = {0};
    struct args     arguments = {0};
    
    get_options(argc, argv, &options, &arguments, &long_index);
    //printf("[liratd]\n[%d%d%d%d%d%d]\n", options.op_long, options.op_inode, options.op_recursive, options.op_all, options.op_tree, options.op_directory);
    
    for(int i = optind; i < argc; i++){
        lsdir(argv[i], &options, &arguments, 0);
    }
    
    return 0;
}

int get_options(int argc, char** argv, struct op *options, struct args* arguments, int* long_index){
    int get_opt_ret = 1;
    while(get_opt_ret){
        get_opt_ret = getopt_long(argc, argv, opt_string, long_options, long_index);
        if (get_opt_ret == -1) break;

        //printf("%c", get_opt_ret);
        switch (get_opt_ret){
            case 'l':
                options->op_long  = 1;
                break;
            case 'i':
                options->op_inode = 1;
                break;
            case 'a':
                options->op_all = 1;
                break;
            case 'R':
                options->op_recursive = 1;
                break;
            case 't':
                options->op_tree = 1;
                break;
            case 'd':
                options->op_directory = 1;
                break;
            default:
                break;
        }

    }
}

int lsdir(char* path, struct op* options, struct args* arguments, int sub_level){
    struct dirent* e;
    struct stat    st_buf[1024];    
    struct stat    st;
    char           path_buf[1024];
    struct dirent* e_buf[1024]; 
    int i = 0;

    DIR* dp = opendir(path);
    if(dp == NULL) return -1;

    for (i = 0; e = readdir(dp); i++){
        e_buf[i] = e;
    }

    printf("%s:\n",path);   
    for(int k = 0; k < i; k++){
        if(e_buf[k]->d_name[0] == '.' && options->op_all == 0)continue;
        sprintf(path_buf, "%s/%s", path, e_buf[k]->d_name );
        print_stat(path_buf, options);
        printf("%s", e_buf[k]->d_name);
        printf("\n");
    }
    printf("\n");
    for(int k = 0; k < i; k++){
        if(e_buf[k]->d_type == DT_DIR && options->op_recursive == 1 && ((strcmp("..", e_buf[k]->d_name) != 0) && (strcmp(".", e_buf[k]->d_name) != 0))){
            sprintf(path_buf, "%s/%s", path, e_buf[k]->d_name );
            lsdir(path_buf, options, arguments, sub_level++);
        }
    }

    closedir(dp); // закрывать раньше?
    return 0;
}

int print_stat(const char* full_path, struct op* opt){
    struct stat st;
    stat(full_path, &st);
    if(opt->op_inode == 1) 
        printf("%ld ", st.st_ino);
    if(opt->op_long  == 1) {
        //printf(" ["); 
        for(int i = 0x4000; i > 0; i = i >> 1){
            switch (i){
                case(0x4000):                                          if((st.st_mode & i) == i){printf("d");} else {printf("-");} break;
                case(0b100000000):case(0b000100000):case(0b000000100): if((st.st_mode & i) == i){printf("r");} else {printf("-");} break;
                case(0b010000000):case(0b000010000):case(0b000000010): if((st.st_mode & i) == i){printf("w");} else {printf("-");} break;
                case(0b001000000):case(0b000001000):case(0b000000001): if((st.st_mode & i) == i){printf("x");} else {printf("-");} break;
                default: break;
            }
        }
        //printf("] ");

        printf(" %ld ", st.st_nlink);
        printf(" ");
        struct passwd* owner_info = getpwuid(st.st_uid);
        struct group*  ow_gr_info = getgrgid(st.st_gid);
        printf("%s ", owner_info->pw_name);
        printf("%s ", ow_gr_info->gr_name);
        printf("%ld ", st.st_size);
        printf("%ld ", st.st_mtime);
    }
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
       #include <sys/types->h>
       #include <pwd->h>

       int getpw(uid_t uid, char *buf);

        DESCRIPTION
       The getpw() function reconstructs the password line entry for the given user ID uid in the buffer buf->  The returned buffer contains a line of for‐
       mat

           name:passwd:uid:gid:gecos:dir:shell

       The passwd structure is defined in <pwd->h> as follows:

           struct passwd {
               char   *pw_name;        username 
               char   *pw_passwd;      user password 
               uid_t   pw_uid;         user ID 
               gid_t   pw_gid;         group ID
               char   *pw_gecos;       user information 
               char   *pw_dir;         home directory 
               char   *pw_shell;       shell program 
           };

for(int i = 0x100; i > 0; i >> 1){
        switch (i){
        case(0b100000000): if(st.st_mode & i == i){printf("r");} break;
        case(0b010000000): if(st.st_mode & i == i){printf("w");} break;
        case(0b001000000): if(st.st_mode & i == i){printf("x");} break;
        case(0b000100000): if(st.st_mode & i == i){printf("r");} break;
        case(0b000010000): if(st.st_mode & i == i){printf("w");} break;
        case(0b000001000): if(st.st_mode & i == i){printf("x");} break;
        case(0b000000100): if(st.st_mode & i == i){printf("r");} break;
        case(0b000000010): if(st.st_mode & i == i){printf("w");} break;
        case(0b000000001): if(st.st_mode & i == i){printf("x");} break;
        default:                                   printf("-");  break;
        }
    }
*/
