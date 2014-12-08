#include "../apue.h"
#include <sys/wait.h>

int main() 
{
    pid_t pid;
    
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        //父进程A的子进程B
        if ((pid = fork()) < 0) 
            err_sys("fork error");
        else if (pid > 0){
            //父进程A的子进程B退出，这里的子进程ID > 0
            printf("parent of the first child process, pid = %d\n",pid);
            exit(0);
        }

        //子进程B的子进程C继续
        sleep(10);
        printf("second child, parent pid = %d\n", getppid());
        exit(0);
    }
    //父进程A阻塞
    if (waitpid(pid, NULL, 0) != pid)
        err_sys("waitpid error");
    exit(0);
}
