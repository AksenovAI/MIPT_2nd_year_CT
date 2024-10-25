#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <grp.h>
#include <pwd.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

pid_t getpid(void);
pid_t getppid(void);
pid_t fork(void);

int main()
{
    printf("Parent's PID: %d\n", getpid());
    printf("Parent's PPID: %d\n\n", getppid());
    pid_t p = 0;

    for (int i = 1; i < 6; i++)
    {
        p = fork();

        if (p == 0)
        {
            printf("child %d PID: %d\n", i, getpid());
            printf("child %d PPID: %d\n\n", i, getppid());
        }

        else
        {
            sleep(1);
            break;
        }
    }
    return 0;
}
